#include "openfhe.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace lbcrypto;
using std::cout;
using std::endl;
using std::vector;

void SwitchCKKStoFHEWAndBack() {
    cout << "\n===== CKKS -> FHEW -> CKKS (0/1 TEST) =====\n" << endl;

    // ---------------------------
    // 1. CKKS 上下文参数
    // ---------------------------
    uint32_t multDepth     = 3 + 9 + 1;
    uint32_t scaleModSize  = 50;
    uint32_t ringDim       = 8192;
    uint32_t slots         = 16;
    SecurityLevel sl       = HEStd_NotSet;

    CCParams<CryptoContextCKKSRNS> params;
    params.SetMultiplicativeDepth(multDepth);
    params.SetScalingModSize(scaleModSize);
    params.SetScalingTechnique(FIXEDAUTO);
    params.SetSecurityLevel(sl);
    params.SetRingDim(ringDim);
    params.SetBatchSize(slots);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(params);

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    cc->Enable(SCHEMESWITCH);

    auto keys = cc->KeyGen();

    cout << "CKKS ring dimension = " << cc->GetRingDimension()
         << ", slots = " << slots << endl;

    // ---------------------------
    // 2. CKKS <-> FHEW 切换参数
    // ---------------------------
    uint32_t        logQ_ccLWE   = 23;
    BINFHE_PARAMSET binParamSet  = TOY;

    SchSwchParams swParams;
    swParams.SetSecurityLevelCKKS(sl);
    swParams.SetSecurityLevelFHEW(binParamSet);
    swParams.SetCtxtModSizeFHEWLargePrec(logQ_ccLWE);
    swParams.SetNumSlotsCKKS(slots);
    swParams.SetNumValues(slots);

    // CKKS -> FHEW 端 setup：返回 LWE sk
    LWEPrivateKey skLWE = cc->EvalCKKStoFHEWSetup(swParams);
    auto ccLWE          = cc->GetBinCCForSchemeSwitch();

    cc->EvalCKKStoFHEWKeyGen(keys, skLWE);

    // FHEW -> CKKS setup + keygen
    cc->EvalFHEWtoCKKSSetup(ccLWE, slots, logQ_ccLWE);
    cc->EvalFHEWtoCKKSKeyGen(keys, skLWE);

    // ---------------------------
    // 3. pLWE & scaleCF
    // ---------------------------
    auto     pLWE_native = ccLWE->GetMaxPlaintextSpace();
    uint32_t pLWE        = pLWE_native.ConvertToInt();
    

    // Xiangchen: Adjust the pLWE 
    pLWE = 1<<12;
    double scaleCF = 1.0 / static_cast<double>(pLWE);

    cout << "FHEW plaintext modulus pLWE = " << pLWE << endl;
    cout << "scaleCF (CKKS -> FHEW) = 1 / pLWE = " << scaleCF << endl;

    cc->EvalCKKStoFHEWPrecompute(scaleCF);

    // ---------------------------
    // 4. 准备输入 & CKKS 加密
    // ---------------------------
    vector<double> data(slots, 0.0);
    // sample slots data from [-4,4]
    for (size_t i = 0; i < slots; ++i) {
        data[i] = static_cast<double>(rand() % 8001 - 4000) / 1000.0;  // [-4.0,4.0]
    }

    cout << "Original CKKS data: ";
    for (size_t i = 0; i < slots; ++i)
        cout << data[i] << " ";
    cout << endl;

    Plaintext pt = cc->MakeCKKSPackedPlaintext(data);
    auto      cCKKS = cc->Encrypt(keys.publicKey, pt);

    // ---------------------------
    // 5. CKKS -> FHEW
    // ---------------------------
    std::vector<LWECiphertext> cLWE = cc->EvalCKKStoFHEW(cCKKS, slots);

    std::vector<int32_t> lwe_plain(slots);
    cout << "FHEW decrypt (mod pLWE): ";
    for (size_t i = 0; i < slots; ++i) {
        LWEPlaintext m;
        ccLWE->Decrypt(skLWE, cLWE[i], &m, pLWE);
        lwe_plain[i] = static_cast<int32_t>(m);
        cout << lwe_plain[i] << " ";
    }
    cout << endl;

    // ---------------------------
    // 6. FHEW -> CKKS
    // ---------------------------
    auto cBack = cc->EvalFHEWtoCKKS(
        cLWE,
        slots,                // number of values
        slots,                // CKKS slots
        pLWE,                 // plaintext modulus (range hint)
        0.0,
        static_cast<double>(pLWE));

    Plaintext ptBack;
    cc->Decrypt(keys.secretKey, cBack, &ptBack);
    ptBack->SetLength(slots);
    auto backVals = ptBack->GetRealPackedValue();

    cout << "CKKS decrypt after round-trip: ";
    for (size_t i = 0; i < slots; ++i)
        cout << backVals[i] << " ";
    cout << endl;

    // ---------------------------
    // 7. 反量化 LWE 整数 -> 实数近似值
    // ---------------------------
    std::vector<double> lwe_real(slots);
    for (size_t i = 0; i < slots; ++i) {
        // 这里简单除以 pLWE，看回到 [0,1) 区间的效果
        lwe_real[i] = static_cast<double>(lwe_plain[i]) / static_cast<double>(pLWE);
    }

    // ---------------------------
    // 8. 写 CSV：idx, x_input_raw, lwe_plain, lwe_real, ckks_back
    // ---------------------------
    std::ofstream fout("ckks_fhew_roundtrip.csv");
    fout << "idx,x_input_raw,lwe_plain,lwe_real,ckks_back\n";
    for (size_t i = 0; i < slots; ++i) {
        fout << i << ","
             << data[i] << ","
             << lwe_plain[i] << ","
             << lwe_real[i] << ","
             << backVals[i] << "\n";
    }
    fout.close();

    cout << "Dumped CSV to ckks_fhew_roundtrip.csv\n";

    double max_abs_err = 0.0;
    double sum_abs_err = 0.0;
    double sum_sq_err  = 0.0;
    double max_rel_err = 0.0;

    const double eps = 1e-12;

    for (size_t i = 0; i < slots; ++i) {
        double diff  = backVals[i] - data[i];
        double adiff = std::abs(diff);

        // max absolute
        if (adiff > max_abs_err)
            max_abs_err = adiff;

        // L1 / MAE
        sum_abs_err += adiff;

        // L2 / RMSE
        sum_sq_err += diff * diff;

        // relative error
        double denom = std::max(std::abs(data[i]), eps);
        double rel   = adiff / denom;
        if (rel > max_rel_err)
            max_rel_err = rel;
    }

    double mae  = sum_abs_err / slots;
    double rmse = std::sqrt(sum_sq_err / slots);

    std::cout << "Max abs error   : " << max_abs_err << "\n";
    std::cout << "MAE             : " << mae << "\n";
    std::cout << "RMSE            : " << rmse << "\n";
    std::cout << "Max rel error   : " << max_rel_err << "\n";

    // bits from max abs error
    if (max_abs_err > 0) {
        double bits_inf = -std::log2(max_abs_err);
        std::cout << "Bits (from L∞)  : " << bits_inf << " bits\n";
    }

    // bits from RMSE
    if (rmse > 0) {
        double bits_rmse = -std::log2(rmse);
        std::cout << "Bits (from RMSE): " << bits_rmse << " bits\n";
    }
}

int main() {
    try {
        SwitchCKKStoFHEWAndBack();
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}