// =============================================================================
// bconv_tb.cpp — BConv Systolic Array C-Simulation Testbench
//
// 测试策略
//   TC-0  最小规模烟雾测试   sizeP = 1，小模数，便于手算验证
//   TC-1  典型 Q→P 转换     sizeP = LIMB_P (2)，使用 ~60-bit 素数级模数
//   TC-2  全列满载           sizeP = MAX_OUT_COLS (5)，全部输出列同时工作
//   TC-3  边界：全零输入     sizeP = 2，结果应全为 0
//   TC-4  边界：权重全零     sizeP = 2，结果应全为 0
//   TC-5  边界：输入值 = mod-1（最大合法值）
//   TC-6  随机压力测试       多轮随机 sizeP + 随机激励
//
// 黄金模型
//   对每个输出列 p，对每个系数 (r,c)：
//     out[p][r][c] = ( sum_{q=0}^{LIMB_Q-1}  in_x[q][r][c] * in_w[q][p] ) mod out_mod[p]
//   使用 128-bit 中间量，不会溢出。
//
// Barrett 参数计算（与 arithmetic.cpp 中 MultMod 的逻辑保持一致）
//   k_half = bit_width(mod)          // 等于 ceil(log2(mod+1))
//   m      = floor( 2^(2*k_half) / mod )
// =============================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <string>

#include <ap_int.h>
#include "../include/bconv.h"   // Compute_BConv + define.h + arithmetic.h

using namespace std;

// ---------------------------------------------------------------------------
// 工具函数
// ---------------------------------------------------------------------------

// 计算 mod 的位宽（即最小的 k 使得 2^k > mod）
static uint64_t bit_width(uint64_t mod) {
    uint64_t tmp = mod;
    uint64_t bits = 0;
    while (tmp) { tmp >>= 1; ++bits; }
    return bits;
}

// 根据 mod 计算 Barrett 参数
static void compute_barrett(uint64_t mod,
                             uint64_t &out_k_half,
                             uint64_t &out_m_barrett) {
    assert(mod > 1);
    uint64_t k = bit_width(mod);
    out_k_half = k;
    // m = floor( 2^(2k) / mod )
    unsigned __int128 num = (unsigned __int128)1 << (2 * k);
    out_m_barrett = (uint64_t)(num / mod);
}

// 软件黄金模型（128-bit 精确计算，无任何近似）
static void golden_bconv(
    const uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP,
    uint64_t golden[MAX_OUT_COLS][SQRT][SQRT])
{
    for (int p = 0; p < sizeP; ++p) {
        uint64_t mod_p = out_mod[p];
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                unsigned __int128 sum = 0;
                for (int q = 0; q < LIMB_Q; ++q) {
                    sum += (unsigned __int128)in_x[q][r][c] *
                           (unsigned __int128)in_w[q][p];
                    // 中途规约防止溢出（LIMB_Q 最多 3 次，128-bit 完全够用，
                    // 但保持与黄金模型的简洁性，在每次累加后取模）
                    sum %= mod_p;
                }
                golden[p][r][c] = (uint64_t)sum;
            }
        }
    }
}

// 比较 HW 结果与黄金模型，返回不匹配数量
static int compare_results(
    const uint64_t hw_in_x[MAX_LIMBS][SQRT][SQRT],   // HW 结果存在 [LIMB_Q + p] 处
    const uint64_t golden[MAX_OUT_COLS][SQRT][SQRT],
    int sizeP,
    const string &tc_name,
    int max_print = 5)
{
    int errors = 0;
    for (int p = 0; p < sizeP; ++p) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                uint64_t hw  = hw_in_x[LIMB_Q + p][r][c];
                uint64_t ref = golden[p][r][c];
                if (hw != ref) {
                    if (errors < max_print) {
                        cout << "  [" << tc_name << "] MISMATCH p=" << p
                             << " r=" << r << " c=" << c
                             << "  HW=0x" << hex << hw
                             << "  SW=0x" << ref << dec << "\n";
                    }
                    ++errors;
                }
            }
        }
    }
    return errors;
}

// ---------------------------------------------------------------------------
// 每个测试用例封装（使用堆内存，避免栈溢出）
// ---------------------------------------------------------------------------

struct TestArrays {
    uint64_t (*in_x)[SQRT][SQRT];          // [MAX_LIMBS][SQRT][SQRT]
    uint64_t (*golden)[SQRT][SQRT];         // [MAX_OUT_COLS][SQRT][SQRT]
    uint64_t in_w[LIMB_Q][MAX_OUT_COLS];
    uint64_t out_mod[MAX_OUT_COLS];
    uint64_t out_k_half[MAX_OUT_COLS];
    uint64_t out_m_barrett[MAX_OUT_COLS];

