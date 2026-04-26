//============================================================================
// File   : top_tb.cpp
// Brief  : Top 模块测试台（opcode 调度验证）
//
// 测试项：
//   Test 1 - OP_INIT：将 MODULUS/K_HALF/M 和 CG-NTT 旋转因子写入片上常量表
//   Test 2 - OP_ADD：多 limb 逐元素模加
//   Test 3 - OP_SUB：多 limb 逐元素模减
//   Test 4 - OP_MULT：多 limb 逐元素 Barrett 模乘
//   Test 5 - OP_NTT → OP_INTT 往返恢复（单 limb + 多 limb）
//   Test 6 - OP_AUTO：k 与 kinv 两次 automorphism 还原
//   Test 7 - OP_BCONV：BConv Q→P（sizeP=LIMB_P 与 sizeP=MAX_OUT_COLS）
//
// 说明：
//   - Top 函数内部维护 static MODULUS/K_HALF/M/NTTTwiddleFactor 等全局表，
//     故一次 OP_INIT 后，后续 ADD/SUB/MULT/NTT/INTT/AUTO 共用该状态。
//   - 为简化测试，所有 limb 使用同一 NTT-friendly 素数 3221225473 (≡1 mod 2N)。
//
// 编译：make csim MODULE=Top（需要先在 Makefile 中为 Top 绑定 TB_Top := ./testbench/top_tb.cpp）
//============================================================================

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>
#include <string>

#include "../include/top.h"
#include "../include/cg_ntt.h"

// ------------------------------------------------------------
// Co-Sim buffer depths — must match 'depth=' in top.cpp m_axi pragmas.
// wrapc copies exactly this many elements from the pointer on each call,
// so every Top() invocation must pass buffers of at least this size.
// ------------------------------------------------------------
static const int COSIM_D_IN1 = LIMB_Q * 3 + MAX_LIMBS * STAGE * CG_HALF_N; // 196617
static const int COSIM_D_IN2 = LIMB_P * 3 + MAX_LIMBS * STAGE * CG_HALF_N; // 196614
static const int COSIM_D_OUT = MAX_LIMBS * RING_DIM;                         // 32768

static uint64_t g_buf_in1[COSIM_D_IN1];
static uint64_t g_buf_in2[COSIM_D_IN2];
static uint64_t g_buf_out[COSIM_D_OUT];

static void TopSim(
    const uint64_t *in1, int n_in1,
    const uint64_t *in2, int n_in2,
    uint64_t *out, int n_out,
    uint8_t opcode, int num_active_limbs, int mod_index
) {
    memcpy(g_buf_in1, in1, n_in1 * sizeof(uint64_t));
    memcpy(g_buf_in2, in2, n_in2 * sizeof(uint64_t));
    memset(g_buf_out, 0, sizeof(g_buf_out));
    Top(g_buf_in1, g_buf_in2, g_buf_out, opcode, num_active_limbs, mod_index);
    memcpy(out, g_buf_out, n_out * sizeof(uint64_t));
}

// ------------------------------------------------------------
// 测试计数
// ------------------------------------------------------------
static int g_total  = 0;
static int g_passed = 0;

static void check(bool cond, const std::string &name) {
    ++g_total;
    if (cond) {
        ++g_passed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        std::cout << "  [FAIL] " << name << "\n";
    }
}

// ------------------------------------------------------------
// 软件参考：模运算
// ------------------------------------------------------------
static uint64_t sw_mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((unsigned __int128)a * b % mod);
}
static uint64_t sw_addmod(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t s = a + b;
    return (s >= mod) ? s - mod : s;
}
static uint64_t sw_submod(uint64_t a, uint64_t b, uint64_t mod) {
    return (a >= b) ? a - b : a + mod - b;
}
static uint64_t sw_powmod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t r = 1; base %= mod;
    while (exp) {
        if (exp & 1) r = sw_mulmod(r, base, mod);
        base = sw_mulmod(base, base, mod);
        exp >>= 1;
    }
    return r;
}

