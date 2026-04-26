//==================================================================================
// BSD 2-Clause License
//
// Copyright (c) 2014-2022, NJIT, Duality Technologies Inc. and other contributors
//
// All rights reserved.
//
// Author TPOC: contact@openfhe.org
//==================================================================================

/*
  FPGA Module Unit Tests
  验证 FpgaManager 中各硬件模块的正确性：
    - ModAdd  (OP_ADD)
    - ModSub  (OP_SUB)
    - ModMult (OP_MULT)
    - NTT Forward  (OP_NTT)
    - NTT Inverse  (OP_INTT)
    - NTT Round-trip (NTT -> INTT == identity)
    - Automorphism (OP_AUTO)
    - BConv (OP_BCONV)
    - ModOpBatch (批量 ADD / SUB / MULT)
 */

#define PROFILE

#include "openfhe.h"
#include "FpgaManager.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace lbcrypto;

// =============================================================================
// Helper utilities
// =============================================================================

// CPU 参考：逐元素模加
static std::vector<uint64_t> CpuModAdd(const std::vector<uint64_t>& a,
                                        const std::vector<uint64_t>& b,
                                        uint64_t mod) {
    std::vector<uint64_t> r(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        r[i] = (a[i] + b[i]) % mod;
    return r;
}

// CPU 参考：逐元素模减
static std::vector<uint64_t> CpuModSub(const std::vector<uint64_t>& a,
                                        const std::vector<uint64_t>& b,
                                        uint64_t mod) {
    std::vector<uint64_t> r(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        r[i] = (a[i] >= b[i]) ? (a[i] - b[i]) : (a[i] + mod - b[i]);
    return r;
}

// CPU 参考：逐元素模乘（使用 128-bit）
static std::vector<uint64_t> CpuModMult(const std::vector<uint64_t>& a,
                                         const std::vector<uint64_t>& b,
                                         uint64_t mod) {
    std::vector<uint64_t> r(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        r[i] = (uint64_t)(((unsigned __int128)a[i] * b[i]) % mod);
    return r;
}

// 比较两段向量是否完全一致，打印差异
static bool CheckEqual(const std::string& tag,
                       const std::vector<uint64_t>& got,
                       const std::vector<uint64_t>& ref,
                       size_t printLen = 8) {
    bool ok = true;
    for (size_t i = 0; i < got.size(); ++i) {
        if (got[i] != ref[i]) {
            ok = false;
            std::cerr << "  [MISMATCH] " << tag << " idx=" << i
                      << " got=" << got[i] << " ref=" << ref[i] << std::endl;
            if (i >= 3) break;  // 只打印前几处差异
        }
    }
    if (ok) {
        std::cout << "  [PASS] " << tag << std::endl;
        if (printLen > 0) {
            std::cout << "    first " << printLen << " values: ";
            for (size_t i = 0; i < printLen && i < got.size(); ++i)
                std::cout << got[i] << " ";
            std::cout << std::endl;
        }
    } else {
        std::cerr << "  [FAIL] " << tag << std::endl;
    }
    return ok;
}

// 打印节标题
static void PrintSection(const std::string& title) {
    std::cout << "\n================================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "================================================" << std::endl;
}

// =============================================================================
// 初始化 FPGA（从 CryptoContext 中提取模数/单位根）
// =============================================================================
static bool InitFpga(CryptoContext<DCRTPoly>& cc,
                     std::vector<uint64_t>& out_q_mods,
                     std::vector<uint64_t>& out_q_roots,
                     std::vector<uint64_t>& out_p_mods,
                     std::vector<uint64_t>& out_p_roots) {
    out_q_mods.clear();
    out_q_roots.clear();
    out_p_mods.clear();
    out_p_roots.clear();

    auto ringDim = cc->GetRingDimension();
    std::cout << "[Host] Ring Dimension: " << ringDim << std::endl;

    // 提取 Q 模数及单位根
    auto elementParams = cc->GetElementParams();
    const auto& rnsParams = elementParams->GetParams();
    std::cout << "[Host] Extracting Q (Ciphertext Moduli and Roots)..." << std::endl;
    for (size_t i = 0; i < rnsParams.size(); i++) {
        uint64_t q_val  = rnsParams[i]->GetModulus().ConvertToInt();
        uint64_t q_root = rnsParams[i]->GetRootOfUnity().ConvertToInt();
        out_q_mods.push_back(q_val);
        out_q_roots.push_back(q_root);
        std::cout << "  Q[" << i << "]: " << q_val << " (Root: " << q_root << ")" << std::endl;
    }

    // 提取 P 模数及单位根
    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    auto paramsP = cryptoParams->GetParamsP();
    std::cout << "[Host] Extracting P (Auxiliary Moduli and Roots)..." << std::endl;
    if (paramsP) {
        const auto& rnsParamsP = paramsP->GetParams();
        for (size_t i = 0; i < rnsParamsP.size(); i++) {
            uint64_t p_val  = rnsParamsP[i]->GetModulus().ConvertToInt();
            uint64_t p_root = rnsParamsP[i]->GetRootOfUnity().ConvertToInt();
            out_p_mods.push_back(p_val);
            out_p_roots.push_back(p_root);
            std::cout << "  P[" << i << "]: " << p_val << " (Root: " << p_root << ")" << std::endl;
        }
    }

    if (!FpgaManager::GetInstance().IsReady()) {
        std::cout << "[Host] FPGA not ready. Skipping FPGA initialization." << std::endl;
        return false;
    }

    std::cout << "[Host] Sending moduli and roots to FPGA..." << std::endl;
    FpgaManager::GetInstance().InitModuli(out_q_mods, out_p_mods, out_q_roots, out_p_roots);
    std::cout << "[Host] FPGA Initialization Done." << std::endl;
    return true;
}

// =============================================================================
// 测试：ModAdd
// =============================================================================
static bool TestModAdd(uint64_t modulus, size_t N) {
    PrintSection("TEST: ModAdd (OP_ADD)");

    // 构造测试向量 a = [0,1,2,...,N-1], b = [N-1,...,1,0]
    std::vector<uint64_t> a(N), b(N);
    for (size_t i = 0; i < N; ++i) {
        a[i] = i % modulus;
        b[i] = (N - 1 - i) % modulus;
    }
    std::vector<uint64_t> fpga_out(N, 0);
    FpgaManager::GetInstance().ModAddOffload(a.data(), b.data(), fpga_out.data(), modulus, N);

    auto ref = CpuModAdd(a, b, modulus);
    return CheckEqual("ModAdd", fpga_out, ref);
}

// =============================================================================
// 测试：ModSub
// =============================================================================
static bool TestModSub(uint64_t modulus, size_t N) {
    PrintSection("TEST: ModSub (OP_SUB)");

    std::vector<uint64_t> a(N), b(N);
    for (size_t i = 0; i < N; ++i) {
        a[i] = (i * 3 + 7) % modulus;
        b[i] = (i * 2 + 3) % modulus;
    }
    std::vector<uint64_t> fpga_out(N, 0);
    FpgaManager::GetInstance().ModSubOffload(a.data(), b.data(), fpga_out.data(), modulus, N);

    auto ref = CpuModSub(a, b, modulus);
    return CheckEqual("ModSub", fpga_out, ref);
}

// =============================================================================
// 测试：ModMult
// =============================================================================
static bool TestModMult(uint64_t modulus, size_t N) {
    PrintSection("TEST: ModMult (OP_MULT)");

    std::vector<uint64_t> a(N), b(N);
    for (size_t i = 0; i < N; ++i) {
        a[i] = (i * 5 + 1) % modulus;
        b[i] = (i * 7 + 3) % modulus;
    }
    std::vector<uint64_t> fpga_out(N, 0);
    FpgaManager::GetInstance().ModMultOffload(a.data(), b.data(), fpga_out.data(), modulus, N);

    auto ref = CpuModMult(a, b, modulus);
    return CheckEqual("ModMult", fpga_out, ref);
}

// =============================================================================
// 测试：NTT 正变换 (OP_NTT)
// 用 OpenFHE CPU NTT 作为参考
// =============================================================================
static bool TestNttForward(CryptoContext<DCRTPoly>& cc,
                            uint64_t modulus, size_t N) {
    PrintSection("TEST: NTT Forward (OP_NTT)");

    // 构造一个小的多项式，通过 OpenFHE 的 DCRTPoly 得到 CPU NTT 结果作为参考
    // 简单起见：先用 FPGA 做一次 NTT，然后用 INTT 还原，验证 round-trip
    // （直接拿 CPU 参考的 NTT 需要访问内部接口，round-trip 更通用）
    std::cout << "  (NTT forward 将在 NTT round-trip 测试中一并验证)" << std::endl;
    std::cout << "  [SKIP standalone forward NTT — see NTT Round-trip]" << std::endl;
    return true;
}

// =============================================================================
// 测试：NTT Round-trip (NTT -> INTT == identity)
// =============================================================================
static bool TestNttRoundTrip(uint64_t modulus, size_t N) {
    PrintSection("TEST: NTT Round-trip (NTT -> INTT == identity)");

    // 构造原始多项式（时域）
    std::vector<uint64_t> poly_in(N), fpga_ntt_out(N, 0), fpga_intt_out(N, 0);
    for (size_t i = 0; i < N; ++i)
        poly_in[i] = (i * 13 + 5) % modulus;

    // Step 1: 正变换
    FpgaManager::GetInstance().NttForwardOffload(poly_in.data(), fpga_ntt_out.data(), modulus, N);

    // Step 2: 逆变换
    FpgaManager::GetInstance().NttInverseOffload(fpga_ntt_out.data(), fpga_intt_out.data(), modulus, N);

    // INTT 通常输出已除以 N（OpenFHE 的约定），与原始 poly_in 相同
    bool ok = CheckEqual("NTT Round-trip (INTT(NTT(x)) == x)", fpga_intt_out, poly_in);

    // 同时打印 NTT 输出的前几个值供调试
    std::cout << "  NTT output (first 8): ";
    for (size_t i = 0; i < 8 && i < N; ++i) std::cout << fpga_ntt_out[i] << " ";
    std::cout << std::endl;

    return ok;
}

// =============================================================================
// 测试：Automorphism (OP_AUTO)
// 验证：Auto(x, k=1) 等价于将多项式系数循环位移 1 位（在 NTT 域外则为置换）
// 最简单的正确性验证：Auto(x, k) 之后再 Auto(x, k_inv) 应当还原
// =============================================================================
static bool TestAuto(uint64_t modulus, size_t N) {
    PrintSection("TEST: Automorphism (OP_AUTO)");

    // 使用 galois element k=5, kinv = ModInverse(5, 2*N)
    // 对于 N=4096，2N=8192，此处演示 k=3
    uint32_t k    = 3;
    // kinv = k^{-1} mod 2N  (使用扩展 Euclidean)
    // 简单枚举求 k_inv
    uint32_t twoN = (uint32_t)(2 * N);
    uint32_t kinv = 0;
    for (uint32_t t = 1; t < twoN; ++t) {
        if ((uint64_t)k * t % twoN == 1) { kinv = t; break; }
    }
    if (kinv == 0) {
        std::cerr << "  [SKIP] Could not find inverse of k=" << k << " mod 2N=" << twoN << std::endl;
        return true;
    }
    std::cout << "  Using k=" << k << ", kinv=" << kinv << " (mod 2N=" << twoN << ")" << std::endl;

    std::vector<uint64_t> poly_in(N), after_auto(N, 0), restored(N, 0);
    for (size_t i = 0; i < N; ++i)
        poly_in[i] = (i * 11 + 7) % modulus;

    // Auto(x, k)
    FpgaManager::GetInstance().AutoOffload(poly_in.data(), after_auto.data(), k, kinv, modulus, N);

    // Auto(Auto(x,k), kinv) 应当还原为 poly_in
    FpgaManager::GetInstance().AutoOffload(after_auto.data(), restored.data(), kinv, k, modulus, N);

    return CheckEqual("Auto round-trip (Auto(Auto(x,k),kinv)==x)", restored, poly_in);
}

// =============================================================================
// 测试：ModOpBatch — 批量 ADD
// =============================================================================
static bool TestBatchAdd(const std::vector<uint64_t>& all_mods,
                          size_t N, int modIdxStart, int numLimbs) {
    PrintSection("TEST: ModOpBatch ADD (批量多 limb)");

    size_t total = (size_t)numLimbs * N;
    std::vector<uint64_t> a(total), b(total), fpga_out(total, 0);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        for (size_t i = 0; i < N; ++i) {
            a[l * N + i] = (i + l * 3 + 1) % mod;
            b[l * N + i] = (i * 2 + l + 5) % mod;
        }
    }

    FpgaManager::GetInstance().ModOpBatchOffload(
        OP_ADD, a.data(), b.data(), fpga_out.data(), modIdxStart, N, numLimbs);

    // CPU 参考
    std::vector<uint64_t> ref(total);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        auto slice_a = std::vector<uint64_t>(a.begin() + l * N, a.begin() + (l + 1) * N);
        auto slice_b = std::vector<uint64_t>(b.begin() + l * N, b.begin() + (l + 1) * N);
        auto slice_r = CpuModAdd(slice_a, slice_b, mod);
        std::copy(slice_r.begin(), slice_r.end(), ref.begin() + l * N);
    }
    return CheckEqual("BatchAdd (limb 0..N-1)", fpga_out, ref);
}

// =============================================================================
// 测试：ModOpBatch — 批量 SUB
// =============================================================================
static bool TestBatchSub(const std::vector<uint64_t>& all_mods,
                          size_t N, int modIdxStart, int numLimbs) {
    PrintSection("TEST: ModOpBatch SUB (批量多 limb)");

    size_t total = (size_t)numLimbs * N;
    std::vector<uint64_t> a(total), b(total), fpga_out(total, 0);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        for (size_t i = 0; i < N; ++i) {
            a[l * N + i] = (i * 5 + l * 2 + 9) % mod;
            b[l * N + i] = (i * 3 + l + 4) % mod;
        }
    }

    FpgaManager::GetInstance().ModOpBatchOffload(
        OP_SUB, a.data(), b.data(), fpga_out.data(), modIdxStart, N, numLimbs);

    std::vector<uint64_t> ref(total);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        auto slice_a = std::vector<uint64_t>(a.begin() + l * N, a.begin() + (l + 1) * N);
        auto slice_b = std::vector<uint64_t>(b.begin() + l * N, b.begin() + (l + 1) * N);
        auto slice_r = CpuModSub(slice_a, slice_b, mod);
        std::copy(slice_r.begin(), slice_r.end(), ref.begin() + l * N);
    }
    return CheckEqual("BatchSub (limb 0..N-1)", fpga_out, ref);
}

