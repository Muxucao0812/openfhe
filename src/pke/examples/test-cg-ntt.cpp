//==================================================================================
// BSD 2-Clause License
//
// Copyright (c) 2014-2022, NJIT, Duality Technologies Inc. and other contributors
// All rights reserved.
//==================================================================================

/*
  CG-NTT Unit Tests
  专门用于验证 CG-NTT（Constant Geometry NTT）从 Host 到 FPGA 的完整通路。

  与 test-fpga-modules.cpp 的区别：
    - 本文件聚焦 NTT/INTT，覆盖 CG-NTT 重构后最易出错的旋转因子几何
    - 增加多 limb 同构性测试、跨 limb 独立性测试、确定性测试
    - 增加 delta / 常向量 等结构化输入的稳态检查
    - 增加 OpenFHE CPU NTT（bit-reversed 输出）作参考，配合 bit-reverse 还原
 */

#define PROFILE

#include "openfhe.h"
#include "FpgaManager.h"

#include "math/hal/intnat/transformnat.h"
#include "math/hal/intnat/mubintvecnat.h"

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace lbcrypto;

// =============================================================================
// 小工具
// =============================================================================

static void PrintSection(const std::string& title) {
    std::cout << "\n================================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "================================================" << std::endl;
}

static bool CheckEqual(const std::string& tag,
                       const std::vector<uint64_t>& got,
                       const std::vector<uint64_t>& ref,
                       size_t printLen = 8) {
    size_t mismatches = 0;
    for (size_t i = 0; i < got.size(); ++i) {
        if (got[i] != ref[i]) {
            if (mismatches < 3) {
                std::cerr << "  [MISMATCH] " << tag << " idx=" << i
                          << " got=" << got[i] << " ref=" << ref[i] << std::endl;
            }
            ++mismatches;
        }
    }
    if (mismatches == 0) {
        std::cout << "  [PASS] " << tag;
        if (printLen > 0) {
            std::cout << "   first " << printLen << ": ";
            for (size_t i = 0; i < printLen && i < got.size(); ++i)
                std::cout << got[i] << " ";
        }
        std::cout << std::endl;
    } else {
        std::cerr << "  [FAIL] " << tag << "  total mismatches = "
                  << mismatches << " / " << got.size() << std::endl;
    }
    return mismatches == 0;
}

// bit-reverse（位宽由 ring dim 决定，log2(N) 比特）
static size_t BitReverse(size_t x, int bits) {
    size_t r = 0;
    for (int b = 0; b < bits; ++b) { r = (r << 1) | (x & 1); x >>= 1; }
    return r;
}

static int IntLog2(size_t n) {
    int r = 0;
    while ((size_t(1) << r) < n) ++r;
    return r;
}

// 用 OpenFHE CPU NTT 生成参考值（negacyclic forward NTT，输出位于 bit-reversed 顺序）
// 返回值：自然顺序的 NTT 结果（已 un-bit-reverse）
// 这样无论 FPGA 输出是 natural 还是 bit-reversed，上层只需选一致的比较策略。
static std::vector<uint64_t> CpuNttReferenceNatural(
    const std::vector<uint64_t>& poly_in,
    uint64_t modulus,
    uint64_t root_2N,
    size_t N)
{
    using IntType = intnat::NativeInteger;
    using VecType = intnat::NativeVector;

    IntType mod(modulus);
    IntType psi(root_2N);
    size_t cycloOrder = 2 * N;

    // 预计算 rootOfUnityTable（bit-reversed 顺序的 ψ^i，0 ≤ i < N）
    VecType rootTable(N, mod);
    IntType acc(1);
    std::vector<IntType> natural(N);
    for (size_t i = 0; i < N; ++i) {
        natural[i] = acc;
        acc = acc.ModMul(psi, mod);
    }
    int logN = IntLog2(N);
    for (size_t i = 0; i < N; ++i)
        rootTable[BitReverse(i, logN)] = natural[i];

    VecType input(N, mod);
    for (size_t i = 0; i < N; ++i) input[i] = IntType(poly_in[i]);

    VecType output(N, mod);
    intnat::NumberTheoreticTransformNat<VecType>().ForwardTransformToBitReverse(
        input, rootTable, &output);
    (void)cycloOrder;

    // 转回自然顺序
    std::vector<uint64_t> result(N);
    for (size_t i = 0; i < N; ++i)
        result[BitReverse(i, logN)] = output[i].ConvertToInt();
    return result;
}

