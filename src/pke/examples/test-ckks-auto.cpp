// ============================================================================
// test-ckks-auto.cpp
//
// 针对 CKKS EvalRotate 中 Automorphism 路径的分步诊断测试。
// 将 INTT → Auto → NTT 三个阶段逐一与 CPU 参考对比，精确定位出错位置。
//
// 测试层次：
//   1. TestAutoInttStep     — FPGA INTT 与 CPU SwitchFormat(EVAL→COEF) 对比
//   2. TestAutoPermStep     — FPGA Auto 置换与 CPU 置换公式对比（COEF 域）
//   3. TestAutoNttStep      — FPGA NTT 与 CPU SwitchFormat(COEF→EVAL) 对比
//   4. TestAutoFullPipeline — FPGA 三步流水线与 CPU AutomorphismTransform 对比
//   5. TestAutoEvalRotate   — 完整 CKKS EvalRotate 端到端解密验证
// ============================================================================

#define PROFILE

#include "openfhe.h"
#include "math/nbtheory.h"
#include "FpgaManager.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace lbcrypto;

// ============================================================================
// Helpers
// ============================================================================

static void PrintSection(const std::string& title) {
    std::cout << "\n================================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "================================================" << std::endl;
}

// 逐元素比较，打印前几处差异，返回是否全部匹配
static bool CheckEqual(const std::string& tag,
                       const std::vector<uint64_t>& got,
                       const std::vector<uint64_t>& ref,
                       size_t printLen = 8) {
    size_t mismatches = 0;
    for (size_t i = 0; i < got.size(); ++i) {
        if (got[i] != ref[i]) {
            if (mismatches < 5)
                std::cerr << "  [MISMATCH] " << tag << " idx=" << i
                          << " got=" << got[i] << " ref=" << ref[i] << std::endl;
            ++mismatches;
        }
    }
    if (mismatches == 0) {
        std::cout << "  [PASS] " << tag << std::endl;
        std::cout << "    first " << printLen << " values: ";
        for (size_t i = 0; i < printLen && i < got.size(); ++i)
            std::cout << got[i] << " ";
        std::cout << std::endl;
        return true;
    }
    std::cerr << "  [FAIL] " << tag << " — " << mismatches << " mismatches (total "
              << got.size() << ")" << std::endl;
    return false;
}

// CPU 参考：Auto 置换（COEF 域，自然序）
//   out[jk & mask] = ((jk >> logn) & 1) ? q - in[j] : in[j]
static std::vector<uint64_t> CpuAutoPerm(const std::vector<uint64_t>& in,
                                          uint32_t k, uint64_t q) {
    uint32_t n    = (uint32_t)in.size();
    uint32_t logn = 0;
    for (uint32_t tmp = n; tmp > 1; tmp >>= 1) ++logn;
    uint32_t mask = (1u << logn) - 1u;

    std::vector<uint64_t> out(n, 0);
    for (uint32_t j = 0, jk = 0; j < n; ++j, jk += k)
        out[jk & mask] = ((jk >> logn) & 1u) ? q - in[j] : in[j];
    return out;
}

// 从 CryptoContext 提取第 limbIdx 个 Q 模数
static uint64_t GetQMod(CryptoContext<DCRTPoly>& cc, size_t limbIdx = 0) {
    const auto& rnsParams = cc->GetElementParams()->GetParams();
    return rnsParams[limbIdx]->GetModulus().ConvertToInt();
}

// 从密文第 ctElem 个多项式的第 limbIdx 个 RNS limb 提取 EVAL 域系数
static std::vector<uint64_t> ExtractLimb(const Ciphertext<DCRTPoly>& ct,
                                          int ctElem, int limbIdx) {
    const auto& poly = ct->GetElements()[ctElem].GetAllElements()[limbIdx];
    uint32_t n = poly.GetLength();
    std::vector<uint64_t> buf(n);
    for (uint32_t i = 0; i < n; ++i)
        buf[i] = poly.GetValues()[i].ConvertToInt();
    return buf;
}