// Barrett 参数计算（与 cg_ntt_tb / arithmetic.cpp 语义一致）
static void compute_barrett_params(uint64_t mod, uint64_t &K_out, uint64_t &M_out) {
    uint64_t tmp = mod; int bits = 0;
    while (tmp) { tmp >>= 1; ++bits; }
    uint64_t S = (uint64_t)bits + 62;
    K_out = S;
    unsigned __int128 numer = (unsigned __int128)1 << S;
    M_out = (uint64_t)(numer / mod);
}

// bit-reverse
static int bit_reverse(int x, int bits) {
    int r = 0;
    for (int b = 0; b < bits; b++) { r = (r << 1) | (x & 1); x >>= 1; }
    return r;
}

// CG-NTT 旋转因子预计算（与 cg_ntt_tb.cpp 中同名函数一致）
static void build_cg_twiddle(
    uint64_t tf[STAGE][CG_HALF_N], int N, uint64_t mod, uint64_t root_2N)
{
    std::vector<int> perm(N);
    for (int i = 0; i < N; i++) perm[i] = bit_reverse(i, STAGE);

    for (int s = 0; s < STAGE; s++) {
        for (int i = 0; i < CG_HALF_N; i++) {
            int logical_a   = perm[i];
            int half_group  = 1 << s;
            int pos_in_half = logical_a % half_group;
            uint64_t exp    = (uint64_t)pos_in_half * ((uint64_t)N / half_group);
            tf[s][i] = sw_powmod(root_2N, exp % (2 * (uint64_t)N), mod);
        }
        std::vector<int> np(N);
        for (int i = 0; i < CG_HALF_N; i++) {
            np[2 * i]     = perm[i];
            np[2 * i + 1] = perm[i + CG_HALF_N];
        }
        perm = np;
    }
}

// ------------------------------------------------------------
// OP_INIT：构建 mem_in1 / mem_in2 布局并调用 Top
//
// mem_in1: [MOD×LIMB_Q] [K_HALF×LIMB_Q] [M×LIMB_Q] [NTT_TF: MAX_LIMBS×STAGE×CG_HALF_N]
// mem_in2: [MOD×LIMB_P] [K_HALF×LIMB_P] [M×LIMB_P] [INTT_TF: MAX_LIMBS×STAGE×CG_HALF_N]
// ------------------------------------------------------------
static const int CG_TF_SIZE       = STAGE * CG_HALF_N;                      // 24576
static const int INIT_MEM_IN1_LEN = LIMB_Q * 3 + MAX_LIMBS * CG_TF_SIZE;
static const int INIT_MEM_IN2_LEN = LIMB_P * 3 + MAX_LIMBS * CG_TF_SIZE;

static void do_init(uint64_t MOD, uint64_t K_HALF, uint64_t M_barrett,
                    uint64_t root_2N, uint64_t inv_root_2N)
{
    static std::vector<uint64_t> in1(INIT_MEM_IN1_LEN);
    static std::vector<uint64_t> in2(INIT_MEM_IN2_LEN);
    static std::vector<uint64_t> out_dummy(RING_DIM);  // OP_INIT 不写 mem_out

    // Q 参数
    for (int i = 0; i < LIMB_Q; i++) {
        in1[i]                = MOD;
        in1[LIMB_Q + i]       = K_HALF;
        in1[LIMB_Q * 2 + i]   = M_barrett;
    }
    // P 参数
    for (int j = 0; j < LIMB_P; j++) {
        in2[j]                = MOD;
        in2[LIMB_P + j]       = K_HALF;
        in2[LIMB_P * 2 + j]   = M_barrett;
    }

    // NTT twiddle
    static uint64_t ntt_tf [STAGE][CG_HALF_N];
    static uint64_t intt_tf[STAGE][CG_HALF_N];
    build_cg_twiddle(ntt_tf,  RING_DIM, MOD, root_2N);
    build_cg_twiddle(intt_tf, RING_DIM, MOD, inv_root_2N);

    const int NTT_BASE  = LIMB_Q * 3;
    const int INTT_BASE = LIMB_P * 3;
    for (int l = 0; l < MAX_LIMBS; l++) {
        for (int s = 0; s < STAGE; s++) {
            for (int t = 0; t < CG_HALF_N; t++) {
                in1[NTT_BASE  + l * CG_TF_SIZE + s * CG_HALF_N + t] = ntt_tf [s][t];
                in2[INTT_BASE + l * CG_TF_SIZE + s * CG_HALF_N + t] = intt_tf[s][t];
            }
        }
    }

    TopSim(in1.data(), (int)in1.size(), in2.data(), (int)in2.size(),
           out_dummy.data(), (int)out_dummy.size(),
           OP_INIT, 0, 0);
}