    TestArrays() {
        in_x   = new uint64_t[MAX_LIMBS][SQRT][SQRT];
        golden = new uint64_t[MAX_OUT_COLS][SQRT][SQRT];
    }
    ~TestArrays() {
        delete[] in_x;
        delete[] golden;
    }
    void clear() {
        memset(in_x,   0, sizeof(uint64_t) * MAX_LIMBS    * SQRT * SQRT);
        memset(golden, 0, sizeof(uint64_t) * MAX_OUT_COLS * SQRT * SQRT);
        memset(in_w, 0, sizeof(in_w));
        memset(out_mod, 0, sizeof(out_mod));
        memset(out_k_half, 0, sizeof(out_k_half));
        memset(out_m_barrett, 0, sizeof(out_m_barrett));
    }
    // 设置一列模数及其 Barrett 参数
    void set_mod(int p, uint64_t mod) {
        out_mod[p] = mod;
        compute_barrett(mod, out_k_half[p], out_m_barrett[p]);
    }
    // 运行 HW + SW，返回 error 数量
    int run_and_compare(int sizeP, const string &name) {
        // SW 黄金模型
        golden_bconv(in_x, in_w, out_mod, sizeP, golden);
        // HW
        Compute_BConv(in_x, in_w, out_mod, out_k_half, out_m_barrett, sizeP);
        // 比较
        return compare_results(in_x, golden, sizeP, name);
    }
};

// ---------------------------------------------------------------------------
// 测试用例
// ---------------------------------------------------------------------------