// ============================================================================
// 测试 1：FPGA INTT 与 CPU INTT 对比
//   输入：从密文 c0 limb-0 取 EVAL 域数据
//   CPU 参考：DCRTPoly SwitchFormat (EVAL→COEF)
// ============================================================================
static bool TestAutoInttStep(CryptoContext<DCRTPoly>& cc,
                              const Ciphertext<DCRTPoly>& ct) {
    PrintSection("TEST 1: INTT Step (FPGA vs CPU SwitchFormat)");

    uint64_t q = GetQMod(cc, 0);
    size_t   N = cc->GetRingDimension();

    auto evalBuf = ExtractLimb(ct, 0, 0);

    // FPGA INTT
    std::vector<uint64_t> fpgaCoef(N, 0);
    FpgaManager::GetInstance().NttInverseOffload(evalBuf.data(), fpgaCoef.data(), q, N);

    // CPU 参考：SwitchFormat (EVAL→COEF)
    auto poly0 = ct->GetElements()[0].GetAllElements()[0];  // copy
    poly0.SwitchFormat();
    std::vector<uint64_t> cpuCoef(N);
    for (size_t i = 0; i < N; ++i)
        cpuCoef[i] = poly0.GetValues()[i].ConvertToInt();

    std::cout << "  q=" << q << ", N=" << N << std::endl;
    std::cout << "  FPGA INTT (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << fpgaCoef[i] << " ";
    std::cout << std::endl;
    std::cout << "  CPU  INTT (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << cpuCoef[i] << " ";
    std::cout << std::endl;

    return CheckEqual("INTT (FPGA vs CPU)", fpgaCoef, cpuCoef);
}

// ============================================================================
// 测试 2：FPGA Auto 置换与 CPU 置换公式对比（COEF 域）
//   先用 CPU INTT 得到 COEF 域数据，再分别用 FPGA/CPU 做置换
// ============================================================================
static bool TestAutoPermStep(CryptoContext<DCRTPoly>& cc,
                              const Ciphertext<DCRTPoly>& ct,
                              uint32_t k) {
    PrintSection("TEST 2: Auto Permutation Step (FPGA vs CPU, COEF domain)");

    uint64_t q = GetQMod(cc, 0);
    size_t   N = cc->GetRingDimension();

    // CPU INTT → COEF 域（避免 INTT 误差干扰本测试）
    auto poly0 = ct->GetElements()[0].GetAllElements()[0];
    poly0.SwitchFormat();
    std::vector<uint64_t> coefBuf(N);
    for (size_t i = 0; i < N; ++i)
        coefBuf[i] = poly0.GetValues()[i].ConvertToInt();

    // kinv = k^{-1} mod 2N
    uint32_t twoN = (uint32_t)(2 * N);
    uint32_t kinv = 0;
    for (uint32_t t = 1; t < twoN; ++t) {
        if ((uint64_t)k * t % twoN == 1) { kinv = t; break; }
    }
    if (kinv == 0) {
        std::cerr << "  [SKIP] k=" << k << " has no inverse mod 2N=" << twoN << std::endl;
        return true;
    }
    std::cout << "  k=" << k << ", kinv=" << kinv << ", q=" << q << ", N=" << N << std::endl;

    // FPGA Auto
    std::vector<uint64_t> fpgaOut(N, 0);
    FpgaManager::GetInstance().AutoOffload(coefBuf.data(), fpgaOut.data(), k, kinv, q, N);

    // CPU 参考（置换公式）
    auto cpuOut = CpuAutoPerm(coefBuf, k, q);

    std::cout << "  FPGA Auto (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << fpgaOut[i] << " ";
    std::cout << std::endl;
    std::cout << "  CPU  Auto (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << cpuOut[i] << " ";
    std::cout << std::endl;

    bool ok = CheckEqual("Auto Perm (FPGA vs CPU formula)", fpgaOut, cpuOut);

    // round-trip: Auto(Auto(x,k), kinv) == x
    std::vector<uint64_t> restored(N, 0);
    FpgaManager::GetInstance().AutoOffload(fpgaOut.data(), restored.data(), kinv, k, q, N);
    ok &= CheckEqual("Auto round-trip (FPGA: Auto(Auto(x,k),kinv)==x)", restored, coefBuf);

    return ok;
}

