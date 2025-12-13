/*
  GELU Approximation using:
  1) Plaintext (baseline)
  2) CKKS Chebyshev Interpolation
  3) CKKS -> TFHE/BinFHE LUT-based Evaluation (direct):
     CKKS(real x) -> FHEW(LUT: integer-coded GELU) -> CKKS(real)

  - Same input samples for all three
 */

#define PROFILE  // turns on the reporting of timing results
#define NUMLIMBS 10

#include "openfhe.h"
#include "binfhecontext.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

#ifdef __linux__
#include <unistd.h>
#include <cstdio>
#endif

using namespace lbcrypto;

//===================== Common GELU helpers =====================//


// ---------------------------------------------------------
// Helpers for TFHE: Z_p <-> [xmin, xmax]
// ---------------------------------------------------------
double IntToRealUniform(uint32_t m, double xmin, double xmax, uint32_t p) {
    if (m >= p)
        m %= p;
    if (p <= 1)
        return xmin;
    double t = static_cast<double>(m) / static_cast<double>(p - 1);  // [0,1]
    return xmin + t * (xmax - xmin);
}

uint32_t RealToIntUniform(double x, double xmin, double xmax, uint32_t p) {
    if (x < xmin)
        x = xmin;
    if (x > xmax)
        x = xmax;
    if (p <= 1)
        return 0;
    double t = (x - xmin) / (xmax - xmin);  // [0,1]
    uint64_t v =
        static_cast<uint64_t>(std::llround(t * static_cast<double>(p - 1)));
    if (v >= p)
        v = p - 1;
    return static_cast<uint32_t>(v);
}


double gelu_scalar(double x) {
    return 0.5 * x *
           (1.0 + std::tanh(std::sqrt(2.0 / M_PI) *
                            (x + 0.044715 * std::pow(x, 3))));
}

void gelu_plaintext(const std::vector<double>& input,
                    std::vector<double>& output) {
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
        output[i] = gelu_scalar(input[i]);
}

void generate_input(std::vector<double>& input) {
    input.clear();
    double start = -4.0;
    double end   =  4.0;
    double step  =  0.5;

    for (double v = start; v <= end + 1e-12; v += step)
        input.push_back(v);

    std::cout << "Number of input points: " << input.size() << std::endl;
}

//===================== Plaintext baseline =====================//

void run_plain_gelu(const std::vector<double>& input,
                    std::vector<double>& y_plain) {
    std::cout << "\n====== Plaintext GELU baseline ======\n" << std::endl;
    gelu_plaintext(input, y_plain);
    std::cout << std::setprecision(7) << std::fixed;
    std::cout << "Sample results (first " << NUMLIMBS << "):\n";
    for (size_t i = 0; i < NUMLIMBS && i < input.size(); ++i) {
        std::cout << "x = " << std::setw(8) << input[i]
                  << ", GELU(x) = " << std::setw(20) << y_plain[i] << "\n";
    }
}

//===================== CKKS Chebyshev-based GELU =====================//

void run_ckks_gelu(const std::vector<double>& input,
                   const std::vector<double>& y_plain,
                   std::vector<double>&       y_ckks) {
    std::cout << "\n====== CKKS GELU approximation (Chebyshev) ======\n" << std::endl;

    uint32_t multDepth    = 30;
    uint32_t scaleModSize = 50;
    uint32_t ringDim      = 8192;
    uint32_t batchSize    = ringDim / 2;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDim);
    parameters.SetBatchSize(batchSize);
    parameters.SetSecurityLevel(HEStd_NotSet);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);

    auto keyPair = cc->KeyGen();
    cc->EvalMultKeyGen(keyPair.secretKey);

    Plaintext ptxt = cc->MakeCKKSPackedPlaintext(input);
    auto      ctxt = cc->Encrypt(keyPair.publicKey, ptxt);

    double   lowerBound = -4.0;
    double   upperBound =  4.0;
    uint32_t polyDegree = 12;

    auto ctxtResult = cc->EvalChebyshevFunction(
        [](double x) -> double {
            return gelu_scalar(x);
        },
        ctxt, lowerBound, upperBound, polyDegree);

    Plaintext ptxtResult;
    cc->Decrypt(keyPair.secretKey, ctxtResult, &ptxtResult);
    ptxtResult->SetLength(input.size());
    y_ckks = ptxtResult->GetRealPackedValue();

    std::cout << std::setprecision(7) << std::fixed;
    std::cout << "Sample results (first " << NUMLIMBS << "):\n";
    for (size_t i = 0; i < NUMLIMBS && i < input.size(); ++i) {
        std::cout << "x = " << std::setw(8) << input[i]
                  << ", GELU_CKKS(x)   = " << std::setw(10) << y_ckks[i]
                  << ", GELU_plain(x) = " << std::setw(10) << y_plain[i] << "\n";
    }
}

