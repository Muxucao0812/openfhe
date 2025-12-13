#ifndef MOD_MULT_KERNEL_H
#define MOD_MULT_KERNEL_H

#include "define.h"
#include "arithmetic.h"

extern "C" {
    void Compute_Mult(
        uint64_t in1[MAX_LIMBS][SQRT][SQRT], // 输入 A (片上 BRAM)
        uint64_t in2[MAX_LIMBS][SQRT][SQRT], // 输入 B (片上 BRAM)
        uint64_t out[MAX_LIMBS][SQRT][SQRT], // 输出 C (片上 BRAM)
        uint64_t MODULUS[MAX_LIMBS],
        uint64_t K_HALF[MAX_LIMBS],
        uint64_t M[MAX_LIMBS],
        int num_active_limbs,                // 当前有效层数 (比如 44)
        int mod_idx_offset = 0                // 如果有模数偏移 (通常是 0)
    );
}

#endif