#include "../include/bconv.h"
#include <hls_stream.h>

static const int TOTAL_CYCLES = LIMB_Q + RING_DIM + MAX_OUT_COLS - 1;

// =============================================================
// bconv_systolic: 脉动阵列核心计算
// =============================================================
// 设计思路：
//   顶层 local_in_x 定义为 1D [MAX_LIMBS][RING_DIM]，dim=1 complete
//   partition 后每个 limb 是独立 BRAM bank。
//   Systolic_Loop 内用线性下标 data_idx / valid_count[p] 直接访问，
//   每个 bank 每周期最多 1 读或 1 写，不会触发 full array load/store。
// =============================================================
void bconv_systolic(
    uint64_t in_x[MAX_LIMBS][RING_DIM],       // 1D 线性化
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP
) {
    #pragma HLS INLINE

    // in_x 继承调用方的 partition 属性，不再重复声明
    #pragma HLS ARRAY_PARTITION variable=in_w complete dim=0
    #pragma HLS ARRAY_PARTITION variable=out_mod complete dim=0

    // ---------------------------------------------------------
    // 脉动阵列寄存器
    // ---------------------------------------------------------
    ap_uint<64> x_reg[LIMB_Q][MAX_OUT_COLS + 1];
    ap_uint<128> sum_reg[MAX_OUT_COLS][LIMB_Q + 1];
    #pragma HLS ARRAY_PARTITION variable=x_reg complete dim=0
    #pragma HLS ARRAY_PARTITION variable=sum_reg complete dim=0

    Init_X_Reg: for (int q = 0; q < LIMB_Q; ++q) {
    #pragma HLS UNROLL
        for (int p = 0; p <= MAX_OUT_COLS; ++p) {
    #pragma HLS UNROLL
            x_reg[q][p] = 0;
        }
    }

    Init_Sum_Reg: for (int p = 0; p < MAX_OUT_COLS; ++p) {
    #pragma HLS UNROLL
        for (int q = 0; q <= LIMB_Q; ++q) {
    #pragma HLS UNROLL
            sum_reg[p][q] = 0;
        }
    }

    int valid_count[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=valid_count complete

    Init_Count: for (int p = 0; p < MAX_OUT_COLS; ++p) {
    #pragma HLS UNROLL
        valid_count[p] = 0;
    }

    // ---------------------------------------------------------
    // 主脉动循环
    // in_x 已是 1D，线性下标访问，每个 bank 每周期最多 1 次读/写
    // ---------------------------------------------------------
    Systolic_Loop: for (int t = 0; t < TOTAL_CYCLES; ++t) {
    #pragma HLS PIPELINE II=1

        ap_uint<64> x_curr[LIMB_Q][MAX_OUT_COLS + 1];
        ap_uint<128> sum_curr[MAX_OUT_COLS][LIMB_Q + 1];
        #pragma HLS ARRAY_PARTITION variable=x_curr complete dim=0
        #pragma HLS ARRAY_PARTITION variable=sum_curr complete dim=0

        // 快照当前寄存器状态
        Save_X: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            for (int p = 0; p <= MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
                x_curr[q][p] = x_reg[q][p];
            }
        }

        Save_Sum: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            for (int q = 0; q <= LIMB_Q; ++q) {
        #pragma HLS UNROLL
                sum_curr[p][q] = sum_reg[p][q];
            }
        }

        // 从 in_x 喂数据：in_x[q][data_idx]，每个 q bank 最多 1 次读
        Feed_X: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            int data_idx = t - q;
            if (data_idx >= 0 && data_idx < RING_DIM) {
                x_reg[q][0] = in_x[q][data_idx];
            } else {
                x_reg[q][0] = 0;
            }
        }

        Init_Sum: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            sum_reg[p][0] = 0;
        }

        // PE 阵列：纯寄存器运算
        PE_Row: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            PE_Col: for (int p = 0; p < MAX_OUT_COLS; ++p) {
            #pragma HLS UNROLL
                ap_uint<64> x_in = x_curr[q][p];
                ap_uint<128> sum_in = sum_curr[p][q];

                ap_uint<64> mod_p = out_mod[p];
                ap_uint<128> prod = ((ap_uint<128>)x_in * (ap_uint<128>)in_w[q][p]) % mod_p;
                ap_uint<128> sum_out = (sum_in + prod) % mod_p;

                x_reg[q][p + 1] = x_in;
                sum_reg[p][q + 1] = sum_out;
            }
        }

        // 收集输出：in_x[LIMB_Q+p][valid_count[p]]，每个 p bank 最多 1 次写
        Collect: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            int latency = LIMB_Q + p;
            if (p < sizeP && t >= latency && valid_count[p] < RING_DIM) {
                ap_uint<128> result = sum_reg[p][LIMB_Q];
                in_x[LIMB_Q + p][valid_count[p]] = (uint64_t)(result);
                valid_count[p]++;
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
    int sizeP
) {
    #pragma HLS INTERFACE m_axi port=in_x   bundle=gmem0
    #pragma HLS INTERFACE m_axi port=in_w   bundle=gmem1
    #pragma HLS INTERFACE s_axilite port=out_mod bundle=control
    #pragma HLS INTERFACE s_axilite port=sizeP bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // ----------------------------------------------
    // 1. 片上存储空间
    // ----------------------------------------------
    // 1D 线性化：[MAX_LIMBS][RING_DIM]，dim=1 complete partition
    // 每个 limb 是独立 BRAM bank，pipeline 内线性下标访问无冲突
    uint64_t local_in_x[MAX_LIMBS][RING_DIM];
    #pragma HLS BIND_STORAGE variable=local_in_x type=ram_2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_in_x type=complete dim=1

    // 权重和模数数据量小，完全打散成寄存器
    uint64_t local_w[LIMB_Q][MAX_OUT_COLS];
    uint64_t local_mod[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod complete dim=0

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
        local_mod[p] = out_mod[p];
    }

    // DDR 是 2D [SQRT][SQRT]，线性化搬入 1D [RING_DIM]
    Load_X: for (int l = 0; l < LIMB_Q; ++l) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                #pragma HLS PIPELINE II=1
                local_in_x[l][r * SQRT + c] = in_x[l][r][c];
            }
        }
    }

    // -----------------------------------------
    // 3. Compute Phase: 纯片上脉动阵列计算
    // -----------------------------------------
    bconv_systolic(local_in_x, local_w, local_mod, sizeP);

    // -----------------------------------------
    // 4. Store Phase: 片上 1D → DDR 2D
    // -----------------------------------------
    Store_X: for (int p = 0; p < sizeP; ++p) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                #pragma HLS PIPELINE II=1
                in_x[LIMB_Q + p][r][c] = local_in_x[LIMB_Q + p][r * SQRT + c];
            }
        }
    }
}