// ============================================================================
// 测试 3：FPGA NTT 与 CPU NTT 对比（给定相同 COEF 域输入）
// ============================================================================
static bool TestAutoNttStep(CryptoContext<DCRTPoly>& cc,
                             const Ciphertext<DCRTPoly>& ct,
                             uint32_t k) {
    PrintSection("TEST 3: NTT Step (FPGA vs CPU SwitchFormat, after CPU Auto)");

    uint64_t q = GetQMod(cc, 0);
    size_t   N = cc->GetRingDimension();

    // CPU INTT → CPU Auto → COEF 域
    auto poly0 = ct->GetElements()[0].GetAllElements()[0];
    poly0.SwitchFormat();
    std::vector<uint64_t> coefBuf(N);
    for (size_t i = 0; i < N; ++i)
        coefBuf[i] = poly0.GetValues()[i].ConvertToInt();
    auto cpuAutoCoef = CpuAutoPerm(coefBuf, k, q);

    // FPGA NTT
    std::vector<uint64_t> fpgaEval(N, 0);
    FpgaManager::GetInstance().NttForwardOffload(cpuAutoCoef.data(), fpgaEval.data(), q, N);

    // CPU 参考：把 cpuAutoCoef 放回 poly，SwitchFormat (COEF→EVAL)
    auto params0 = ct->GetElements()[0].GetAllElements()[0].GetParams();
    using IntType = NativeInteger;
    NativeVector cpuVec(N, NativeInteger(q));
    for (size_t i = 0; i < N; ++i)
        cpuVec[i] = IntType(cpuAutoCoef[i]);
    PolyImpl<NativeVector> refPoly(params0, Format::COEFFICIENT, true);
    refPoly.SetValues(cpuVec, Format::COEFFICIENT);
    refPoly.SwitchFormat();

    std::vector<uint64_t> cpuEval(N);
    for (size_t i = 0; i < N; ++i)
        cpuEval[i] = refPoly.GetValues()[i].ConvertToInt();

    std::cout << "  q=" << q << ", N=" << N << ", k=" << k << std::endl;
    std::cout << "  FPGA NTT (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << fpgaEval[i] << " ";
    std::cout << std::endl;
    std::cout << "  CPU  NTT (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << cpuEval[i] << " ";
    std::cout << std::endl;

    return CheckEqual("NTT (FPGA vs CPU, after Auto)", fpgaEval, cpuEval);
}