// ------------------------------------------------------------
// Test 1 — OP_INIT（只验证不崩溃；正确性由后续测试间接验证）
// ------------------------------------------------------------
static void test_init(uint64_t MOD, uint64_t K_HALF, uint64_t M_b,
                      uint64_t root_2N, uint64_t inv_root_2N)
{
    std::cout << "\n[Test 1] OP_INIT：写入 MODULUS/K_HALF/M 与 CG-NTT 旋转因子表\n";
    do_init(MOD, K_HALF, M_b, root_2N, inv_root_2N);
    check(true, "OP_INIT 调用返回");
}

// ------------------------------------------------------------
// 多 limb 数据打包辅助：mem_in/out 为线性数组 [num_limbs * RING_DIM]
// ------------------------------------------------------------
static void random_fill(uint64_t *dst, int num_u64, uint64_t mod, uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<uint64_t> dis(0, mod - 1);
    for (int i = 0; i < num_u64; i++) dst[i] = dis(rng);
}

// ------------------------------------------------------------
// Test 2 — OP_ADD
// ------------------------------------------------------------
static void test_add(uint64_t MOD, int num_limbs, int mod_index) {
    std::cout << "\n[Test 2] OP_ADD (num_limbs=" << num_limbs
              << ", mod_index=" << mod_index << ")\n";
    const int N = num_limbs * RING_DIM;
    std::vector<uint64_t> a(N), b(N), out(N);
    random_fill(a.data(), N, MOD, 0xA11);
    random_fill(b.data(), N, MOD, 0xB22);

    TopSim(a.data(), N, b.data(), N, out.data(), N, OP_ADD, num_limbs, mod_index);

    bool ok = true;
    int bad = 0;
    for (int i = 0; i < N && bad < 3; i++) {
        uint64_t want = sw_addmod(a[i], b[i], MOD);
        if (out[i] != want) {
            ok = false; bad++;
            std::cout << "  mismatch@" << i << " got=" << out[i]
                      << " want=" << want << "\n";
        }
    }
    check(ok, "OP_ADD 逐元素结果正确");
}

// ------------------------------------------------------------
// Test 3 — OP_SUB
// ------------------------------------------------------------
static void test_sub(uint64_t MOD, int num_limbs, int mod_index) {
    std::cout << "\n[Test 3] OP_SUB (num_limbs=" << num_limbs
              << ", mod_index=" << mod_index << ")\n";
    const int N = num_limbs * RING_DIM;
    std::vector<uint64_t> a(N), b(N), out(N);
    random_fill(a.data(), N, MOD, 0xC33);
    random_fill(b.data(), N, MOD, 0xD44);

    TopSim(a.data(), N, b.data(), N, out.data(), N, OP_SUB, num_limbs, mod_index);

    bool ok = true;
    int bad = 0;
    for (int i = 0; i < N && bad < 3; i++) {
        uint64_t want = sw_submod(a[i], b[i], MOD);
        if (out[i] != want) {
            ok = false; bad++;
            std::cout << "  mismatch@" << i << " got=" << out[i]
                      << " want=" << want << "\n";
        }
    }
    check(ok, "OP_SUB 逐元素结果正确");
}

