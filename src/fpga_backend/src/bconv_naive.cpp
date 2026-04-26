#include "../include/bconv_naive.h"

void Compute_BConv_Naive(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_k_half[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
) {
    #pragma HLS INLINE
    
    // ----------------------------------------------
    // 1. 片上存储空间（输入/输出分离，避免 full array load/store）
    // ----------------------------------------------
    // 输入数组：只读，LIMB_Q 个 bank，每个 bank RING_DIM
    uint64_t local_in_x[LIMB_Q][RING_DIM];
    #pragma HLS BIND_STORAGE variable=local_in_x type=ram_2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_in_x type=complete dim=1

    // 输出数组：只写，MAX_OUT_COLS 个 bank，每个 bank RING_DIM
    uint64_t local_out_x[MAX_OUT_COLS][RING_DIM];
    #pragma HLS BIND_STORAGE variable=local_out_x type=ram_2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_out_x type=complete dim=1

    // 权重、模数与 Barrett 常数数据量小，完全打散成寄存器
    uint64_t local_w[LIMB_Q][MAX_OUT_COLS];
    uint64_t local_mod[MAX_OUT_COLS];
    uint64_t local_k_half[MAX_OUT_COLS];
    uint64_t local_m_barrett[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w          complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod        complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_k_half     complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_m_barrett  complete dim=0

    // -----------------------------------------
    // 2. Load Phase: DDR → 片上
    // -----------------------------------------
    Load_W: for (int q = 0; q < LIMB_Q; ++q) {
        for (int p = 0; p < MAX_OUT_COLS; ++p) {
            #pragma HLS PIPELINE II=1
            local_w[q][p] = in_w[q][p];
        }
    }

    Load_Mod: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS PIPELINE II=1
        local_mod[p]       = out_mod[p];
        local_k_half[p]    = out_k_half[p];
        local_m_barrett[p] = out_m_barrett[p];
    }

    // DDR 是 2D [SQRT][SQRT]，线性化搬入 1D [RING_DIM]（只搬输入 limbs）
    Load_X: for (int l = 0; l < LIMB_Q; ++l) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                #pragma HLS PIPELINE II=1
                local_in_x[l][r * SQRT + c] = in_x[l][r][c];
            }
        }
    }

    // -----------------------------------------
    // 3. Compute Phase: 朴素串行累加
    // -----------------------------------------
    Compute_Loop: for (int n = 0; n < RING_DIM; ++n) {
        for (int p = 0; p < sizeP; ++p) {
            #pragma HLS PIPELINE II=1
            uint64_t sum = 0;
            
            for (int q = 0; q < LIMB_Q; ++q) {
                // 使用128位乘法避免溢出
                uint128_t prod = (uint128_t)local_in_x[q][n] * local_w[q][p];
                
                // 模运算：先对prod取模，然后累加
                uint64_t prod_mod = prod % local_mod[p];
                sum = (sum + prod_mod) % local_mod[p];
            }
            
            local_out_x[p][n] = sum;
        }
    }

    // -----------------------------------------
    // 4. Store Phase: 片上 1D → DDR 2D
    // -----------------------------------------
    Store_X: for (int p = 0; p < sizeP; ++p) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                #pragma HLS PIPELINE II=1
                in_x[LIMB_Q + p][r][c] = local_out_x[p][r * SQRT + c];
            }
        }
    }
}