// =============================================================================
// 初始化：从 CryptoContext 提取模数与单位根并下发 FPGA
// =============================================================================
static bool InitFpga(CryptoContext<DCRTPoly>& cc,
                     std::vector<uint64_t>& q_mods,
                     std::vector<uint64_t>& q_roots,
                     std::vector<uint64_t>& p_mods,
                     std::vector<uint64_t>& p_roots) {
    q_mods.clear(); q_roots.clear(); p_mods.clear(); p_roots.clear();

    auto ringDim = cc->GetRingDimension();
    std::cout << "[Host] Ring Dimension: " << ringDim << std::endl;

    auto elementParams = cc->GetElementParams();
    const auto& rnsParams = elementParams->GetParams();
    std::cout << "[Host] Q (ciphertext moduli):" << std::endl;
    for (size_t i = 0; i < rnsParams.size(); ++i) {
        uint64_t m = rnsParams[i]->GetModulus().ConvertToInt();
        uint64_t r = rnsParams[i]->GetRootOfUnity().ConvertToInt();
        q_mods.push_back(m);
        q_roots.push_back(r);
        std::cout << "  Q[" << i << "] mod=" << m << "  ψ=" << r << std::endl;
    }

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    auto paramsP = cryptoParams ? cryptoParams->GetParamsP() : nullptr;
    if (paramsP) {
        std::cout << "[Host] P (auxiliary moduli):" << std::endl;
        const auto& rnsParamsP = paramsP->GetParams();
        for (size_t i = 0; i < rnsParamsP.size(); ++i) {
            uint64_t m = rnsParamsP[i]->GetModulus().ConvertToInt();
            uint64_t r = rnsParamsP[i]->GetRootOfUnity().ConvertToInt();
            p_mods.push_back(m);
            p_roots.push_back(r);
            std::cout << "  P[" << i << "] mod=" << m << "  ψ=" << r << std::endl;
        }
    }

    if (!FpgaManager::GetInstance().IsReady()) {
        std::cout << "[Host] FPGA not ready — aborting hardware tests." << std::endl;
        return false;
    }
    std::cout << "[Host] Sending CG-NTT twiddle tables to FPGA ..." << std::endl;
    FpgaManager::GetInstance().InitModuli(q_mods, p_mods, q_roots, p_roots);
    std::cout << "[Host] FPGA Initialization Done." << std::endl;
    return true;
}

// =============================================================================
// 测试 1：NTT Round-trip （最基础的健全性检查）
// INTT(NTT(x)) == x  对于随机输入 / delta / 常数向量 都应成立
// =============================================================================
static bool TestRoundTrip(uint64_t modulus, size_t N, const std::string& label,
                          const std::vector<uint64_t>& poly_in) {
    std::vector<uint64_t> ntt_out(N, 0), intt_out(N, 0);
    FpgaManager::GetInstance().NttForwardOffload(poly_in.data(), ntt_out.data(), modulus, N);
    FpgaManager::GetInstance().NttInverseOffload(ntt_out.data(), intt_out.data(), modulus, N);
    return CheckEqual("Round-trip " + label, intt_out, poly_in, /*printLen=*/4);
}

static bool TestRoundTripSuite(uint64_t modulus, size_t N) {
    PrintSection("TEST 1: NTT Round-trip (INTT(NTT(x)) == x)");
    bool ok = true;

    // (a) 随机输入
    std::vector<uint64_t> rand_in(N);
    std::mt19937_64 rng(0xC0FFEEULL);
    for (size_t i = 0; i < N; ++i) rand_in[i] = rng() % modulus;
    ok &= TestRoundTrip(modulus, N, "random", rand_in);

    // (b) delta: x[0]=1, others=0
    std::vector<uint64_t> delta(N, 0); delta[0] = 1;
    ok &= TestRoundTrip(modulus, N, "delta[0]", delta);

    // (c) delta: x[1]=1, others=0
    std::vector<uint64_t> delta1(N, 0); delta1[1] = 1;
    ok &= TestRoundTrip(modulus, N, "delta[1]", delta1);

    // (d) 常向量
    std::vector<uint64_t> ones(N, 7);
    ok &= TestRoundTrip(modulus, N, "constant(7)", ones);

    // (e) 线性序列
    std::vector<uint64_t> lin(N);
    for (size_t i = 0; i < N; ++i) lin[i] = (i * 37 + 11) % modulus;
    ok &= TestRoundTrip(modulus, N, "linear", lin);
    return ok;
}