// ------------------------------------------------------------
// Test 4 — OP_MULT（Barrett）
// ------------------------------------------------------------
static void test_mult(uint64_t MOD, int num_limbs, int mod_index) {
    std::cout << "\n[Test 4] OP_MULT (num_limbs=" << num_limbs
              << ", mod_index=" << mod_index << ")\n";
    const int N = num_limbs * RING_DIM;
    std::vector<uint64_t> a(N), b(N), out(N);
    random_fill(a.data(), N, MOD, 0xE55);
    random_fill(b.data(), N, MOD, 0xF66);

    TopSim(a.data(), N, b.data(), N, out.data(), N, OP_MULT, num_limbs, mod_index);

    bool ok = true;
    int bad = 0;
    for (int i = 0; i < N && bad < 3; i++) {
        uint64_t want = sw_mulmod(a[i], b[i], MOD);
        if (out[i] != want) {
            ok = false; bad++;
            std::cout << "  mismatch@" << i << " got=" << out[i]
                      << " want=" << want << "\n";
        }
    }
    check(ok, "OP_MULT 逐元素结果正确");
}

// ------------------------------------------------------------
// Test 5 — OP_NTT → OP_INTT 往返
// ------------------------------------------------------------
static void test_ntt_intt_roundtrip(uint64_t MOD, int num_limbs, int mod_index) {
    std::cout << "\n[Test 5] OP_NTT → OP_INTT 往返 (num_limbs=" << num_limbs
              << ", mod_index=" << mod_index << ")\n";
    const int N = num_limbs * RING_DIM;
    std::vector<uint64_t> in(N), ntt_out(N), back(N), dummy(N);
    random_fill(in.data(), N, MOD, 0x1234);

    // Forward
    TopSim(in.data(), N, dummy.data(), N, ntt_out.data(), N, OP_NTT, num_limbs, mod_index);
    bool changed = false;
    for (int i = 0; i < N; i++) if (ntt_out[i] != in[i]) { changed = true; break; }
    check(changed, "OP_NTT 产生变换（输出 ≠ 输入）");

    // Inverse
    TopSim(ntt_out.data(), N, dummy.data(), N, back.data(), N, OP_INTT, num_limbs, mod_index);

    bool ok = true;
    int bad = 0;
    for (int i = 0; i < N && bad < 3; i++) {
        if (back[i] != in[i]) {
            ok = false; bad++;
            std::cout << "  mismatch@" << i << " got=" << back[i]
                      << " want=" << in[i] << "\n";
        }
    }
    check(ok, "OP_INTT(OP_NTT(x)) == x");
}

// ------------------------------------------------------------
// Test 6 — OP_AUTO：k=5, kinv = 5^{-1} mod 2N，两次调用应恒等
// ------------------------------------------------------------
static uint32_t inv_mod_2N(uint32_t k, uint32_t two_N) {
    // 扩展欧几里得（两个都是正整数，k 与 2N=8192 互素）
    int64_t old_r = k,     r  = two_N;
    int64_t old_s = 1,     s  = 0;
    while (r != 0) {
        int64_t q = old_r / r;
        int64_t tr = old_r - q * r; old_r = r; r = tr;
        int64_t ts = old_s - q * s; old_s = s; s = ts;
    }
    int64_t inv = old_s % (int64_t)two_N;
    if (inv < 0) inv += two_N;
    return (uint32_t)inv;
}

