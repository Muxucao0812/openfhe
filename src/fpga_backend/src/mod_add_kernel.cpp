#include "../include/mod_add_kernel.h"


// =========================================================
// 核心计算单元：加法 (适配 3D Memory 形状)
// =========================================================
void Compute_Add(
    uint64_t in1[MAX_LIMBS][SQRT][SQRT], // 输入 A (片上 BRAM)
    uint64_t in2[MAX_LIMBS][SQRT][SQRT], // 输入 B (片上 BRAM)
    uint64_t out[MAX_LIMBS][SQRT][SQRT], // 输出 C (片上 BRAM)
    uint64_t MODULUS[MAX_LIMBS],
    int num_active_limbs,                // 当前有效层数 (比如 44)
    int mod_idx_offset               // 如果有模数偏移 (通常是 0)
) {
    #pragma HLS ARRAY_PARTITION variable=in1 cyclic factor=PE_PARALLEL dim=3
    #pragma HLS ARRAY_PARTITION variable=in2 cyclic factor=PE_PARALLEL dim=3
    #pragma HLS ARRAY_PARTITION variable=out cyclic factor=PE_PARALLEL dim=3

    Limb_Loop:
    for (int r = mod_idx_offset; r < num_active_limbs + mod_idx_offset; r++) {
        uint64_t q = MODULUS[r];
        Row_Loop:
        for (int i = 0; i < SQRT; i++) {
            Col_Loop:
            for (int j = 0; j < SQRT; j++) {
                #pragma HLS PIPELINE II=1
                #pragma HLS UNROLL factor=PE_PARALLEL

                uint64_t a = in1[r][i][j];
                uint64_t b = in2[r][i][j];

                AddMod(a, b, q, true);

                out[r][i][j] = a;
            }
        }
    }
}