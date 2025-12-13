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
  Simple examples for CKKS Bootstrapping with FPGA Acceleration
 */

#define PROFILE

#include "openfhe.h"

// 【新增】引入 FPGA 管理器
// 如果编译报错找不到文件，请检查这个路径是否正确
#include "FpgaManager.h"

using namespace lbcrypto;

int main() {
    // ======================================================================
    // Step 1: Setup CryptoContext
    // ======================================================================
    
    // A. Specify main parameters
    uint32_t multDepth = 1;
    uint32_t scaleModSize = 50;
    uint32_t ringDegree = 1 << 12;
    uint32_t batchSize = 8;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDegree);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    // ======================================================================
    // 【FPGA 初始化模块 START】
    // 必须在 GenCryptoContext 之后，Enable 之前执行
    // ======================================================================
    std::cout << "\n################################################" << std::endl;
    std::cout << "       FPGA Initialization & Debug Info         " << std::endl;
    std::cout << "################################################" << std::endl;

    // 1. 准备容器
    std::vector<uint64_t> fpga_q_mods;
    std::vector<uint64_t> fpga_p_mods;

    // 获取degree
    auto ringDim = cc->GetRingDimension();
    std::cout << "[Host] Ring Dimension: " << ringDim << std::endl;

    // 2. 提取 Q 模数 (Ciphertext Moduli)
    auto elementParams = cc->GetElementParams();
    const auto& rnsParams = elementParams->GetParams();
    
    std::cout << "[Host] Extracting Q (Ciphertext Moduli)..." << std::endl;
    for (size_t i = 0; i < rnsParams.size(); i++) {
        uint64_t q_val = rnsParams[i]->GetModulus().ConvertToInt();
        fpga_q_mods.push_back(q_val);
        // 打印前几个看看
        if (i < 3 || i == rnsParams.size() - 1) {
            std::cout << "  Q[" << i << "]: " << q_val << std::endl;
        }
    }
    std::cout << "  Total Q Limbs: " << fpga_q_mods.size() << std::endl;

    // 3. 提取 P 模数 (Auxiliary Moduli for Keyswitching/Bootstrapping)
    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    auto paramsP = cryptoParams->GetParamsP();
    
    std::cout << "[Host] Extracting P (Auxiliary Moduli)..." << std::endl;
    if (paramsP) {
        const auto& rnsParamsP = paramsP->GetParams();
        for (size_t i = 0; i < rnsParamsP.size(); i++) {
            uint64_t p_val = rnsParamsP[i]->GetModulus().ConvertToInt();
            fpga_p_mods.push_back(p_val);
            std::cout << "  P[" << i << "]: " << p_val << std::endl;
        }
    } else {
        std::cout << "  [Warning] No P modulus found (Config might use specific security level)." << std::endl;
    }
    std::cout << "  Total P Limbs: " << fpga_p_mods.size() << std::endl;

    // 4. 调用 FPGA Init
    // 这会将 Q 和 P 发送到 FPGA 的 BRAM/URAM
    if (FpgaManager::GetInstance().IsReady()) {
        std::cout << "[Host] Sending moduli to FPGA..." << std::endl;
        FpgaManager::GetInstance().InitModuli(fpga_q_mods, fpga_p_mods);
        std::cout << "[Host] FPGA Initialization Done." << std::endl;
    } else {
        std::cerr << "\n[CRITICAL WARNING] FPGA not ready! Calculations will fail or fallback." << std::endl;
        // 如果你想在没有 FPGA 时终止程序，取消下面注释
        // return 1; 
    }
    std::cout << "################################################\n" << std::endl;
    // ======================================================================
    // 【FPGA 初始化模块 END】
    // ======================================================================


    // Enable the features that you wish to use
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    // B. Step 2: Key Generation
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalRotateKeyGen(keys.secretKey, {1, -2});

    // Step 3: Encoding and encryption of inputs
    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> x2 = {5.0, 4.0, 3.0, 2.0, 1.0, 0.75, 0.5, 0.25};

    Plaintext ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
    Plaintext ptxt2 = cc->MakeCKKSPackedPlaintext(x2);

    std::cout << "Input x1: " << ptxt1 << std::endl;
    std::cout << "Input x2: " << ptxt2 << std::endl;

    auto c1 = cc->Encrypt(keys.publicKey, ptxt1);
    auto c2 = cc->Encrypt(keys.publicKey, ptxt2);

    // Step 4: Evaluation
    // 这里的 EvalAdd, EvalMult 会在底层调用你的 FPGA 算子
    auto cAdd = cc->EvalAdd(c1, c2);
    auto cSub = cc->EvalSub(c1, c2);
    auto cScalar = cc->EvalMult(c1, 4.0);
    auto cMul = cc->EvalMult(c1, c2);
    auto cRot1 = cc->EvalRotate(c1, 1);
    auto cRot2 = cc->EvalRotate(c1, -2);

    // Step 5: Decryption and output
    Plaintext result;
    std::cout.precision(8);

    std::cout << std::endl << "Results of homomorphic computations: " << std::endl;

    cc->Decrypt(keys.secretKey, c1, &result);
    result->SetLength(batchSize);
    std::cout << "x1 = " << result;

    cc->Decrypt(keys.secretKey, cAdd, &result);
    result->SetLength(batchSize);
    std::cout << "x1 + x2 = " << result;

    cc->Decrypt(keys.secretKey, cSub, &result);
    result->SetLength(batchSize);
    std::cout << "x1 - x2 = " << result << std::endl;

    cc->Decrypt(keys.secretKey, cScalar, &result);
    result->SetLength(batchSize);
    std::cout << "4 * x1 = " << result << std::endl;

    cc->Decrypt(keys.secretKey, cMul, &result);
    result->SetLength(batchSize);
    std::cout << "x1 * x2 = " << result << std::endl;

    cc->Decrypt(keys.secretKey, cRot1, &result);
    result->SetLength(batchSize);
    std::cout << "x1 rotate by 1 = " << result << std::endl;

    cc->Decrypt(keys.secretKey, cRot2, &result);
    result->SetLength(batchSize);
    std::cout << "x1 rotate by -2 = " << result << std::endl;

    return 0;
}