static void test_auto_roundtrip(uint64_t MOD, int num_limbs, int mod_index) {
    std::cout << "\n[Test 6] OP_AUTO 往返 (num_limbs=" << num_limbs
              << ", mod_index=" << mod_index << ")\n";
    const int N = num_limbs * RING_DIM;
    const uint32_t two_N = 2 * RING_DIM;
    const uint32_t k     = 5;
    const uint32_t kinv  = inv_mod_2N(k, two_N);

    std::vector<uint64_t> in(N), mid(N), back(N);
    random_fill(in.data(), N, MOD, 0xAEF0);

    // mem_in2 = [k, kinv]
    std::vector<uint64_t> params_fwd = { k, kinv };
    std::vector<uint64_t> params_inv = { kinv, k };

    TopSim(in.data(),  N, params_fwd.data(), (int)params_fwd.size(), mid.data(),  N, OP_AUTO, num_limbs, mod_index);
    TopSim(mid.data(), N, params_inv.data(), (int)params_inv.size(), back.data(), N, OP_AUTO, num_limbs, mod_index);

    bool ok = true;
    int bad = 0;
    for (int i = 0; i < N && bad < 3; i++) {
        if (back[i] != in[i]) {
            ok = false; bad++;
            std::cout << "  mismatch@" << i << " got=" << back[i]
                      << " want=" << in[i] << "\n";
        }
    }
    check(ok, "OP_AUTO(kinv, OP_AUTO(k, x)) == x");
}

// ------------------------------------------------------------
// Test 7 — OP_BCONV：Q→P 基转换
//
// mem_in1 布局: [LIMB_Q × RING_DIM]  (Q limbs, 平铺)
// mem_in2 布局: [LIMB_Q × MAX_OUT_COLS  weights]
//              [MAX_OUT_COLS  out_mod]
//              [MAX_OUT_COLS  out_S]
//              [MAX_OUT_COLS  out_m_barrett]
// mem_out 布局: [sizeP × RING_DIM]   (P limbs, 平铺)
// ------------------------------------------------------------
static void test_bconv(uint64_t q_mod, int sizeP, unsigned seed) {
    std::cout << "\n[Test 7] OP_BCONV Q→P (sizeP=" << sizeP
              << ", seed=0x" << std::hex << seed << std::dec << ")\n";

    // P 输出列的素数模数（取 MAX_OUT_COLS 个，只使用前 sizeP 个）
    static const uint64_t P_MODS[MAX_OUT_COLS] = {
        998244353ULL, 786433ULL, 469762049ULL, 167772161ULL, 1004535809ULL,
    };

    // 随机生成输入与权重
    std::mt19937_64 rng(seed);
    std::vector<uint64_t> mem_in1(LIMB_Q * RING_DIM);
    for (auto &v : mem_in1) v = rng() % q_mod;

    uint64_t in_w[LIMB_Q][MAX_OUT_COLS];
    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            in_w[q][p] = rng() % P_MODS[p];

    // 打包 mem_in2: [weights=15] [out_mod=5] [out_S=5] [out_m_barrett=5]
    const int W_COUNT   = LIMB_Q * MAX_OUT_COLS;          // 15
    const int META_LEN  = W_COUNT + 3 * MAX_OUT_COLS;     // 30
    std::vector<uint64_t> mem_in2(META_LEN, 0);

    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            mem_in2[q * MAX_OUT_COLS + p] = in_w[q][p];

    const int MOD_OFF = W_COUNT;
    const int S_OFF   = MOD_OFF + MAX_OUT_COLS;
    const int M_OFF   = S_OFF   + MAX_OUT_COLS;
    for (int p = 0; p < MAX_OUT_COLS; ++p) {
        uint64_t pm = P_MODS[p];
        uint64_t S, m;
        compute_barrett_params(pm, S, m);
        mem_in2[MOD_OFF + p] = pm;
        mem_in2[S_OFF   + p] = S;
        mem_in2[M_OFF   + p] = m;
    }

    // 调用 Top (num_active_limbs = sizeP)
    std::vector<uint64_t> mem_out(sizeP * RING_DIM, 0);
    TopSim(mem_in1.data(), (int)mem_in1.size(), mem_in2.data(), (int)mem_in2.size(),
           mem_out.data(), (int)mem_out.size(), OP_BCONV, sizeP, 0);

    // 软件黄金参考
    bool ok = true;
    int  bad = 0;
    for (int p = 0; p < sizeP && bad < 3; ++p) {
        uint64_t pm = P_MODS[p];
        for (int n = 0; n < RING_DIM && bad < 3; ++n) {
            unsigned __int128 sum = 0;
            for (int q = 0; q < LIMB_Q; ++q)
                sum = (sum + (unsigned __int128)mem_in1[q * RING_DIM + n]
                           * in_w[q][p]) % pm;
            uint64_t got = mem_out[p * RING_DIM + n];
            if (got != (uint64_t)sum) {
                ok = false; ++bad;
                std::cout << "  mismatch p=" << p << " n=" << n
                          << " got=" << got << " want=" << (uint64_t)sum << "\n";
            }
        }
    }
    check(ok, "OP_BCONV Q→P 结果正确 (sizeP=" + std::to_string(sizeP) + ")");
}

