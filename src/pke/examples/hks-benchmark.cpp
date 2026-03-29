#define PROFILE

#include "openfhe.h"
#include "FpgaManager.h"
#include "keyswitch/hks_strategy.h"

#include <chrono>
#include <cstring>
#include <iostream>

using namespace lbcrypto;

static void PrintUsage(const char* prog) {
    std::cout << "Usage: " << prog << " --strategy <DC|MP|OC> [--iters N]\n"
              << "  DC  Digit-Centric (default): per-digit INTT→BConv→NTT\n"
              << "  MP  Max-Parallel:            all-INTT → all-BConv → all-NTT\n"
              << "  OC  Output-Centric:          per-P-tower BConv (min peak SRAM)\n"
              << "  --iters N  number of EvalRotate calls to time (default 10)\n";
}

int main(int argc, char* argv[]) {
    // -------------------------------------------------------------------------
    // Parse arguments
    // -------------------------------------------------------------------------
    HKSStrategy strategy = HKSStrategy::DC;
    int iters = 10;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--strategy") == 0 && i + 1 < argc) {
            std::string s = argv[++i];
            if (s == "MP")      strategy = HKSStrategy::MP;
            else if (s == "OC") strategy = HKSStrategy::OC;
            else if (s == "DC") strategy = HKSStrategy::DC;
            else { PrintUsage(argv[0]); return 1; }
        } else if (std::strcmp(argv[i], "--iters") == 0 && i + 1 < argc) {
            iters = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--help") == 0) {
            PrintUsage(argv[0]); return 0;
        }
    }

    SetHKSStrategy(strategy);
    const char* stratName[] = {"DC", "MP", "OC"};
    const char* sname = stratName[static_cast<int>(strategy)];
    std::cout << "[HKS-Bench] Strategy: " << sname << "  Iters: " << iters << "\n\n";

    // -------------------------------------------------------------------------
    // CryptoContext setup
    // -------------------------------------------------------------------------
    uint32_t multDepth    = 1;
    uint32_t scaleModSize = 50;
    uint32_t ringDegree   = 1 << 12;
    uint32_t batchSize    = 8;

    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecurityLevel(HEStd_NotSet);
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetRingDim(ringDegree);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    // -------------------------------------------------------------------------
    // FPGA initialization (must happen after GenCryptoContext, before Enable)
    // -------------------------------------------------------------------------
    std::vector<uint64_t> fpga_q_mods, fpga_p_mods, fpga_q_roots, fpga_p_roots;

    auto elementParams = cc->GetElementParams();
    for (const auto& p : elementParams->GetParams()) {
        fpga_q_mods.push_back(p->GetModulus().ConvertToInt());
        fpga_q_roots.push_back(p->GetRootOfUnity().ConvertToInt());
    }

    auto cryptoParams = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
    auto paramsP = cryptoParams->GetParamsP();
    if (paramsP) {
        for (const auto& p : paramsP->GetParams()) {
            fpga_p_mods.push_back(p->GetModulus().ConvertToInt());
            fpga_p_roots.push_back(p->GetRootOfUnity().ConvertToInt());
        }
    }

    if (FpgaManager::GetInstance().IsReady()) {
        std::cout << "[Host] Initializing FPGA...\n";
        FpgaManager::GetInstance().InitModuli(fpga_q_mods, fpga_p_mods, fpga_q_roots, fpga_p_roots);
        std::cout << "[Host] FPGA ready.\n\n";
    } else {
        std::cout << "[Host] FPGA not available, running on CPU.\n\n";
    }

    // -------------------------------------------------------------------------
    // Key generation
    // -------------------------------------------------------------------------
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    auto keys = cc->KeyGen();
    cc->EvalRotateKeyGen(keys.secretKey, {1});

    std::vector<double> x1 = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    auto ptxt = cc->MakeCKKSPackedPlaintext(x1);
    auto ctxt = cc->Encrypt(keys.publicKey, ptxt);

    // -------------------------------------------------------------------------
    // Warm-up (1 call to JIT any lazy init)
    // -------------------------------------------------------------------------
    cc->EvalRotate(ctxt, 1);

    // -------------------------------------------------------------------------
    // Capture per-operation stats (single call after warm-up)
    // -------------------------------------------------------------------------
    ResetHKSStats();
    cc->EvalRotate(ctxt, 1);
    HKSStats s = GetHKSStats();

    size_t p_tower_bytes = (size_t)s.peak_p_towers * s.ring_dim * sizeof(uint64_t);

    std::cout << "\n====================================================\n";
    std::cout << "  HKS Strategy Evaluation Report\n";
    std::cout << "====================================================\n";
    std::cout << "  Strategy        : " << sname << "\n";
    std::cout << "  Ring Dim (N)    : " << s.ring_dim << "\n";
    std::cout << "  sizeQl          : " << s.size_ql << "\n";
    std::cout << "  sizeP           : " << s.size_p << "\n";
    std::cout << "  Digits          : " << s.num_digits
              << "  (alpha=" << s.alpha << ")\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "  [Operation Counts per KeySwitch]\n";
    std::cout << "  INTT (poly)     : " << s.intt_poly << "\n";
    std::cout << "  NTT  (poly)     : " << s.ntt_poly << "\n";
    if (s.ntt_limb > 0)
        std::cout << "  NTT  (limb, OC) : " << s.ntt_limb << "\n";
    std::cout << "  BConv           : " << s.bconv << "\n";
    std::cout << "  ModMul (limb)   : " << s.modmul_limb << "\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "  [Peak SRAM - P-tower complement buffer]\n";
    std::cout << "  P-towers held   : " << s.peak_p_towers << "\n";
    std::cout << "  Buffer size     : " << p_tower_bytes << " bytes"
              << "  (" << p_tower_bytes / 1024.0 << " KB)\n";
    std::cout << "----------------------------------------------------\n";

    // -------------------------------------------------------------------------
    // Timed benchmark
    // -------------------------------------------------------------------------
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iters; i++) {
        cc->EvalRotate(ctxt, 1);
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    double total_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "  [Timing (" << iters << " iters)]\n";
    std::cout << "  Total           : " << total_ms << " ms\n";
    std::cout << "  Avg per op      : " << total_ms / iters << " ms\n";
    std::cout << "====================================================\n";

    return 0;
}