// ============================================================================
// 测试 4：FPGA 完整流水线 (INTT→Auto→NTT) 与 CPU AutomorphismTransform 对比
//   对密文 c0 的每个 RNS limb 分别测试
// ============================================================================
static bool TestAutoFullPipeline(CryptoContext<DCRTPoly>& cc,
                                  const Ciphertext<DCRTPoly>& ct,
                                  uint32_t k) {
    PrintSection("TEST 4: Full Auto Pipeline (FPGA INTT+Auto+NTT vs CPU AutomorphismTransform)");

    size_t N = cc->GetRingDimension();
    uint32_t twoN = (uint32_t)(2 * N);
    uint32_t kinv = 0;
    for (uint32_t t = 1; t < twoN; ++t) {
        if ((uint64_t)k * t % twoN == 1) { kinv = t; break; }
    }
    std::cout << "  k=" << k << ", kinv=" << kinv << ", N=" << N << std::endl;

    // 构造 precomp（供 CPU AutomorphismTransform 使用）
    std::vector<uint32_t> precomp(N);
    PrecomputeAutoMap((uint32_t)N, k, &precomp);

    bool all_ok = true;
    const auto& dcrtPoly = ct->GetElements()[0];
    const auto& limbs    = dcrtPoly.GetAllElements();

    for (size_t l = 0; l < limbs.size(); ++l) {
        uint64_t q = limbs[l].GetParams()->GetModulus().ConvertToInt();

        // 取 EVAL 域原始数据
        std::vector<uint64_t> evalBuf(N);
        for (size_t i = 0; i < N; ++i)
            evalBuf[i] = limbs[l].GetValues()[i].ConvertToInt();

        // FPGA 三步流水线
        std::vector<uint64_t> coefBuf(N, 0), autoCoef(N, 0), fpgaEval(N, 0);
        FpgaManager::GetInstance().NttInverseOffload(evalBuf.data(), coefBuf.data(), q, N);
        FpgaManager::GetInstance().AutoOffload(coefBuf.data(), autoCoef.data(), k, kinv, q, N);
        FpgaManager::GetInstance().NttForwardOffload(autoCoef.data(), fpgaEval.data(), q, N);

        // CPU 参考：AutomorphismTransform(k, precomp)
        auto cpuResult = limbs[l].AutomorphismTransform(k, precomp);
        std::vector<uint64_t> cpuEval(N);
        for (size_t i = 0; i < N; ++i)
            cpuEval[i] = cpuResult.GetValues()[i].ConvertToInt();

        std::string tag = "limb[" + std::to_string(l) + "] q=" + std::to_string(q);
        std::cout << "  FPGA pipeline (first 8): ";
        for (size_t i = 0; i < 8; ++i) std::cout << fpgaEval[i] << " ";
        std::cout << std::endl;
        std::cout << "  CPU  result  (first 8): ";
        for (size_t i = 0; i < 8; ++i) std::cout << cpuEval[i] << " ";
        std::cout << std::endl;

        all_ok &= CheckEqual("Full Pipeline " + tag, fpgaEval, cpuEval);
    }
    return all_ok;
}

// ============================================================================
// 测试 5：CKKS EvalRotate 端到端解密验证
// ============================================================================
static bool TestAutoEvalRotate(CryptoContext<DCRTPoly>& cc,
                                KeyPair<DCRTPoly>& keys) {
    PrintSection("TEST 5: CKKS EvalRotate End-to-End");

    const uint32_t batchSize = 8;
    const double   tol       = 1e-3;
    bool all_ok = true;

    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    auto ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
    auto c1    = cc->Encrypt(keys.publicKey, ptxt1);

    for (int rot : {1, -1, 2, -2, 3}) {
        auto cRot = cc->EvalRotate(c1, rot);

        Plaintext pt;
        cc->Decrypt(keys.secretKey, cRot, &pt);
        pt->SetLength(batchSize);
        auto got = pt->GetRealPackedValue();

        std::vector<double> expected(batchSize);
        for (size_t i = 0; i < batchSize; ++i)
            expected[i] = x1[((int)i + rot % (int)batchSize + (int)batchSize) % (int)batchSize];

        bool ok = true;
        for (size_t i = 0; i < batchSize; ++i) {
            if (std::abs(got[i] - expected[i]) > tol) {
                std::cerr << "  [FAIL] EvalRotate(" << rot << ") idx=" << i
                          << " got=" << got[i] << " expected=" << expected[i] << std::endl;
                ok = false;
            }
        }
        if (ok)
            std::cout << "  [PASS] EvalRotate(" << rot << ")" << std::endl;
        else
            std::cerr << "  [FAIL] EvalRotate(" << rot << ")" << std::endl;

        std::cout << "    got:      ";
        for (size_t i = 0; i < batchSize; ++i)
            std::cout << std::setprecision(5) << got[i] << " ";
        std::cout << std::endl;
        std::cout << "    expected: ";
        for (size_t i = 0; i < batchSize; ++i)
            std::cout << std::setprecision(5) << expected[i] << " ";
        std::cout << std::endl;

        all_ok &= ok;
    }
    return all_ok;
}

