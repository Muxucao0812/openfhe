#include "../include/bconv.h"
#include <hls_stream.h>

static const int MULTMOD_LAT  = 4;
static const int TOTAL_CYCLES = LIMB_Q + RING_DIM + MAX_OUT_COLS - 1 + MULTMOD_LAT;

// =============================================================
// bconv_systolic: 脉动阵列核心计算
// =============================================================
// 设计思路：
//   输入和输出使用独立数组，避免 HLS full array load/store 错误。
//   in_x [LIMB_Q][RING_DIM]  —— 只读，partition dim=1 complete
//   out_x[MAX_OUT_COLS][RING_DIM] —— 只写，partition dim=1 complete
//   每个 bank 每周期最多 1 次读或 1 次写，访问模式清晰无冲突。
// =============================================================
void bconv_systolic(
    const uint64_t in_x[LIMB_Q][RING_DIM],    // 输入只读
    uint64_t out_x[MAX_OUT_COLS][RING_DIM],    // 输出只写
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_k_half[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
) {
    #pragma HLS INLINE

    // 输入输出分离，各自继承调用方的 partition 属性
    #pragma HLS ARRAY_PARTITION variable=in_w complete dim=0
    #pragma HLS ARRAY_PARTITION variable=out_mod complete dim=0

    // ---------------------------------------------------------
    // 脉动阵列寄存器
    // ---------------------------------------------------------
    ap_uint<64> x_reg[LIMB_Q][MAX_OUT_COLS + 1];
    uint64_t sum_reg[MAX_OUT_COLS][LIMB_Q + 1];  // 从 ap_uint<128> 降为 uint64_t
    #pragma HLS ARRAY_PARTITION variable=x_reg complete dim=0
    #pragma HLS ARRAY_PARTITION variable=sum_reg complete dim=0

    // ---------------------------------------------------------
    // MultMod 延迟补偿移位寄存器
    // ---------------------------------------------------------
    uint64_t prod_pipe[MULTMOD_LAT + 1][LIMB_Q][MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=prod_pipe complete dim=0

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

    Init_Prod_Pipe: for (int d = 0; d <= MULTMOD_LAT; ++d) {
    #pragma HLS UNROLL
        for (int q = 0; q < LIMB_Q; ++q) {
    #pragma HLS UNROLL
            for (int p = 0; p < MAX_OUT_COLS; ++p) {
    #pragma HLS UNROLL
                prod_pipe[d][q][p] = 0;
            }
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
    // in_x 只读，out_x 只写，访问模式清晰无冲突
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

        // 从 in_x 喂数据：in_x[q][data_idx]，只读访问
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

        // 移位 prod_pipe：先腾出位置，再写入新值（HLS SRL 推断优化）
        Shift_Prod_Pipe: for (int lat = MULTMOD_LAT; lat > 0; --lat) {
        #pragma HLS UNROLL
            for (int q = 0; q < LIMB_Q; ++q) {
            #pragma HLS UNROLL
                for (int p = 0; p < MAX_OUT_COLS; ++p) {
                #pragma HLS UNROLL
                    prod_pipe[lat][q][p] = prod_pipe[lat - 1][q][p];
                }
            }
        }

        // PE 阵列：纯寄存器运算
        PE_Row: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            PE_Col: for (int p = 0; p < MAX_OUT_COLS; ++p) {
            #pragma HLS UNROLL
                ap_uint<64> x_in = x_curr[q][p];
                uint64_t sum_in = sum_curr[p][q];

                ap_uint<64> mod_p = out_mod[p];

                // 1. 启动 MultMod，结果写入 prod_pipe[0]
                MultMod((uint64_t)x_in, in_w[q][p], (uint64_t)mod_p,
                        out_m_barrett[p], out_k_half[p],
                        prod_pipe[0][q][p]);

                // 2. 使用 MULTMOD_LAT 拍前的乘法结果做累加
                uint64_t delayed_prod = prod_pipe[MULTMOD_LAT][q][p];
                uint64_t sum_tmp = sum_in + delayed_prod;
                if (sum_tmp >= (uint64_t)mod_p) sum_tmp -= (uint64_t)mod_p;

                x_reg[q][p + 1] = x_in;
                sum_reg[p][q + 1] = sum_tmp;
            }
        }

        // 收集输出：写入 out_x[p][valid_count[p]]，只写访问
        Collect: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            int latency = LIMB_Q + p + MULTMOD_LAT;
            if (p < sizeP && t >= latency && valid_count[p] < RING_DIM) {
                uint64_t result = sum_reg[p][LIMB_Q];
                out_x[p][valid_count[p]] = result;
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
    const uint64_t out_k_half[MAX_OUT_COLS],
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
    uint64_t local_k_half[MAX_OUT_COLS];
    uint64_t local_m_barrett[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_k_half complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_m_barrett complete dim=0

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
        local_k_half[p] = out_k_half[p];
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
    bconv_systolic(local_in_x, local_out_x, local_w, local_mod, local_k_half, local_m_barrett, sizeP);
    // 4. Store Phase: 片上 1D → DDR 2D（从 local_out_x 搬出）
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