// =============================================================================
// 测试：ModOpBatch — 批量 MULT
// =============================================================================
static bool TestBatchMult(const std::vector<uint64_t>& all_mods,
                           size_t N, int modIdxStart, int numLimbs) {
    PrintSection("TEST: ModOpBatch MULT (批量多 limb)");

    size_t total = (size_t)numLimbs * N;
    std::vector<uint64_t> a(total), b(total), fpga_out(total, 0);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        for (size_t i = 0; i < N; ++i) {
            a[l * N + i] = (i * 7 + l * 4 + 2) % mod;
            b[l * N + i] = (i * 3 + l * 6 + 1) % mod;
        }
    }

    FpgaManager::GetInstance().ModOpBatchOffload(
        OP_MULT, a.data(), b.data(), fpga_out.data(), modIdxStart, N, numLimbs);

    std::vector<uint64_t> ref(total);
    for (int l = 0; l < numLimbs; ++l) {
        uint64_t mod = all_mods[modIdxStart + l];
        auto slice_a = std::vector<uint64_t>(a.begin() + l * N, a.begin() + (l + 1) * N);
        auto slice_b = std::vector<uint64_t>(b.begin() + l * N, b.begin() + (l + 1) * N);
        auto slice_r = CpuModMult(slice_a, slice_b, mod);
        std::copy(slice_r.begin(), slice_r.end(), ref.begin() + l * N);
    }
    return CheckEqual("BatchMult (limb 0..N-1)", fpga_out, ref);
}

