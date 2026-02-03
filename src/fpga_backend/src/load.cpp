#include "define.h"
#include "../include/load.h"

void Load(
    const uint64_t *mem_in,
    uint64_t buffer[MAX_LIMBS][SQRT][SQRT],
    const int num_active_limbs,
    const int mod_index // 【新增】偏移量
) {
    #pragma HLS INLINE off
    for (int l = mod_index; l < mod_index + num_active_limbs; l++) {
        LOAD_ROW: 
        for (int i = 0; i < SQRT; i++) {
            LOAD_COL: 
            for (int j = 0; j < SQRT; j++) {
                #pragma HLS PIPELINE II=1
        
                buffer[l][i][j] = mem_in[ (l - mod_index) * RING_DIM + i * SQRT + j];
            }
        }
    }
}

void Store(
    uint64_t buffer[MAX_LIMBS][SQRT][SQRT],
    uint64_t *mem_out,
    const int num_active_limbs,
    const int mod_index // 【新增】偏移量
) {
    #pragma HLS INLINE off

    for (int l = mod_index; l < mod_index + num_active_limbs; l++) {
        // #pragma HLS UNROLL
        STORE_ROW: 
        for (int i = 0; i < SQRT; i++) {
            STORE_COL: 
            for (int j = 0; j < SQRT; j++) {
                mem_out[ (l - mod_index) * RING_DIM + i * SQRT + j] = buffer[l][i][j];
            }
        }
    }
}