// ------------------------------------------------------------
// main
// ------------------------------------------------------------
int main() {
    std::cout << "============================================================\n";
    std::cout << "  Top Kernel Testbench\n";
    std::cout << "  RING_DIM="    << RING_DIM
              << "  MAX_LIMBS="   << MAX_LIMBS
              << "  LIMB_Q="      << LIMB_Q
              << "  LIMB_P="      << LIMB_P << "\n";
    std::cout << "============================================================\n";

    // NTT-friendly 素数：3221225473 = 3·2^30 + 1 (≡ 1 mod 2·4096)
    const uint64_t MOD = 3221225473ULL;
    const uint64_t ROOT = 5ULL;
    uint64_t K_HALF, M_b;
    compute_barrett_params(MOD, K_HALF, M_b);
    uint64_t root_2N     = sw_powmod(ROOT, (MOD - 1) / (2 * RING_DIM), MOD);
    uint64_t inv_root_2N = sw_powmod(root_2N, MOD - 2, MOD);

    test_init(MOD, K_HALF, M_b, root_2N, inv_root_2N);

    test_add (MOD, /*num_limbs=*/LIMB_Q, /*mod_index=*/0);
    test_sub (MOD, /*num_limbs=*/LIMB_Q, /*mod_index=*/0);
    test_mult(MOD, /*num_limbs=*/LIMB_Q, /*mod_index=*/0);

    test_ntt_intt_roundtrip(MOD, /*num_limbs=*/1,      /*mod_index=*/0);
    test_ntt_intt_roundtrip(MOD, /*num_limbs=*/LIMB_Q, /*mod_index=*/0);

    test_auto_roundtrip(MOD, /*num_limbs=*/LIMB_Q, /*mod_index=*/0);

    // 偏移测试：num=2, mod_index=1 → 写入 buffer[1..2]
    test_add(MOD, /*num_limbs=*/2, /*mod_index=*/1);

    // Test 7 — OP_BCONV：典型 Q→P (sizeP=LIMB_P) 与全列 (sizeP=MAX_OUT_COLS)
    test_bconv(MOD, /*sizeP=*/LIMB_P,       /*seed=*/0xBC01U);
    test_bconv(MOD, /*sizeP=*/MAX_OUT_COLS, /*seed=*/0xBC02U);

    std::cout << "\n============================================================\n";
    std::cout << "  Top 结果：" << g_passed << " / " << g_total << " 通过\n";
    if (g_passed == g_total) {
        std::cout << "  *** Top ALL TESTS PASSED ***\n";
    } else {
        std::cout << "  *** Top " << (g_total - g_passed) << " TEST(S) FAILED ***\n";
    }
    std::cout << "  (含 OP_BCONV sizeP=LIMB_P 与 sizeP=MAX_OUT_COLS 两项)\n";
    std::cout << "============================================================\n";
    return (g_passed == g_total) ? 0 : 1;
}