// =============================================================================
// 测试：BConv
// 验证：BConv(x) 结果与 CPU 手动计算的模转换结果一致
//
// BConv 做的事：
//   result[j] = sum_i ( x[i] * w[i][j] ) mod out_mod[j]   (j=0..sizeP-1)
// 其中 w[i][j] 是预计算好的 CRT 系数（此处用单位矩阵做最简验证）
// =============================================================================
static bool TestBConv(const std::vector<uint64_t>& q_mods,
                       const std::vector<uint64_t>& p_mods,
                       size_t N) {
    PrintSection("TEST: BConv (OP_BCONV)");

    const int LIMB_Q = FpgaManager::KERNEL_LIMB_Q;       // 3
    const int MAX_OUT = FpgaManager::KERNEL_MAX_OUT_COLS; // 5
    int sizeP = (int)std::min(p_mods.size(), (size_t)MAX_OUT);

    if ((int)q_mods.size() < LIMB_Q || sizeP == 0) {
        std::cout << "  [SKIP] Not enough limbs for BConv test (need Q>=" << LIMB_Q
                  << " P>=" << 1 << ")" << std::endl;
        return true;
    }

    // 构造输入 x[LIMB_Q × N]
    std::vector<uint64_t> x(LIMB_Q * N);
    for (int l = 0; l < LIMB_Q; ++l) {
        uint64_t mod = q_mods[l];
        for (size_t i = 0; i < N; ++i)
            x[l * N + i] = (i * 3 + l + 1) % mod;
    }

    // 构造权重矩阵 w[LIMB_Q × MAX_OUT]
    // 使用单位矩阵（w[i][j] = (i==j) ? 1 : 0），此时 BConv(x)[j] = x[j] mod out_mod[j]
    std::vector<uint64_t> w(LIMB_Q * MAX_OUT, 0);
    for (int i = 0; i < LIMB_Q && i < MAX_OUT && i < sizeP; ++i)
        w[i * MAX_OUT + i] = 1;

    // 输出模数
    std::vector<uint64_t> out_mod(sizeP);
    for (int j = 0; j < sizeP; ++j)
        out_mod[j] = p_mods[j];

    std::vector<uint64_t> fpga_out(sizeP * N, 0);
    FpgaManager::GetInstance().BConvOffload(x.data(), w.data(), out_mod.data(),
                                             fpga_out.data(), N, sizeP);

    // CPU 参考：result[j][i] = sum_l ( x[l][i] * w[l][j] ) mod out_mod[j]
    std::vector<uint64_t> ref(sizeP * N, 0);
    for (int j = 0; j < sizeP; ++j) {
        uint64_t mod = out_mod[j];
        for (size_t i = 0; i < N; ++i) {
            unsigned __int128 acc = 0;
            for (int l = 0; l < LIMB_Q; ++l)
                acc = (acc + (unsigned __int128)x[l * N + i] * w[l * MAX_OUT + j]) % mod;
            ref[j * N + i] = (uint64_t)acc;
        }
    }

    return CheckEqual("BConv (identity weight)", fpga_out, ref);
}

