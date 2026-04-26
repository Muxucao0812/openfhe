#include "../include/bconv.h"
#include <hls_stream.h>

// =============================================================
// bconv_core: 向量内积阵列核心计算
// =============================================================
// 设计思路：
//   外层遍历 RING_DIM 个系数，每个系数独立完成所有输出列的计算。
//   内层 LIMB_Q × MAX_OUT_COLS 全部展开，HLS 自动推断：
//     - 15 个并行 MultMod 流水线（各自 II=1，LATENCY=4）
//     - 每列 LIMB_Q 级加法树
//   零跨迭代依赖，II=1 可达。
//
//   in_x [LIMB_Q][RING_DIM]      —— 只读，dim=1 complete partition
//   out_x[MAX_OUT_COLS][RING_DIM] —— 只写，dim=1 complete partition
// =============================================================
void bconv_core(
    const uint64_t in_x[LIMB_Q][RING_DIM],
    uint64_t out_x[MAX_OUT_COLS][RING_DIM],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_S[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
) {
    #pragma HLS INLINE

    #pragma HLS ARRAY_PARTITION variable=in_w          complete dim=0
    #pragma HLS ARRAY_PARTITION variable=out_mod       complete dim=0
    #pragma HLS ARRAY_PARTITION variable=out_S         complete dim=0
    #pragma HLS ARRAY_PARTITION variable=out_m_barrett complete dim=0

    // 外层：遍历每个系数，每拍处理一个系数的全部输出
    Coeff_Loop: for (int n = 0; n < RING_DIM; ++n) {
    #pragma HLS PIPELINE II=1

        // 内层展开：MAX_OUT_COLS 个输出列并行计算
        Out_Col: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL

            // LIMB_Q 个乘法并行启动，HLS 调度器自动处理各自的 4 拍延迟
            uint64_t prod[LIMB_Q];
            #pragma HLS ARRAY_PARTITION variable=prod complete

            In_Row: for (int q = 0; q < LIMB_Q; ++q) {
            #pragma HLS UNROLL
                MultMod(in_x[q][n], in_w[q][p],
                        out_mod[p], out_m_barrett[p], out_S[p],
                        prod[q]);

            }

            // 加法树（LIMB_Q=3）：两级归约，使用uint128_t避免溢出
            uint128_t s01 = (uint128_t)prod[0] + prod[1];
            if (s01 >= out_mod[p]) s01 -= out_mod[p];

            uint128_t s = s01 + prod[2];
            if (s >= out_mod[p]) s -= out_mod[p];

            if (p < sizeP) {
                out_x[p][n] = (uint64_t)s;
            }
        }
    }
}

// =================================================
// Host Side Wrapper
// =================================================
void Compute_BConv(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_S[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
) {
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
    uint64_t local_S[MAX_OUT_COLS];
    uint64_t local_m_barrett[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w          complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod        complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_S          complete dim=0
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
        local_S[p]         = out_S[p];
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
    // 3. Compute Phase: 输入只读，输出只写，无冲突
    // -----------------------------------------
    bconv_core(local_in_x, local_out_x,
               local_w, local_mod, local_S, local_m_barrett,
               sizeP);

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
