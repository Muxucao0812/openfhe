#include "load.h"

void Load(
    const uint64_t *src_ddr,
    uint64_t dest_local[MAX_LIMBS][SQRT][SQRT],
    int num_active_limbs
) {
    #pragma HLS INLINE off

    Load_Limb_Loop:
    for (int limb = 0; limb < num_active_limbs; ++limb) {
        // 对每个 limb 进行加载
        Load_Row_Outer:
        for (int i = 0; i < SQRT; ++i) {
            Load_Row_Inner:
            for (int j = 0; j < SQRT; ++j) {
                #pragma HLS PIPELINE II=1
                // 计算在 DDR 中的线性地址
                size_t addr = limb * RING_DIM + i * SQRT + j;
                dest_local[limb][i][j] = src_ddr[addr];
            }
        }
    }
}


void Store(
    const uint64_t source_local[MAX_LIMBS][SQRT][SQRT],
    uint64_t *dest_ddr,
    int num_active_limbs
) {
    #pragma HLS INLINE off

    Store_Limb_Loop:
    for (int limb = 0; limb < num_active_limbs; ++limb) {
        // 对每个 limb 进行存储
        Store_Row_Outer:
        for (int i = 0; i < SQRT; ++i) {
            Store_Row_Inner:
            for (int j = 0; j < SQRT; ++j) {
                #pragma HLS PIPELINE II=1
                // 计算在 DDR 中的线性地址
                size_t addr = limb * RING_DIM + i * SQRT + j;
                dest_ddr[addr] = source_local[limb][i][j];
            }
        }
    }
}