// ============================================================================
// main
// ============================================================================
int main() {
    std::cout << "\n################################################" << std::endl;
    std::cout << "  CKKS Auto (EvalRotate) Diagnostic Tests       " << std::endl;
    std::cout << "################################################" << std::endl;

    // -------------------------------------------------------------------------
    // Step 1: 建立 CryptoContext
    // -------------------------------------------------------------------------
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(1);
    parameters.SetScalingModSize(50);
    parameters.SetRingDim(1 << 12);   // 4096 = FPGA_RING_DIM
    parameters.SetBatchSize(8);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    // -------------------------------------------------------------------------
    // Step 2: 初始化 FPGA
    // -------------------------------------------------------------------------
    std::vector<uint64_t> q_mods, q_roots, p_mods, p_roots;
    for (auto& rp : cc->GetElementParams()->GetParams()) {
        q_mods.push_back(rp->GetModulus().ConvertToInt());
        q_roots.push_back(rp->GetRootOfUnity().ConvertToInt());
    }
    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    if (auto paramsP = cryptoParams->GetParamsP()) {
        for (auto& pp : paramsP->GetParams()) {
            p_mods.push_back(pp->GetModulus().ConvertToInt());
            p_roots.push_back(pp->GetRootOfUnity().ConvertToInt());
        }
    }

    bool fpga_ok = FpgaManager::GetInstance().IsReady();
    if (fpga_ok) {
        FpgaManager::GetInstance().InitModuli(q_mods, p_mods, q_roots, p_roots);
        std::cout << "[Host] FPGA initialized." << std::endl;
    } else {
        std::cout << "[Host] FPGA not ready — hardware tests will be skipped." << std::endl;
    }

    // -------------------------------------------------------------------------
    // Step 3: Enable PKE
    // -------------------------------------------------------------------------
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalRotateKeyGen(keys.secretKey, {1, -1, 2, -2, 3});

    uint32_t N = cc->GetRingDimension();
    std::cout << "[Host] Ring dimension: " << N << std::endl;
    std::cout << "[Host] Q limbs: " << q_mods.size() << ", P limbs: " << p_mods.size() << std::endl;

    // galois element for rotation by 1: FindAutomorphismIndex2nComplex(1, 2N)
    uint32_t k_rot1 = FindAutomorphismIndex2nComplex(1, 2 * N);
    std::cout << "[Host] Galois element for rotation +1: k=" << k_rot1 << std::endl;

    // -------------------------------------------------------------------------
    // Step 4: 构造测试密文
    // -------------------------------------------------------------------------
    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    auto ct = cc->Encrypt(keys.publicKey, cc->MakeCKKSPackedPlaintext(x1));

    // -------------------------------------------------------------------------
    // Step 5: 运行各测试
    // -------------------------------------------------------------------------
    int total = 0, passed = 0;
    auto run = [&](const std::string& name, bool result) {
        ++total;
        if (result) ++passed;
        std::cout << "\n>>> " << name << ": " << (result ? "PASSED" : "FAILED") << std::endl;
    };

    if (fpga_ok) {
        run("INTT Step",      TestAutoInttStep(cc, ct));
        run("Auto Perm Step", TestAutoPermStep(cc, ct, k_rot1));
        run("NTT Step",       TestAutoNttStep(cc, ct, k_rot1));
        run("Full Pipeline",  TestAutoFullPipeline(cc, ct, k_rot1));
    } else {
        std::cout << "\n[WARN] FPGA not ready — skipping hardware unit tests 1-4." << std::endl;
    }

    // 端到端测试无论 FPGA 是否就绪都跑
    run("EvalRotate E2E", TestAutoEvalRotate(cc, keys));

    // -------------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------------
    std::cout << "\n================================================" << std::endl;
    std::cout << "  SUMMARY: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "================================================" << std::endl;

    return (passed == total) ? 0 : 1;
}