// TC-0：最小烟雾测试
static int tc0_smoke() {
    cout << "[TC-0] Smoke test  sizeP=1, small modulus\n";
    TestArrays t;
    t.clear();

    // 使用一个很小的模数（17），方便手算
    int sizeP = 1;
    t.set_mod(0, 17ULL);

    // 权重：q=0 → p=0 为 3，其余为 1
    t.in_w[0][0] = 3;
    t.in_w[1][0] = 1;
    t.in_w[2][0] = 1;

    // 输入：所有系数为 5
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = 5;
    // 期望：(5*3 + 5*1 + 5*1) % 17 = 25 % 17 = 8

    int err = t.run_and_compare(sizeP, "TC-0");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-1：典型 Q→P 转换（sizeP = LIMB_P = 2），使用真实 HE 模数量级
static int tc1_typical_qp(unsigned seed) {
    cout << "[TC-1] Typical Q->P  sizeP=" << LIMB_P << "  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);

    // 使用典型的小素数（约 17~18 bit），足够测试 Barrett 正确性
    uint64_t mods[MAX_OUT_COLS] = {
        131071ULL,   // 17-bit prime
        262139ULL,   // 18-bit prime
        524287ULL,   // 19-bit prime
        1048573ULL,  // 20-bit prime
        2097143ULL,  // 21-bit prime
    };
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, mods[p]);

    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = rand() % mods[p];

    // mods[] 长度为 MAX_OUT_COLS，q 最大为 LIMB_Q-1；
    // 用 q % MAX_OUT_COLS 确保不越界读取 mods[]。
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = rand() % mods[q % MAX_OUT_COLS];

    int err = t.run_and_compare(LIMB_P, "TC-1");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-2：全列满载（sizeP = MAX_OUT_COLS = 5）
static int tc2_full_cols(unsigned seed) {
    cout << "[TC-2] Full columns  sizeP=" << MAX_OUT_COLS << "  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);

    uint64_t mods[MAX_OUT_COLS] = {
        998244353ULL,   // ~30-bit prime (NTT-friendly)
        1004535809ULL,  // ~30-bit prime
        786433ULL,      // ~20-bit prime
        469762049ULL,   // ~29-bit prime
        167772161ULL,   // ~28-bit prime
    };
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, mods[p]);

    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = (uint64_t)rand() % mods[p];

    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = (uint64_t)rand() % mods[q % MAX_OUT_COLS];

    int err = t.run_and_compare(MAX_OUT_COLS, "TC-2");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-3：全零输入 → 结果应全为 0
static int tc3_zero_input() {
    cout << "[TC-3] Zero input  sizeP=2\n";
    TestArrays t;
    t.clear();

    int sizeP = 2;
    t.set_mod(0, 998244353ULL);
    t.set_mod(1, 786433ULL);

    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = 123456;   // 非零权重
    // in_x 全零（clear 已清零）

    int err = t.run_and_compare(sizeP, "TC-3");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-4：权重全零 → 结果应全为 0
static int tc4_zero_weights(unsigned seed) {
    cout << "[TC-4] Zero weights  sizeP=2  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);

    int sizeP = 2;
    t.set_mod(0, 998244353ULL);
    t.set_mod(1, 786433ULL);

    // in_w 全零（clear 已清零）
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = (uint64_t)rand() % 998244353ULL;

    int err = t.run_and_compare(sizeP, "TC-4");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-5：输入值 = mod-1（最大合法值），权重 = mod-1
//        压力测试 Barrett 约减上界
static int tc5_max_values() {
    cout << "[TC-5] Max values (mod-1 inputs & weights)  sizeP=2\n";
    TestArrays t;
    t.clear();

    int sizeP = 2;
    uint64_t mod0 = 998244353ULL;
    uint64_t mod1 = 786433ULL;
    t.set_mod(0, mod0);
    t.set_mod(1, mod1);
    // 其余列也需设置合法模数（systolic 循环里会访问 sizeP 之外的寄存器，但结果只写前 sizeP 列）
    t.set_mod(2, 131071ULL);
    t.set_mod(3, 262139ULL);
    t.set_mod(4, 524287ULL);

    for (int q = 0; q < LIMB_Q; ++q) {
        t.in_w[q][0] = mod0 - 1;
        t.in_w[q][1] = mod1 - 1;
        for (int p = 2; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = t.out_mod[p] - 1;
    }

    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = mod0 - 1;   // 大于 mod1，但黄金模型内部会取模

    int err = t.run_and_compare(sizeP, "TC-5");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

// TC-6：随机压力测试（多轮，随机 sizeP）
static int tc6_random_stress(int rounds = 4) {
    cout << "[TC-6] Random stress  rounds=" << rounds << "\n";
    int total_errors = 0;

    // 一组较大的素数，覆盖 30~40 bit 范围
    static const uint64_t PRIME_POOL[] = {
        998244353ULL,
        1004535809ULL,
        786433ULL,
        469762049ULL,
        167772161ULL,
        2013265921ULL,
        1811939329ULL,
        2013265921ULL,
        1004535809ULL,
        206158430209ULL,    // 38-bit prime
    };
    static const int POOL_SIZE = (int)(sizeof(PRIME_POOL) / sizeof(PRIME_POOL[0]));

    for (int round = 0; round < rounds; ++round) {
        unsigned seed = 0x1000 + round * 0x3F7;
        srand(seed);

        // 随机选 sizeP（1 ~ MAX_OUT_COLS）
        int sizeP = 1 + rand() % MAX_OUT_COLS;
        cout << "  round=" << round << " sizeP=" << sizeP << " seed=0x" << hex << seed << dec << "\n";

        TestArrays t;
        t.clear();

        // 从 PRIME_POOL 中随机选取模数
        for (int p = 0; p < MAX_OUT_COLS; ++p) {
            uint64_t mod = PRIME_POOL[rand() % POOL_SIZE];
            t.set_mod(p, mod);
        }

        for (int q = 0; q < LIMB_Q; ++q)
            for (int p = 0; p < MAX_OUT_COLS; ++p)
                t.in_w[q][p] = (uint64_t)rand() % t.out_mod[p];

        for (int q = 0; q < LIMB_Q; ++q)
            for (int r = 0; r < SQRT; ++r)
                for (int c = 0; c < SQRT; ++c)
                    t.in_x[q][r][c] = (uint64_t)rand() % t.out_mod[q % MAX_OUT_COLS];

        int err = t.run_and_compare(sizeP, "TC-6-r" + to_string(round));
        if (err) cout << "  FAIL  errors=" << err << "\n";
        else     cout << "  PASS\n";
        total_errors += err;
    }
    return total_errors;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
    cout << "============================================================\n";
    cout << "   BConv Systolic Array Testbench\n";
    cout << "   RING_DIM=" << RING_DIM
         << "  LIMB_Q=" << LIMB_Q
         << "  MAX_OUT_COLS=" << MAX_OUT_COLS << "\n";
    cout << "============================================================\n\n";

    int total_errors = 0;

    total_errors += tc0_smoke();
    total_errors += tc1_typical_qp(0xABCD1234U);
    total_errors += tc2_full_cols(0xDEADBEEFU);
    total_errors += tc3_zero_input();
    total_errors += tc4_zero_weights(0xCAFEBABEU);
    total_errors += tc5_max_values();
    total_errors += tc6_random_stress(4);

    cout << "\n============================================================\n";
    if (total_errors == 0) {
        cout << "  [ALL PASS]  7 test cases passed, 0 errors.\n";
    } else {
        cout << "  [FAILED]  total mismatches = " << total_errors << "\n";
    }
    cout << "============================================================\n";

    return (total_errors == 0) ? 0 : 1;
}
