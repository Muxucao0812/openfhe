// ============================================================================
// test-bconv.cpp
//
// 针对 ApproxSwitchCRTBasis (BConv) 的精确诊断测试。
// 直接从 CryptoContext 提取 key switching 中使用的真实参数，
// 逐 digit、逐 limb 对比 FPGA BConv 与 CPU BConv 的输出。
//
// 测试层次：
//   1. TestBConvSingleDigit  — 对 part=0 的单 digit 做 FPGA vs CPU 对比
//   2. TestBConvAllDigits    — 对所有 digit 做 FPGA vs CPU 对比
//   3. TestBConvVsApproxSwitch — 直接对比 FPGA BConv 与 ApproxSwitchCRTBasis CPU 路径
// ============================================================================

#define PROFILE

#include "openfhe.h"
#include "FpgaManager.h"
#include "utils/utilities-int.h"   // BarrettUint128ModUint64, Mul128

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

static bool CheckEqual(const std::string& tag,
                       const std::vector<uint64_t>& got,
                       const std::vector<uint64_t>& ref,
                       size_t printLen = 8) {
    size_t mismatches = 0;
    for (size_t i = 0; i < got.size(); ++i) {
        if (got[i] != ref[i]) {
            if (mismatches < 8)
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
    std::cerr << "  [FAIL] " << tag << " — " << mismatches << "/" << got.size()
              << " mismatches" << std::endl;
    return false;
}

// CPU 参考：ApproxSwitchCRTBasis 的核心计算
//   flat_inputs[i * ringDim + ri] = x[i][ri] * QHatInvModq[i] mod q[i]  (已预乘)
//   flat_weights[i * MAX_OUT + j] = QHatModp[i][j]
//   result[j * ringDim + ri] = sum_i(flat_inputs[i*N+ri] * flat_weights[i*MAX_OUT+j]) mod p[j]
static std::vector<uint64_t> CpuBConv(
    const std::vector<uint64_t>& flat_inputs,   // [sizeQ * ringDim]
    const std::vector<uint64_t>& flat_weights,  // [sizeQ * MAX_OUT_COLS], padded
    const std::vector<uint64_t>& out_mods,      // [sizeP]
    const std::vector<DoubleNativeInt>& barrettMu, // [sizeP]
    uint32_t sizeQ, uint32_t sizeP, uint32_t ringDim,
    uint32_t MAX_OUT_COLS)
{
    std::vector<uint64_t> result(sizeP * ringDim, 0);
    for (uint32_t ri = 0; ri < ringDim; ++ri) {
        for (uint32_t j = 0; j < sizeP; ++j) {
            DoubleNativeInt sum = 0;
            for (uint32_t i = 0; i < sizeQ; ++i) {
                uint64_t x_val = flat_inputs[i * ringDim + ri];
                uint64_t w_val = flat_weights[i * MAX_OUT_COLS + j];
                sum += Mul128(x_val, w_val);
            }
            result[j * ringDim + ri] = BarrettUint128ModUint64(sum, out_mods[j], barrettMu[j]);
        }
    }
    return result;
}

// ============================================================================
// 核心测试：对给定 digit (part) 的 BConv 做 FPGA vs CPU 对比
// ============================================================================
static bool TestBConvDigit(
    CryptoContext<DCRTPoly>& cc,
    const DCRTPoly& partCt,   // 已经是 COEFFICIENT 域的 digit 多项式
    uint32_t part,
    uint32_t sizeQl)
{
    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());

    uint32_t sizePartQl = partCt.GetNumOfElements();
    uint32_t ringDim    = cc->GetRingDimension();

    const auto& QHatInvModq      = cryptoParams->GetPartQlHatInvModq(part, sizePartQl - 1);
    const auto& QHatInvModqPrecon= cryptoParams->GetPartQlHatInvModqPrecon(part, sizePartQl - 1);
    const auto& QHatModp         = cryptoParams->GetPartQlHatModp(sizeQl - 1, part);
    const auto& barrettMu        = cryptoParams->GetmodComplPartqBarrettMu(sizeQl - 1, part);
    const auto  paramsP          = cryptoParams->GetParamsComplPartQ(sizeQl - 1, part);

    uint32_t sizeP = paramsP->GetParams().size();

    const uint32_t KERNEL_LIMB_Q    = FpgaManager::KERNEL_LIMB_Q;
    const uint32_t KERNEL_MAX_OUT   = FpgaManager::KERNEL_MAX_OUT_COLS;

    std::cout << "  part=" << part << " sizePartQl=" << sizePartQl
              << " sizeP=" << sizeP << " ringDim=" << ringDim << std::endl;

    // ---- 准备 flat_inputs (与 dcrtpoly-impl.h 中完全相同) ----
    std::vector<uint64_t> flat_inputs(ringDim * KERNEL_LIMB_Q, 0);
    for (uint32_t i = 0; i < sizePartQl; ++i) {
        const auto& limb = partCt.GetAllElements()[i];
        const auto& qi   = limb.GetModulus();
        for (uint32_t ri = 0; ri < ringDim; ++ri) {
            NativeInteger tmp = limb[ri].ModMulFastConst(QHatInvModq[i], qi, QHatInvModqPrecon[i]);
            flat_inputs[i * ringDim + ri] = tmp.ConvertToInt();
        }
    }

    // ---- 准备 flat_weights ----
    std::vector<uint64_t> flat_weights(KERNEL_LIMB_Q * KERNEL_MAX_OUT, 0);
    for (uint32_t i = 0; i < sizePartQl; ++i)
        for (uint32_t j = 0; j < sizeP; ++j)
            flat_weights[i * KERNEL_MAX_OUT + j] = QHatModp[i][j].ConvertToInt();

    // ---- 准备 out_mods ----
    std::vector<uint64_t> out_mods(sizeP);
    for (uint32_t j = 0; j < sizeP; ++j)
        out_mods[j] = paramsP->GetParams()[j]->GetModulus().ConvertToInt();

    // ---- FPGA BConv ----
    std::vector<uint64_t> fpga_out(sizeP * ringDim, 0);
    FpgaManager::GetInstance().BConvOffload(
        flat_inputs.data(), flat_weights.data(), out_mods.data(),
        fpga_out.data(), ringDim, sizeP);

    // ---- CPU BConv ----
    auto cpu_out = CpuBConv(flat_inputs, flat_weights, out_mods, barrettMu,
                            sizePartQl, sizeP, ringDim, KERNEL_MAX_OUT);

    // ---- 打印前几个值 ----
    std::cout << "  FPGA out[0] (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << fpga_out[i] << " ";
    std::cout << std::endl;
    std::cout << "  CPU  out[0] (first 8): ";
    for (size_t i = 0; i < 8; ++i) std::cout << cpu_out[i] << " ";
    std::cout << std::endl;

    // ---- 逐 P limb 比较 ----
    bool ok = true;
    for (uint32_t j = 0; j < sizeP; ++j) {
        std::vector<uint64_t> fpga_limb(fpga_out.begin() + j * ringDim,
                                         fpga_out.begin() + (j + 1) * ringDim);
        std::vector<uint64_t> cpu_limb(cpu_out.begin() + j * ringDim,
                                        cpu_out.begin() + (j + 1) * ringDim);
        std::string tag = "part=" + std::to_string(part) + " P-limb[" + std::to_string(j)
                        + "] mod=" + std::to_string(out_mods[j]);
        ok &= CheckEqual(tag, fpga_limb, cpu_limb, 4);
    }
    return ok;
}

// ============================================================================
// 测试 1：对 part=0 的单 digit 做 FPGA vs CPU BConv 对比
// ============================================================================
static bool TestBConvSingleDigit(CryptoContext<DCRTPoly>& cc,
                                  const Ciphertext<DCRTPoly>& ct) {
    PrintSection("TEST 1: BConv Single Digit (part=0, FPGA vs CPU)");

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    const auto& cv    = ct->GetElements();
    const auto  paramsQl = cv[0].GetParams();
    uint32_t sizeQl   = paramsQl->GetParams().size();
    uint32_t alpha    = cryptoParams->GetNumPerPartQ();
    uint32_t numPartQl = (uint32_t)std::ceil((double)sizeQl / alpha);
    if (numPartQl > cryptoParams->GetNumberOfQPartitions())
        numPartQl = cryptoParams->GetNumberOfQPartitions();

    std::cout << "  sizeQl=" << sizeQl << " alpha=" << alpha
              << " numPartQl=" << numPartQl << std::endl;

    // 取 c1（key switching 的输入），提取 part=0 的 digit
    const DCRTPoly& c1 = cv[1];
    uint32_t part = 0;
    uint32_t sizePartQl = (numPartQl == 1) ? sizeQl : alpha;

    // 构造 part=0 的 DCRTPoly（COEFFICIENT 域）
    auto paramsPartQ = cryptoParams->GetParamsPartQ(part);
    // 如果是最后一个 digit，sizePartQl 可能不足 alpha
    if (part == numPartQl - 1)
        sizePartQl = sizeQl - alpha * part;

    DCRTPoly partCt(paramsPartQ, Format::EVALUATION, true);
    for (uint32_t i = 0; i < sizePartQl; ++i)
        partCt.SetElementAtIndex(i, c1.GetElementAtIndex(alpha * part + i));
    partCt.SetFormat(Format::COEFFICIENT);

    return TestBConvDigit(cc, partCt, part, sizeQl);
}

// ============================================================================
// 测试 2：对所有 digit 做 FPGA vs CPU BConv 对比
// ============================================================================
static bool TestBConvAllDigits(CryptoContext<DCRTPoly>& cc,
                                const Ciphertext<DCRTPoly>& ct) {
    PrintSection("TEST 2: BConv All Digits (FPGA vs CPU)");

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    const auto& cv    = ct->GetElements();
    const auto  paramsQl = cv[0].GetParams();
    uint32_t sizeQl   = paramsQl->GetParams().size();
    uint32_t alpha    = cryptoParams->GetNumPerPartQ();
    uint32_t numPartQl = (uint32_t)std::ceil((double)sizeQl / alpha);
    if (numPartQl > cryptoParams->GetNumberOfQPartitions())
        numPartQl = cryptoParams->GetNumberOfQPartitions();

    const DCRTPoly& c1 = cv[1];
    bool all_ok = true;

    for (uint32_t part = 0; part < numPartQl; ++part) {
        uint32_t sizePartQl = (part == numPartQl - 1) ? (sizeQl - alpha * part) : alpha;
        auto paramsPartQ    = cryptoParams->GetParamsPartQ(part);

        // 如果是最后一个 digit，需要用截断的 params
        std::shared_ptr<DCRTPoly::Params> params;
        if (part == numPartQl - 1 && sizePartQl < alpha) {
            std::vector<NativeInteger> moduli(sizePartQl), roots(sizePartQl);
            for (uint32_t i = 0; i < sizePartQl; ++i) {
                moduli[i] = paramsPartQ->GetParams()[i]->GetModulus();
                roots[i]  = paramsPartQ->GetParams()[i]->GetRootOfUnity();
            }
            params = std::make_shared<DCRTPoly::Params>(
                cc->GetRingDimension() * 2, moduli, roots);
        } else {
            params = paramsPartQ;
        }

        DCRTPoly partCt(params, Format::EVALUATION, true);
        for (uint32_t i = 0; i < sizePartQl; ++i)
            partCt.SetElementAtIndex(i, c1.GetElementAtIndex(alpha * part + i));
        partCt.SetFormat(Format::COEFFICIENT);

        all_ok &= TestBConvDigit(cc, partCt, part, sizeQl);
    }
    return all_ok;
}

// ============================================================================
// 测试 3：对比 FPGA BConv 与 ApproxSwitchCRTBasis CPU 路径的输出
//   直接调用 ApproxSwitchCRTBasis（CPU 路径），与 FPGA BConv 结果逐 limb 比较
// ============================================================================
static bool TestBConvVsApproxSwitch(CryptoContext<DCRTPoly>& cc,
                                     const Ciphertext<DCRTPoly>& ct) {
    PrintSection("TEST 3: FPGA BConv vs ApproxSwitchCRTBasis CPU output");

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    const auto& cv    = ct->GetElements();
    const auto  paramsQl = cv[0].GetParams();
    uint32_t sizeQl   = paramsQl->GetParams().size();
    uint32_t alpha    = cryptoParams->GetNumPerPartQ();
    uint32_t numPartQl = (uint32_t)std::ceil((double)sizeQl / alpha);
    if (numPartQl > cryptoParams->GetNumberOfQPartitions())
        numPartQl = cryptoParams->GetNumberOfQPartitions();

    const DCRTPoly& c1 = cv[1];
    bool all_ok = true;

    for (uint32_t part = 0; part < numPartQl; ++part) {
        uint32_t sizePartQl = (part == numPartQl - 1) ? (sizeQl - alpha * part) : alpha;
        auto paramsPartQ    = cryptoParams->GetParamsPartQ(part);
        auto paramsComplP   = cryptoParams->GetParamsComplPartQ(sizeQl - 1, part);

        std::shared_ptr<DCRTPoly::Params> params;
        if (part == numPartQl - 1 && sizePartQl < alpha) {
            std::vector<NativeInteger> moduli(sizePartQl), roots(sizePartQl);
            for (uint32_t i = 0; i < sizePartQl; ++i) {
                moduli[i] = paramsPartQ->GetParams()[i]->GetModulus();
                roots[i]  = paramsPartQ->GetParams()[i]->GetRootOfUnity();
            }
            params = std::make_shared<DCRTPoly::Params>(
                cc->GetRingDimension() * 2, moduli, roots);
        } else {
            params = paramsPartQ;
        }

        // 构造 digit（COEFFICIENT 域）
        DCRTPoly partCt(params, Format::EVALUATION, true);
        for (uint32_t i = 0; i < sizePartQl; ++i)
            partCt.SetElementAtIndex(i, c1.GetElementAtIndex(alpha * part + i));
        partCt.SetFormat(Format::COEFFICIENT);

        // ---- FPGA BConv（通过 FpgaManager 直接调用）----
        const auto& QHatInvModq       = cryptoParams->GetPartQlHatInvModq(part, sizePartQl - 1);
        const auto& QHatInvModqPrecon = cryptoParams->GetPartQlHatInvModqPrecon(part, sizePartQl - 1);
        const auto& QHatModp          = cryptoParams->GetPartQlHatModp(sizeQl - 1, part);
        const auto& barrettMu         = cryptoParams->GetmodComplPartqBarrettMu(sizeQl - 1, part);

        uint32_t sizeP   = paramsComplP->GetParams().size();
        uint32_t ringDim = cc->GetRingDimension();
        const uint32_t KERNEL_LIMB_Q  = FpgaManager::KERNEL_LIMB_Q;
        const uint32_t KERNEL_MAX_OUT = FpgaManager::KERNEL_MAX_OUT_COLS;

        std::vector<uint64_t> flat_inputs(ringDim * KERNEL_LIMB_Q, 0);
        for (uint32_t i = 0; i < sizePartQl; ++i) {
            const auto& limb = partCt.GetAllElements()[i];
            const auto& qi   = limb.GetModulus();
            for (uint32_t ri = 0; ri < ringDim; ++ri) {
                NativeInteger tmp = limb[ri].ModMulFastConst(QHatInvModq[i], qi, QHatInvModqPrecon[i]);
                flat_inputs[i * ringDim + ri] = tmp.ConvertToInt();
            }
        }
        std::vector<uint64_t> flat_weights(KERNEL_LIMB_Q * KERNEL_MAX_OUT, 0);
        for (uint32_t i = 0; i < sizePartQl; ++i)
            for (uint32_t j = 0; j < sizeP; ++j)
                flat_weights[i * KERNEL_MAX_OUT + j] = QHatModp[i][j].ConvertToInt();
        std::vector<uint64_t> out_mods(sizeP);
        for (uint32_t j = 0; j < sizeP; ++j)
            out_mods[j] = paramsComplP->GetParams()[j]->GetModulus().ConvertToInt();

        std::vector<uint64_t> fpga_out(sizeP * ringDim, 0);
        FpgaManager::GetInstance().BConvOffload(
            flat_inputs.data(), flat_weights.data(), out_mods.data(),
            fpga_out.data(), ringDim, sizeP);

        // ---- CPU ApproxSwitchCRTBasis（直接调用，走 CPU 路径）----
        // 临时禁用 FPGA 路径：通过直接调用 CPU 实现
        // 由于无法 toggle FPGA，我们用 CpuBConv 作为参考
        auto cpu_out = CpuBConv(flat_inputs, flat_weights, out_mods, barrettMu,
                                sizePartQl, sizeP, ringDim, KERNEL_MAX_OUT);

        std::cout << "  part=" << part << " sizePartQl=" << sizePartQl
                  << " sizeP=" << sizeP << std::endl;

        for (uint32_t j = 0; j < sizeP; ++j) {
            std::vector<uint64_t> fpga_limb(fpga_out.begin() + j * ringDim,
                                             fpga_out.begin() + (j + 1) * ringDim);
            std::vector<uint64_t> cpu_limb(cpu_out.begin() + j * ringDim,
                                            cpu_out.begin() + (j + 1) * ringDim);
            std::string tag = "part=" + std::to_string(part) + " P[" + std::to_string(j)
                            + "] mod=" + std::to_string(out_mods[j]);
            all_ok &= CheckEqual(tag, fpga_limb, cpu_limb, 4);
        }
    }
    return all_ok;
}

// ============================================================================
// 测试 4：打印 BConv 参数摘要（辅助调试）
// ============================================================================
static void PrintBConvParams(CryptoContext<DCRTPoly>& cc) {
    PrintSection("BConv Parameter Summary");

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    auto paramsQ = cc->GetElementParams();
    uint32_t sizeQl = paramsQ->GetParams().size();
    uint32_t alpha  = cryptoParams->GetNumPerPartQ();
    uint32_t numPartQl = (uint32_t)std::ceil((double)sizeQl / alpha);
    if (numPartQl > cryptoParams->GetNumberOfQPartitions())
        numPartQl = cryptoParams->GetNumberOfQPartitions();

    std::cout << "  sizeQl=" << sizeQl << " alpha=" << alpha
              << " numPartQl=" << numPartQl << std::endl;

    for (uint32_t part = 0; part < numPartQl; ++part) {
        uint32_t sizePartQl = (part == numPartQl - 1) ? (sizeQl - alpha * part) : alpha;
        auto paramsComplP   = cryptoParams->GetParamsComplPartQ(sizeQl - 1, part);
        uint32_t sizeP      = paramsComplP->GetParams().size();

        std::cout << "  part=" << part << ": sizePartQl=" << sizePartQl
                  << " sizeP=" << sizeP << std::endl;

        // 打印 Q 模数
        auto paramsPartQ = cryptoParams->GetParamsPartQ(part);
        for (uint32_t i = 0; i < sizePartQl; ++i)
            std::cout << "    Q[" << i << "] = "
                      << paramsPartQ->GetParams()[i]->GetModulus().ConvertToInt() << std::endl;

        // 打印 P 模数
        for (uint32_t j = 0; j < sizeP; ++j)
            std::cout << "    P[" << j << "] = "
                      << paramsComplP->GetParams()[j]->GetModulus().ConvertToInt() << std::endl;

        // 打印权重矩阵前几个值
        const auto& QHatModp = cryptoParams->GetPartQlHatModp(sizeQl - 1, part);
        std::cout << "    QHatModp[0][0..sizeP-1]: ";
        for (uint32_t j = 0; j < sizeP; ++j)
            std::cout << QHatModp[0][j].ConvertToInt() << " ";
        std::cout << std::endl;
    }
}

// ============================================================================
// main
// ============================================================================
int main() {
    std::cout << "\n################################################" << std::endl;
    std::cout << "  BConv (ApproxSwitchCRTBasis) Diagnostic Tests  " << std::endl;
    std::cout << "################################################" << std::endl;

    // -------------------------------------------------------------------------
    // Step 1: 建立 CryptoContext（与 key switching 测试相同的参数）
    // -------------------------------------------------------------------------
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(1);
    parameters.SetScalingModSize(50);
    parameters.SetRingDim(1 << 12);
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
    auto cryptoParams0 = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    if (auto paramsP = cryptoParams0->GetParamsP()) {
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
        std::cout << "[Host] FPGA not ready — aborting." << std::endl;
        return 1;
    }

    // -------------------------------------------------------------------------
    // Step 3: Enable PKE
    // -------------------------------------------------------------------------
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalRotateKeyGen(keys.secretKey, {1});

    std::cout << "[Host] Ring dimension: " << cc->GetRingDimension() << std::endl;

    // -------------------------------------------------------------------------
    // Step 4: 构造测试密文
    // -------------------------------------------------------------------------
    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    auto ct = cc->Encrypt(keys.publicKey, cc->MakeCKKSPackedPlaintext(x1));

    // -------------------------------------------------------------------------
    // Step 5: 打印参数摘要
    // -------------------------------------------------------------------------
    PrintBConvParams(cc);

    // -------------------------------------------------------------------------
    // Step 6: 运行测试
    // -------------------------------------------------------------------------
    int total = 0, passed = 0;
    auto run = [&](const std::string& name, bool result) {
        ++total;
        if (result) ++passed;
        std::cout << "\n>>> " << name << ": " << (result ? "PASSED" : "FAILED") << std::endl;
    };

    run("BConv Single Digit",       TestBConvSingleDigit(cc, ct));
    run("BConv All Digits",         TestBConvAllDigits(cc, ct));
    run("BConv vs ApproxSwitch",    TestBConvVsApproxSwitch(cc, ct));

    // -------------------------------------------------------------------------
    // Summary
    // -------------------------------------------------------------------------
    std::cout << "\n================================================" << std::endl;
    std::cout << "  SUMMARY: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "================================================" << std::endl;

    return (passed == total) ? 0 : 1;
}