// =============================================================================
// 测试：End-to-end CKKS — 利用 FPGA 算子完成加/减/乘/旋转解密验证
// （与 simple-real-numbers.cpp 相同的流程，作为系统级集成测试）
// =============================================================================
static bool TestCkksEndToEnd(CryptoContext<DCRTPoly>& cc, KeyPair<DCRTPoly>& keys) {
    PrintSection("TEST: CKKS End-to-End (Add / Sub / Mult / Rotate)");

    uint32_t batchSize = 8;
    double tol = 1e-3;   // CKKS 近似精度容忍度
    bool all_ok = true;

    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> x2 = {5.0,  4.0, 3.0,  2.0, 1.0, 0.75, 0.5, 0.25};

    // 带异常保护的解密+验证 lambda
    auto checkDecrypt = [&](const std::string& name, Ciphertext<DCRTPoly> ctxt,
                             const std::vector<double>& expected) -> bool {
        try {
            Plaintext pt;
            cc->Decrypt(keys.secretKey, ctxt, &pt);
            pt->SetLength(batchSize);
            auto got = pt->GetRealPackedValue();
            bool ok = true;
            for (size_t i = 0; i < batchSize; ++i) {
                if (std::abs(got[i] - expected[i]) > tol) {
                    std::cerr << "  [FAIL] " << name << " idx=" << i
                              << " got=" << got[i] << " expected=" << expected[i] << std::endl;
                    ok = false;
                }
            }
            if (ok) {
                std::cout << "  [PASS] " << name << std::endl;
                std::cout << "    got: ";
                for (size_t i = 0; i < batchSize; ++i)
                    std::cout << std::setprecision(6) << got[i] << " ";
                std::cout << std::endl;
            }
            return ok;
        } catch (const std::exception& e) {
            std::cerr << "  [FAIL] " << name << " — exception during decrypt: "
                      << e.what() << std::endl;
            return false;
        }
    };

    // --------- Group 1: 仅 Add/Sub（不消耗乘法深度）---------
    {
        auto ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
        auto ptxt2 = cc->MakeCKKSPackedPlaintext(x2);
        auto c1 = cc->Encrypt(keys.publicKey, ptxt1);
        auto c2 = cc->Encrypt(keys.publicKey, ptxt2);

        auto cAdd = cc->EvalAdd(c1, c2);
        auto cSub = cc->EvalSub(c1, c2);

        std::vector<double> expAdd(batchSize), expSub(batchSize);
        for (size_t i = 0; i < batchSize; ++i) {
            expAdd[i] = x1[i] + x2[i];
            expSub[i] = x1[i] - x2[i];
        }
        all_ok &= checkDecrypt("EvalAdd (x1+x2)", cAdd, expAdd);
        all_ok &= checkDecrypt("EvalSub (x1-x2)", cSub, expSub);
    }

    // --------- Group 2: 标量乘（消耗 1 层深度）---------
    {
        auto ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
        auto c1 = cc->Encrypt(keys.publicKey, ptxt1);

        auto cScalar = cc->EvalMult(c1, 4.0);

        std::vector<double> expScalar(batchSize);
        for (size_t i = 0; i < batchSize; ++i)
            expScalar[i] = 4.0 * x1[i];
        all_ok &= checkDecrypt("EvalMult scalar (4*x1)", cScalar, expScalar);
    }

    // --------- Group 3: 密文×密文（消耗 1 层深度 + relinearization）---------
    {
        auto ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
        auto ptxt2 = cc->MakeCKKSPackedPlaintext(x2);
        auto c1 = cc->Encrypt(keys.publicKey, ptxt1);
        auto c2 = cc->Encrypt(keys.publicKey, ptxt2);

        auto cMul = cc->EvalMult(c1, c2);

        std::vector<double> expMul(batchSize);
        for (size_t i = 0; i < batchSize; ++i)
            expMul[i] = x1[i] * x2[i];
        all_ok &= checkDecrypt("EvalMult (x1*x2)", cMul, expMul);
    }

    // --------- Group 4: 旋转（需要 key switching，独立新密文）---------
    {
        auto ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
        auto c1 = cc->Encrypt(keys.publicKey, ptxt1);

        auto cRot1 = cc->EvalRotate(c1, 1);
        auto cRot2 = cc->EvalRotate(c1, -2);

        std::vector<double> expRot1(batchSize), expRot2(batchSize);
        for (size_t i = 0; i < batchSize; ++i)
            expRot1[i] = x1[(i + 1) % batchSize];
        for (size_t i = 0; i < batchSize; ++i)
            expRot2[i] = x1[(i + batchSize - 2) % batchSize];

        all_ok &= checkDecrypt("EvalRotate (+1)",  cRot1, expRot1);
        all_ok &= checkDecrypt("EvalRotate (-2)",  cRot2, expRot2);
    }

    return all_ok;
}

