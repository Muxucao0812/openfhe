#ifndef AUTO_H
#define AUTO_H

#include <cstdint>
#include "define.h"

// Single-limb CKKS automorphism (coefficient domain): output[o] = ±input[i]
// where i = (o * kinv) mod 2N, negate when (o*kinv) mod 2N >= N.
// Used internally by Compute_Auto.
extern "C" void Auto(
    uint64_t input[SQRT][SQRT],
    uint32_t k,
    uint32_t kinv,
    uint64_t output[SQRT][SQRT],
    uint64_t mod
);

// Limb-parallel CKKS automorphism: same permutation for all limbs,
// each limb uses its own modulus for negation.
extern "C" void Compute_Auto(
    uint64_t input[MAX_LIMBS][SQRT][SQRT],
    uint32_t k,
    uint32_t kinv,
    uint64_t output[MAX_LIMBS][SQRT][SQRT],
    uint64_t MODULUS[MAX_LIMBS],
    int num_active_limbs,
    int mod_index
);

#endif // AUTO_H