// =============================================================================
// 测试 2：Forward NTT vs CPU 参考
// 这才是真正能捕获 CG-NTT 旋转因子几何错误的测试。
// 策略：把 FPGA 输出与 CPU 生成的"自然顺序 NTT"作比较。
// 若两者不匹配但 bit-reverse(FPGA) 匹配，则说明 FPGA 输出是 bit-reversed 顺序
// （CG-NTT 的已知特性），自动识别并提示。
// =============================================================================
static bool TestForwardVsCpu(uint64_t modulus, uint64_t psi, size_t N) {
    PrintSection("TEST 2: Forward NTT vs CPU reference");

    std::vector<uint64_t> poly_in(N);
    std::mt19937_64 rng(0xBADC0DEULL);
    for (size_t i = 0; i < N; ++i) poly_in[i] = rng() % modulus;

    std::vector<uint64_t> fpga_out(N, 0);
    FpgaManager::GetInstance().NttForwardOffload(poly_in.data(), fpga_out.data(), modulus, N);

    auto ref_natural = CpuNttReferenceNatural(poly_in, modulus, psi, N);

    // 先尝试 natural 比对
    bool match_natural = (fpga_out == ref_natural);
    if (match_natural) {
        std::cout << "  [PASS] FPGA forward NTT matches CPU in NATURAL order." << std::endl;
        return true;
    }

    // 再尝试 bit-reversed 比对
    int logN = IntLog2(N);
    std::vector<uint64_t> fpga_bitrev(N);
    for (size_t i = 0; i < N; ++i)
        fpga_bitrev[BitReverse(i, logN)] = fpga_out[i];
    bool match_bitrev = (fpga_bitrev == ref_natural);
    if (match_bitrev) {
        std::cout << "  [PASS] FPGA forward NTT matches CPU in BIT-REVERSED order "
                     "(expected for CG-NTT)." << std::endl;
        return true;
    }

    std::cerr << "  [FAIL] FPGA forward NTT does NOT match CPU in natural OR bit-rev order."
              << std::endl;
    std::cerr << "    first 8 FPGA : ";
    for (size_t i = 0; i < 8; ++i) std::cerr << fpga_out[i] << " ";
    std::cerr << "\n    first 8 CPU  : ";
    for (size_t i = 0; i < 8; ++i) std::cerr << ref_natural[i] << " ";
    std::cerr << std::endl;
    return false;
}

// =============================================================================
// 测试 3：确定性 —— 同一输入多次 NTT 结果必须一致
// =============================================================================
static bool TestDeterminism(uint64_t modulus, size_t N) {
    PrintSection("TEST 3: Determinism (same input → same output)");
    std::vector<uint64_t> poly_in(N);
    std::mt19937_64 rng(0x12345678ULL);
    for (size_t i = 0; i < N; ++i) poly_in[i] = rng() % modulus;

    std::vector<uint64_t> out1(N, 0), out2(N, 0);
    FpgaManager::GetInstance().NttForwardOffload(poly_in.data(), out1.data(), modulus, N);
    FpgaManager::GetInstance().NttForwardOffload(poly_in.data(), out2.data(), modulus, N);
    return CheckEqual("NTT determinism", out2, out1);
}

// =============================================================================
// 测试 4：多 limb 正确性 —— 每个 limb 单独调用，结果不得互相污染
// =============================================================================
static bool TestMultiLimb(const std::vector<uint64_t>& mods, size_t N) {
    PrintSection("TEST 4: Multi-limb round-trip (每个 limb 独立验证)");
    if (mods.size() < 2) {
        std::cout << "  [SKIP] need >= 2 moduli (have " << mods.size() << ")" << std::endl;
        return true;
    }
    bool ok = true;
    std::mt19937_64 rng(0xDEADBEEFULL);
    for (size_t li = 0; li < mods.size(); ++li) {
        uint64_t m = mods[li];
        std::vector<uint64_t> in(N), mid(N, 0), out(N, 0);
        for (size_t i = 0; i < N; ++i) in[i] = rng() % m;
        FpgaManager::GetInstance().NttForwardOffload(in.data(), mid.data(), m, N);
        FpgaManager::GetInstance().NttInverseOffload(mid.data(), out.data(), m, N);
        ok &= CheckEqual("limb[" + std::to_string(li) + "] round-trip (mod=" +
                         std::to_string(m) + ")", out, in, /*printLen=*/0);
    }
    return ok;
}

