#include "../include/mod_mult_kernel.h"

void Compute_Mult(
    uint64_t in1[MAX_LIMBS][SQRT][SQRT], // 输入 A (片上 BRAM)
    uint64_t in2[MAX_LIMBS][SQRT][SQRT], // 输入 B (片上 BRAM)
    uint64_t out[MAX_LIMBS][SQRT][SQRT], // 输出 C (片上 BRAM)
    uint64_t MODULUS[MAX_LIMBS],
    uint64_t K_HALF[MAX_LIMBS],
    uint64_t M[MAX_LIMBS],

    int num_active_limbs,                // 当前有效层数 (比如 44)
    int mod_idx_offset               // 如果有模数偏移 (通常是 0)
) {
    #pragma HLS ARRAY_PARTITION variable=in1 cyclic factor=8 dim=3
    #pragma HLS ARRAY_PARTITION variable=in2 cyclic factor=8 dim=3
    #pragma HLS ARRAY_PARTITION variable=out cyclic factor=8 dim=3

    Limb_Loop:
    for (int r = mod_idx_offset; r < num_active_limbs + mod_idx_offset; r++) {
        uint64_t q = MODULUS[r];
        uint64_t k_half = K_HALF[r];
        uint64_t m = M[r];
        Row_Loop:
        for (int i = 0; i < SQRT; i++) {
            Col_Loop:
            for (int j = 0; j < SQRT; j++) {
                #pragma HLS PIPELINE II=1
                #pragma HLS UNROLL factor=8
                uint64_t a = in1[r][i][j];
                uint64_t b = in2[r][i][j];
                uint64_t res;
                MultMod(a, b, q, m, k_half, res);
                out[r][i][j] = res;
            }
        }
    }
}