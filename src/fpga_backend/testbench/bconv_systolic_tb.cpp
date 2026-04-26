// =============================================================================
// bconv_systolic_tb.cpp — Compute_BConv_Systolic C-Simulation Testbench
//
// 测试策略
//   TC-0  最小烟雾测试   sizeP=1，小模数，便于手算验证
//   TC-1  典型 Q→P 转换  sizeP=LIMB_P(2)，~20-bit 素数
//   TC-2  全列满载        sizeP=MAX_OUT_COLS(5)
//   TC-3  全零输入        结果应全为 0
//   TC-4  权重全零        结果应全为 0
//   TC-5  输入/权重 = mod-1（最大合法值）
//   TC-6  随机压力测试    多轮随机 sizeP + 随机激励
// =============================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <string>
#include <algorithm>

#include <ap_int.h>
#include "../include/bconv_systolic.h"

using namespace std;

// ---------------------------------------------------------------------------
// 软件黄金模型（128-bit 精确计算）
// ---------------------------------------------------------------------------
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
                    sum %= mod_p;
                }
                golden[p][r][c] = (uint64_t)sum;
            }
        }
    }
}

static int compare_results(
    const uint64_t hw_in_x[MAX_LIMBS][SQRT][SQRT],
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
// Barrett 常数计算（匹配 MultMod 约定: S = bitwidth(mod) + 62）
// ---------------------------------------------------------------------------
static uint64_t barrett_compute(uint64_t mod, uint64_t &S_out) {
    int bw = 0;
    for (uint64_t v = mod; v > 0; v >>= 1) ++bw;
    S_out = (uint64_t)(bw + 62);
    unsigned __int128 two_S = (unsigned __int128)1 << S_out;
    return (uint64_t)(two_S / (unsigned __int128)mod);
}

// ---------------------------------------------------------------------------
// 测试辅助结构
// ---------------------------------------------------------------------------
struct TestArrays {
    uint64_t (*in_x)[SQRT][SQRT];
    uint64_t (*golden)[SQRT][SQRT];
    uint64_t in_w        [LIMB_Q]      [MAX_OUT_COLS];
    uint64_t out_mod     [MAX_OUT_COLS];
    uint64_t out_S       [MAX_OUT_COLS];
    uint64_t out_m_barrett[MAX_OUT_COLS];

    TestArrays() {
        in_x   = new uint64_t[MAX_LIMBS]    [SQRT][SQRT];
        golden = new uint64_t[MAX_OUT_COLS]  [SQRT][SQRT];
    }
    ~TestArrays() {
        delete[] in_x;
        delete[] golden;
    }
    void clear() {
        memset(in_x,        0, sizeof(uint64_t) * MAX_LIMBS    * SQRT * SQRT);
        memset(golden,      0, sizeof(uint64_t) * MAX_OUT_COLS * SQRT * SQRT);
        memset(in_w,        0, sizeof(in_w));
        memset(out_mod,     0, sizeof(out_mod));
        memset(out_S,       0, sizeof(out_S));
        memset(out_m_barrett, 0, sizeof(out_m_barrett));
    }
    void set_mod(int p, uint64_t mod) {
        out_mod[p]      = mod;
        out_m_barrett[p] = barrett_compute(mod, out_S[p]);
    }

    int run_and_compare(int sizeP, const string &name) {
        golden_bconv(in_x, in_w, out_mod, sizeP, golden);
        Compute_BConv_Systolic(in_x, in_w, out_mod, out_S, out_m_barrett, sizeP);
        return compare_results(in_x, golden, sizeP, name);
    }
};

// ---------------------------------------------------------------------------
// 测试用例
// ---------------------------------------------------------------------------

static int tc0_smoke() {
    cout << "[TC-0] Smoke test  sizeP=1, mod=17\n";
    TestArrays t;
    t.clear();
    t.set_mod(0, 17ULL);
    // 非活跃列设非零模数，防止 PE 除零（HLS sim 中全列都跑）
    for (int p = 1; p < MAX_OUT_COLS; ++p) t.set_mod(p, 17ULL);

    t.in_w[0][0] = 3; t.in_w[1][0] = 1; t.in_w[2][0] = 1;
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = 5;
    // 期望：(5*3 + 5*1 + 5*1) % 17 = 25 % 17 = 8

    int err = t.run_and_compare(1, "TC-0");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc1_typical_qp(unsigned seed) {
    cout << "[TC-1] Typical Q->P  sizeP=" << LIMB_P << "  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);

    uint64_t mods[MAX_OUT_COLS] = {
        131071ULL, 262139ULL, 524287ULL, 1048573ULL, 2097143ULL,
    };
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, mods[p]);
    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = rand() % mods[p];
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = rand() % mods[q % MAX_OUT_COLS];

    int err = t.run_and_compare(LIMB_P, "TC-1");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc2_full_cols(unsigned seed) {
    cout << "[TC-2] Full columns  sizeP=" << MAX_OUT_COLS << "  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);

    uint64_t mods[MAX_OUT_COLS] = {
        998244353ULL, 1004535809ULL, 786433ULL, 469762049ULL, 167772161ULL,
    };
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, mods[p]);
    uint64_t min_mod = *min_element(mods, mods + MAX_OUT_COLS);
    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = (uint64_t)rand() % mods[p];
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = (uint64_t)rand() % min_mod;

    int err = t.run_and_compare(MAX_OUT_COLS, "TC-2");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc3_zero_input() {
    cout << "[TC-3] Zero input  sizeP=2\n";
    TestArrays t;
    t.clear();
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, 998244353ULL);
    t.set_mod(1, 786433ULL);
    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = 123456;

    int err = t.run_and_compare(2, "TC-3");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc4_zero_weights(unsigned seed) {
    cout << "[TC-4] Zero weights  sizeP=2  seed=" << seed << "\n";
    TestArrays t;
    t.clear();
    srand(seed);
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, 998244353ULL);
    t.set_mod(1, 786433ULL);
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = (uint64_t)rand() % 786433ULL;

    int err = t.run_and_compare(2, "TC-4");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc5_max_values() {
    cout << "[TC-5] Max values (mod-1 inputs & weights)  sizeP=2\n";
    TestArrays t;
    t.clear();
    uint64_t mods[MAX_OUT_COLS] = {
        998244353ULL, 786433ULL, 131071ULL, 262139ULL, 524287ULL,
    };
    for (int p = 0; p < MAX_OUT_COLS; ++p) t.set_mod(p, mods[p]);
    for (int q = 0; q < LIMB_Q; ++q)
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.in_w[q][p] = mods[p] - 1;
    for (int q = 0; q < LIMB_Q; ++q)
        for (int r = 0; r < SQRT; ++r)
            for (int c = 0; c < SQRT; ++c)
                t.in_x[q][r][c] = (mods[0] - 1) % mods[1];

    int err = t.run_and_compare(2, "TC-5");
    cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
    return err;
}

static int tc6_random_stress(int rounds = 4) {
    cout << "[TC-6] Random stress  rounds=" << rounds << "\n";
    int total_errors = 0;

    static const uint64_t PRIME_POOL[] = {
        998244353ULL, 1004535809ULL, 786433ULL,
        469762049ULL, 167772161ULL,  2013265921ULL,
        131071ULL,    262139ULL,     524287ULL, 1048573ULL,
    };
    static const int POOL_SIZE = (int)(sizeof(PRIME_POOL) / sizeof(PRIME_POOL[0]));

    for (int round = 0; round < rounds; ++round) {
        unsigned seed = 0x1000 + round * 0x3F7;
        srand(seed);
        int sizeP = 1 + rand() % MAX_OUT_COLS;
        cout << "  round=" << round << " sizeP=" << sizeP
             << " seed=0x" << hex << seed << dec << "\n";

        TestArrays t;
        t.clear();
        for (int p = 0; p < MAX_OUT_COLS; ++p)
            t.set_mod(p, PRIME_POOL[rand() % POOL_SIZE]);
        for (int q = 0; q < LIMB_Q; ++q)
            for (int p = 0; p < MAX_OUT_COLS; ++p)
                t.in_w[q][p] = (uint64_t)rand() % t.out_mod[p];
        for (int q = 0; q < LIMB_Q; ++q)
            for (int r = 0; r < SQRT; ++r)
                for (int c = 0; c < SQRT; ++c)
                    t.in_x[q][r][c] = (uint64_t)rand() % t.out_mod[q % MAX_OUT_COLS];

        int err = t.run_and_compare(sizeP, "TC-6-r" + to_string(round));
        cout << (err == 0 ? "  PASS\n" : "  FAIL  errors=" + to_string(err) + "\n");
        total_errors += err;
    }
    return total_errors;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
    cout << "============================================================\n";
    cout << "   BConv Systolic Testbench\n";
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
    if (total_errors == 0)
        cout << "  [ALL PASS]  7 test cases passed, 0 errors.\n";
    else
        cout << "  [FAILED]  total mismatches = " << total_errors << "\n";
    cout << "============================================================\n";

    return (total_errors == 0) ? 0 : 1;
}