//===================== BinFHE LUT: GELU on integer domain =====================//

constexpr double X_MIN = -4.0;
constexpr double X_MAX =  4.0;
constexpr double Y_MIN = -1.0;
constexpr double Y_MAX =  1.0;

// m ∈ [0,p-1] -> 量化 x -> GELU(x) -> 量化到 [0,p-1]
NativeInteger GeluIntLUT(NativeInteger m, NativeInteger pNative) {
    uint32_t p = pNative.ConvertToInt();

    double m_d = static_cast<double>(m.ConvertToInt());
    double x   = (m_d / static_cast<double>(p - 1)) * (X_MAX - X_MIN) + X_MIN;

    double g = gelu_scalar(x);
    if (g < Y_MIN) g = Y_MIN;
    if (g > Y_MAX) g = Y_MAX;

    double code  = (g - Y_MIN) / (Y_MAX - Y_MIN) * static_cast<double>(p - 1);
    long   rcode = static_cast<long>(std::llround(code));

    if (rcode < 0)          rcode = 0;
    if (rcode > long(p-1))  rcode = p-1;

    return NativeInteger(static_cast<uint64_t>(rcode));
}

//===================== CKKS -> FHEW(LUT-GELU) -> CKKS =====================//
void run_ckks_tfhe_gelu_fhew(const std::vector<double>& input,
                             const std::vector<double>& y_plain,
                             std::vector<double>&       y_tfhe) {
    std::cout << "\n====== CKKS -> FHEW LUT-GELU -> CKKS (direct) ======\n" << std::endl;

    // 1) CKKS context
    uint32_t numValues = 16;
    uint32_t multDepth    = 20;
    uint32_t scaleModSize = 50;
    uint32_t ringDim      = 8192;
    uint32_t batchSize    = ringDim / 2;
    SecurityLevel   sl    = HEStd_NotSet;
    BINFHE_PARAMSET slBin = TOY;  // 官方示例也常用 TOY 做 demo

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDim);
    parameters.SetBatchSize(batchSize);
    parameters.SetSecurityLevel(sl);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    cc->Enable(SCHEMESWITCH);
    cc->Enable(FHE);

    auto keyPair = cc->KeyGen();
    cc->EvalMultKeyGen(keyPair.secretKey);

    std::cout << "CKKS ring dimension = " << cc->GetRingDimension()
              << ", slots = " << batchSize << std::endl;

    // 2) CKKS <-> FHEW scheme switching (官方 FuncViaSchemeSwitching 风格)
    SchSwchParams params;
    params.SetSecurityLevelCKKS(sl);
    params.SetSecurityLevelFHEW(slBin);
    params.SetArbitraryFunctionEvaluation(true);      // 关键：允许 EvalFunc
    params.SetNumSlotsCKKS(numValues);
    params.SetNumValues(input.size());
    // 如果需要，也可以设置 ctxt modulus 位宽：
    // params.SetCtxtModSizeFHEWLargePrec(23);

    // 让 OpenFHE 建好 BinFHE context + LWE sk
    auto skLWE = cc->EvalSchemeSwitchingSetup(params);
    auto ccLWE = cc->GetBinCCForSchemeSwitch();

    // 生成 CKKS<->FHEW 所需的各种密钥
    cc->EvalSchemeSwitchingKeyGen(keyPair, skLWE);

    // 为 FHEW EvalFunc 生成 bootstrapping key
    ccLWE->BTKeyGen(skLWE);

    // 3) 明文模数 pLWE + CKKS->FHEW 的 scale
    auto     pLWE_native = ccLWE->GetMaxPlaintextSpace();
    uint32_t pLWE        = pLWE_native.ConvertToInt();
    double   scaleCF     = 1.0 / static_cast<double>(pLWE);

    std::cout << "FHEW plaintext modulus pLWE = " << pLWE << std::endl;
    std::cout << "BinFHE: q = "
              << ccLWE->GetParams()->GetLWEParams()->Getq()
              << ", N = "
              << ccLWE->GetParams()->GetLWEParams()->GetN()
              << " (must satisfy q <= N for EvalFunc)\n";

    cc->EvalCKKStoFHEWPrecompute(scaleCF);

    // 4) 构建 GELU 的 LUT：NativeInteger -> NativeInteger
    auto lut = ccLWE->GenerateLUTviaFunction(GeluIntLUT, pLWE_native);

    // 5) CKKS 编码 + 加密输入
    Plaintext ptxt = cc->MakeCKKSPackedPlaintext(input);
    auto      ctxt = cc->Encrypt(keyPair.publicKey, ptxt);

    // 6) CKKS -> FHEW
    auto cLWE = cc->EvalCKKStoFHEW(ctxt, numValues);

    std::cout << "FHEW decrypt of mapped inputs (first " << NUMLIMBS << "): ";
    for (size_t i = 0; i < NUMLIMBS && i < cLWE.size(); ++i) {
        LWEPlaintext m;
        ccLWE->Decrypt(skLWE, cLWE[i], &m, pLWE);
        std::cout << m << " ";
    }
    std::cout << std::endl;

    // 7) 在 FHEW 上通过 LUT 实现 GELU
    std::vector<LWECiphertext> cLWE_gelu(cLWE.size());
    for (size_t i = 0; i < cLWE.size(); ++i)
        cLWE_gelu[i] = ccLWE->EvalFunc(cLWE[i], lut);

    std::cout << "FHEW decrypt after GELU LUT (integer codes, first "
              << NUMLIMBS << "): ";
    for (size_t i = 0; i < NUMLIMBS && i < cLWE_gelu.size(); ++i) {
        LWEPlaintext m;
        ccLWE->Decrypt(skLWE, cLWE_gelu[i], &m, pLWE);
        std::cout << m << " ";
    }
    std::cout << std::endl;

    // 8) FHEW -> CKKS
    // 这里的 pRange / scale 和 FuncViaSchemeSwitching 后半段一致：
    // 把 [0, pLWE-1] 的整数 code 映射回 CKKS 的实数，再自己反量化为 [-1,1]
    auto ctxtBack = cc->EvalFHEWtoCKKS(
        cLWE_gelu,
        input.size(),               // 有效元素个数
        batchSize,                  // CKKS slots
        pLWE,                       // plaintext 模数
        0.0,                        // lower bound (整数域)
        static_cast<double>(pLWE)   // upper bound
    );

    Plaintext ptxtBack;
    cc->Decrypt(keyPair.secretKey, ctxtBack, &ptxtBack);
    ptxtBack->SetLength(input.size());
    auto backVals = ptxtBack->GetRealPackedValue();

    // 9) 将 CKKS 输出近似看作整数 code，再反量化到 [Y_MIN, Y_MAX]
    y_tfhe.resize(input.size());

    std::cout << std::setprecision(7) << std::fixed;
    std::cout << "Sample comparison (first " << NUMLIMBS << "):\n";
    for (size_t i = 0; i < NUMLIMBS && i < input.size(); ++i) {
        int m = static_cast<int>(std::llround(backVals[i]));
        if (m < 0) m = 0;
        if (m > int(pLWE - 1)) m = pLWE - 1;

        // 反量化：code ∈ [0, pLWE-1] -> y ∈ [Y_MIN, Y_MAX]
        double g_dec =
            (static_cast<double>(m) / (pLWE - 1)) * (Y_MAX - Y_MIN) + Y_MIN;
        y_tfhe[i] = g_dec;

        std::cout << "x = " << std::setw(8) << input[i]
                  << ", GELU_plain(x)   = " << std::setw(10) << y_plain[i]
                  << ", GELU_TFHE_LUT   = " << std::setw(10) << g_dec
                  << ", |err| = "
                  << std::setw(10) << std::fabs(g_dec - y_plain[i])
                  << "\n";
    }
}
//===================== main =====================//