// =============================================================================
// 测试 5：跨 limb 独立性 —— 同一输入在不同 modulus 下应得到不同输出
// （若两个模数下输出相同，几乎肯定说明 FPGA 内部 modulus/twiddle 串线了）
// =============================================================================
static bool TestLimbIndependence(const std::vector<uint64_t>& mods, size_t N) {
    PrintSection("TEST 5: Cross-limb independence (different modulus → different output)");
    if (mods.size() < 2) {
        std::cout << "  [SKIP] need >= 2 moduli" << std::endl;
        return true;
    }
    // 构造一个值足够小的输入以确保可以安全地对所有模数取模
    std::vector<uint64_t> in(N);
    for (size_t i = 0; i < N; ++i) in[i] = (i * 17 + 3) % 1024;

    std::vector<uint64_t> out0(N, 0), out1(N, 0);
    FpgaManager::GetInstance().NttForwardOffload(in.data(), out0.data(), mods[0], N);
    FpgaManager::GetInstance().NttForwardOffload(in.data(), out1.data(), mods[1], N);

    if (out0 == out1) {
        std::cerr << "  [FAIL] NTT outputs for mods[0]=" << mods[0]
                  << " and mods[1]=" << mods[1]
                  << " are byte-identical — suggests modulus routing is wrong." << std::endl;
        return false;
    }
    std::cout << "  [PASS] NTT outputs differ between mods[0] and mods[1]." << std::endl;
    return true;
}

// =============================================================================
// main
// =============================================================================
int main() {
    std::cout << "\n################################################" << std::endl;
    std::cout << "       CG-NTT Unit Tests                        " << std::endl;
    std::cout << "################################################" << std::endl;

    // Step 1: 构造 CryptoContext（ring=4096，Q 多 limb 以覆盖 multi-limb 测试）
    uint32_t multDepth    = 1;          // 更多 Q limb
    uint32_t scaleModSize = 50;
    uint32_t ringDegree   = 1 << 12;    // 必须 == FPGA_RING_DIM
    uint32_t batchSize    = 8;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDegree);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    // Step 2: FPGA 初始化（下发 CG-NTT 旋转因子表）
    std::vector<uint64_t> q_mods, q_roots, p_mods, p_roots;
    bool fpga_ok = InitFpga(cc, q_mods, q_roots, p_mods, p_roots);
    if (!fpga_ok) {
        std::cerr << "[FATAL] FPGA not ready — CG-NTT tests require hardware." << std::endl;
        return 2;
    }

    std::vector<uint64_t> all_mods  = q_mods;  all_mods.insert(all_mods.end(), p_mods.begin(), p_mods.end());
    std::vector<uint64_t> all_roots = q_roots; all_roots.insert(all_roots.end(), p_roots.begin(), p_roots.end());

    const size_t N = FPGA_RING_DIM;
    uint64_t test_mod  = all_mods.empty()  ? 0 : all_mods[0];
    uint64_t test_psi  = all_roots.empty() ? 0 : all_roots[0];

    int total = 0, passed = 0;
    auto run = [&](const std::string& name, bool result) {
        ++total;
        if (result) ++passed;
        std::cout << "\n>>> " << name << ": " << (result ? "PASSED" : "FAILED") << std::endl;
    };

    // Step 3: 跑测试
    run("1. Round-trip suite",          TestRoundTripSuite(test_mod, N));
    run("2. Forward NTT vs CPU",        TestForwardVsCpu(test_mod, test_psi, N));
    run("3. Determinism",               TestDeterminism(test_mod, N));
    run("4. Multi-limb round-trip",     TestMultiLimb(all_mods, N));
    run("5. Cross-limb independence",   TestLimbIndependence(all_mods, N));

    // Step 4: 汇总
    std::cout << "\n================================================" << std::endl;
    std::cout << "  CG-NTT TEST SUMMARY: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "================================================" << std::endl;
    return (passed == total) ? 0 : 1;
}