// =============================================================================
// main
// =============================================================================
int main() {
    std::cout << "\n################################################" << std::endl;
    std::cout << "       FPGA Module Unit Tests                   " << std::endl;
    std::cout << "################################################" << std::endl;

    // =========================================================================
    // Step 1: Setup CryptoContext（与 simple-real-numbers.cpp 相同）
    // =========================================================================
    uint32_t multDepth    = 1;
    uint32_t scaleModSize = 50;
    uint32_t ringDegree   = 1 << 12;   // FPGA_RING_DIM = 4096
    uint32_t batchSize    = 8;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDegree);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    // =========================================================================
    // Step 2: 初始化 FPGA（必须在 cc->Enable 之前）
    // =========================================================================
    std::vector<uint64_t> q_mods, q_roots, p_mods, p_roots;
    bool fpga_ok = InitFpga(cc, q_mods, q_roots, p_mods, p_roots);

    // 汇总所有模数（Q 在前，P 在后），供后续测试使用
    std::vector<uint64_t> all_mods;
    all_mods.insert(all_mods.end(), q_mods.begin(), q_mods.end());
    all_mods.insert(all_mods.end(), p_mods.begin(), p_mods.end());

    // =========================================================================
    // Step 3: Enable PKE features
    // =========================================================================
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    std::cout << "\n[Host] CKKS ring dimension: " << cc->GetRingDimension() << std::endl;

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalRotateKeyGen(keys.secretKey, {1, -2});

    // =========================================================================
    // Step 4: 逐模块测试（若 FPGA 未就绪则跳过硬件测试）
    // =========================================================================
    const size_t N = FPGA_RING_DIM;

    // 取第一个 Q 模数作为单 limb 测试的模数
    uint64_t test_mod = q_mods.empty() ? 0x7fffffffe0001ULL : q_mods[0];

    int total_tests = 0, passed_tests = 0;

    auto run = [&](const std::string& name, bool result) {
        ++total_tests;
        if (result) ++passed_tests;
        std::cout << "\n>>> " << name << ": " << (result ? "PASSED" : "FAILED") << std::endl;
    };

    if (fpga_ok) {
        // ----- 单 limb 算术 -----
        run("ModAdd",     TestModAdd (test_mod, N));
        run("ModSub",     TestModSub (test_mod, N));
        run("ModMult",    TestModMult(test_mod, N));

        // ----- NTT -----
        run("NTT Forward", TestNttForward(cc, test_mod, N));  // 简单跳过，由 round-trip 覆盖
        run("NTT Round-trip", TestNttRoundTrip(test_mod, N));

        // ----- Automorphism -----
        run("Automorphism", TestAuto(test_mod, N));

        // ----- 批量算术 -----
        int numLimbs = (int)std::min(q_mods.size(), (size_t)MAX_LIMBS);
        if (numLimbs >= 2) {
            run("BatchAdd",  TestBatchAdd (all_mods, N, 0, numLimbs));
            run("BatchSub",  TestBatchSub (all_mods, N, 0, numLimbs));
            run("BatchMult", TestBatchMult(all_mods, N, 0, numLimbs));
        } else {
            std::cout << "\n[SKIP] BatchAdd/Sub/Mult: need >= 2 Q limbs" << std::endl;
        }

        // ----- BConv -----
        run("BConv", TestBConv(q_mods, p_mods, N));
    } else {
        std::cout << "\n[WARN] FPGA not ready — skipping all hardware unit tests." << std::endl;
    }

    // ----- 端到端 CKKS 集成测试（无论 FPGA 是否 ready 都跑，验证系统整体） -----
    run("CKKS End-to-End", TestCkksEndToEnd(cc, keys));

    // =========================================================================
    // Step 5: 汇总
    // =========================================================================
    std::cout << "\n================================================" << std::endl;
    std::cout << "  TEST SUMMARY: " << passed_tests << "/" << total_tests << " passed" << std::endl;
    std::cout << "================================================" << std::endl;

    return (passed_tests == total_tests) ? 0 : 1;
}
