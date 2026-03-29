#ifndef LBCRYPTO_CRYPTO_KEYSWITCH_HKS_STRATEGY_H
#define LBCRYPTO_CRYPTO_KEYSWITCH_HKS_STRATEGY_H

namespace lbcrypto {

enum class HKSStrategy {
    DC,  // Digit-Centric: per-digit INTT→BConv→NTT (default, matches current code)
    MP,  // Max-Parallel:  all-INTT → all-BConv → all-NTT (global barriers between phases)
    OC,  // Output-Centric: per-output-tower BConv with sizeP=1 (minimal peak SRAM)
};

inline HKSStrategy& GetHKSStrategy() {
    static HKSStrategy s = HKSStrategy::DC;
    return s;
}

inline void SetHKSStrategy(HKSStrategy s) {
    GetHKSStrategy() = s;
}

// ---------------------------------------------------------------------------
// Per-KeySwitch operation statistics
// Reset via ResetHKSStats() before a call; read via GetHKSStats() after.
// ---------------------------------------------------------------------------
struct HKSStats {
    // --- Operation counts (EvalKeySwitchPrecomputeCore) ---
    int intt_poly   = 0;  // DCRTPoly-level INTT calls  (each covers alpha limbs)
    int ntt_poly    = 0;  // DCRTPoly-level NTT  calls
    int ntt_limb    = 0;  // single-limb NTT calls (OC: one per P-tower per digit)
    int bconv       = 0;  // ApproxSwitchCRTBasis (BConv) calls

    // --- Operation counts (EvalFastKeySwitchCoreExt) ---
    int modmul_limb = 0;  // limb-level multiply-accumulate iterations

    // --- Parameters captured at precompute time ---
    int num_digits  = 0;  // numPartQl
    int size_ql     = 0;  // current ciphertext Q limbs
    int size_p      = 0;  // auxiliary P limbs
    int alpha       = 0;  // limbs per digit
    int ring_dim    = 0;  // N

    // --- Peak SRAM: max P-tower complement ring-elements held simultaneously ---
    // One ring-element = ring_dim * 8 bytes.
    // DC:  sizeP            (one full complement per digit, one digit at a time)
    // MP:  numPartQl*sizeP  (all complements held simultaneously)
    // OC:  1                (one P-tower at a time)
    int peak_p_towers = 0;
};

inline HKSStats& GetHKSStats() {
    static HKSStats s;
    return s;
}

inline void ResetHKSStats() {
    GetHKSStats() = HKSStats{};
}

}  // namespace lbcrypto


#endif