void GeluViaSchemeSwitching() {
    std::cout << "\n-----GeluViaSchemeSwitching-----\n" << std::endl;
    std::cout << "CKKS -> FHEW (GELU via LUT) -> CKKS\n" << std::endl;

    // Step 1: Setup CryptoContext for CKKS（基本照抄 FloorViaSchemeSwitching）
    ScalingTechnique scTech = FLEXIBLEAUTO;

    // 深度：1 层 CKKS->FHEW + FHEW->CKKS + 一点点余量
    uint32_t multDepth = 3 + 9 + 1;
    if (scTech == FLEXIBLEAUTOEXT)
        multDepth += 1;
    uint32_t scaleModSize = 50;
    uint32_t ringDim      = 8192;
    SecurityLevel sl      = HEStd_NotSet;
    BINFHE_PARAMSET slBin = TOY;
    uint32_t logQ_ccLWE   = 23;
    uint32_t slots        = 16;  // 简单起见，少量 slot
    uint32_t batchSize    = slots;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetScalingTechnique(scTech);
    parameters.SetSecurityLevel(sl);
    parameters.SetRingDim(ringDim);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    cc->Enable(SCHEMESWITCH);

    std::cout << "CKKS ringDim = " << cc->GetRingDimension()
              << ", slots = " << slots
              << ", multDepth = " << multDepth << std::endl << std::endl;

    auto keys = cc->KeyGen();

    // Step 2: 准备 FHEW context + 切换用的 key
    SchSwchParams params;
    params.SetSecurityLevelCKKS(sl);
    params.SetSecurityLevelFHEW(slBin);
    params.SetCtxtModSizeFHEWLargePrec(logQ_ccLWE);
    params.SetNumSlotsCKKS(slots);
    params.SetNumValues(slots);
    auto privateKeyFHEW = cc->EvalSchemeSwitchingSetup(params);
    auto ccLWE          = cc->GetBinCCForSchemeSwitch();

    // CKKS<->FHEW 双向切换的密钥
    cc->EvalSchemeSwitchingKeyGen(keys, privateKeyFHEW);

    // FHEW 端 bootstrapping key（EvalFunc 要用）
    ccLWE->BTKeyGen(privateKeyFHEW);

    std::cout << "FHEW n = " << ccLWE->GetParams()->GetLWEParams()->Getn()
              << ", logQ = " << logQ_ccLWE
              << ", q = " << ccLWE->GetParams()->GetLWEParams()->Getq()
              << std::endl << std::endl;

    // ---- 这里开始是 GELU 相关设置 ----

    // 对于 LUT，使用 FHEW 支持的“最大明文空间”作为 pLWE，一般是很小的 (3–4 bit)
    auto modulus_LWE = 1 << logQ_ccLWE;
    auto beta        = ccLWE->GetBeta().ConvertToInt();
    auto pLWE        = modulus_LWE / (2 * beta);  // Large precision
    double scaleCF   = 1.0 / pLWE;

    std::cout << "Using pLWE (for LUT) = " << pLWE << std::endl;

    // CKKS -> FHEW 的预计算（告知缩放系数）
    cc->EvalCKKStoFHEWPrecompute(scaleCF);

     // Step 3: Encoding and encryption of inputs
    // Inputs
    std::vector<double> x1 = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};

    // Encoding as plaintexts
    Plaintext ptxt1 = cc->MakeCKKSPackedPlaintext(x1, 1, 0, nullptr);

    // Encrypt the encoded vectors
    auto c1 = cc->Encrypt(keys.publicKey, ptxt1);

    // Step 4: Scheme switching from CKKS to FHEW
    auto cTemp = cc->EvalCKKStoFHEW(c1);

    // Step 5: Evaluate the floor function
    uint32_t bits = 2;

    
    std::vector<LWECiphertext> cFloor(cTemp.size());
    for (uint32_t i = 0; i < cTemp.size(); i++) {
        cFloor[i] = ccLWE->EvalFloor(cTemp[i], bits);
    }

    auto ptxt1_rpv = ptxt1->GetRealPackedValue();
    std::cout << "Input x1: " << ptxt1_rpv << std::endl;
    std::cout << "Expected result for EvalFloor with " << bits << " bits: ";
    for (uint32_t i = 0; i < slots; ++i) {
        std::cout << (static_cast<int>(ptxt1_rpv[i]) >> bits) << " ";
    }
    LWEPlaintext pFloor;
    std::cout << "\nFHEW decryption p = " << NativeInteger(pLWE)
              << "/(1 << bits) = " << NativeInteger(pLWE) / (1 << bits) << ": ";
    for (uint32_t i = 0; i < cFloor.size(); ++i) {
        ccLWE->Decrypt(privateKeyFHEW, cFloor[i], &pFloor, pLWE / (1 << bits));
        std::cout << pFloor << " ";
    }
    std::cout << "\n" << std::endl;

    // Step 6: Scheme switching from FHEW to CKKS
    auto cTemp2 = cc->EvalFHEWtoCKKS(cFloor, slots, slots, pLWE / (1 << bits), 0, pLWE / (1 << bits));

    Plaintext plaintextDec2;
    cc->Decrypt(keys.secretKey, cTemp2, &plaintextDec2);
    plaintextDec2->SetLength(slots);
    std::cout << "Switched floor decryption modulus_LWE mod " << NativeInteger(pLWE) / (1 << bits) << ": "
              << plaintextDec2 << std::endl;
}


int main() {
    std::vector<double> input;
    generate_input(input);

    std::vector<double> y_plain;
    std::vector<double> y_ckks;
    std::vector<double> y_tfhe;

    // 1. Plaintext baseline
    // run_plain_gelu(input, y_plain);

    // 2. CKKS Chebyshev GELU
    // run_ckks_gelu(input, y_plain, y_ckks);

    GeluViaSchemeSwitching();
    // 3. CKKS -> FHEW(LUT-GELU) -> CKKS
    // run_ckks_tfhe_gelu_fhew(input, y_plain, y_tfhe);

    return 0;
}