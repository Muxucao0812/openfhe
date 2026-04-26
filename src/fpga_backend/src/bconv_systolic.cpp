#include "../include/bconv_systolic.h"
#include "../include/arithmetic.h"
#include <hls_stream.h>

static const int TOTAL_CYCLES = LIMB_Q + RING_DIM + MAX_OUT_COLS - 1;

// ==================================================================
// I/O 带宽参数：匹配 Alveo 512-bit AXI（8 × 64-bit/拍）
// ==================================================================
constexpr int LOAD_PAR = 8;
static_assert((SQRT & (SQRT - 1)) == 0, "SQRT must be power of 2");
static_assert((RING_DIM % LOAD_PAR) == 0, "RING_DIM must be multiple of LOAD_PAR");
static constexpr int SQRT_MASK = SQRT - 1;  // SQRT=64 时取模用

// =============================================================
// bconv_systolic_core: 脉动阵列核心计算
// =============================================================
static void bconv_systolic_core(
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

    ap_uint<64> x_reg  [LIMB_Q]      [MAX_OUT_COLS + 1];
    uint64_t    sum_reg[MAX_OUT_COLS] [LIMB_Q + 1];
    #pragma HLS ARRAY_PARTITION variable=x_reg   complete dim=0
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

    Systolic_Loop: for (int t = 0; t < TOTAL_CYCLES; ++t) {
    #pragma HLS PIPELINE II=1

        ap_uint<64> x_curr  [LIMB_Q]      [MAX_OUT_COLS + 1];
        uint64_t    sum_curr[MAX_OUT_COLS] [LIMB_Q + 1];
        #pragma HLS ARRAY_PARTITION variable=x_curr   complete dim=0
        #pragma HLS ARRAY_PARTITION variable=sum_curr complete dim=0

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

        Feed_X: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            int data_idx = t - q;
            x_reg[q][0] = (data_idx >= 0 && data_idx < RING_DIM)
                          ? (ap_uint<64>)in_x[q][data_idx]
                          : (ap_uint<64>)0;
        }

        Init_Sum: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            sum_reg[p][0] = 0;
        }

        PE_Row: for (int q = 0; q < LIMB_Q; ++q) {
        #pragma HLS UNROLL
            PE_Col: for (int p = 0; p < MAX_OUT_COLS; ++p) {
            #pragma HLS UNROLL
                uint64_t x_in    = (uint64_t)x_curr[q][p];
                uint64_t sum_in  = sum_curr[p][q];
                uint64_t mod_p   = out_mod[p];

                uint64_t prod = 0;
                MultMod(x_in, in_w[q][p], mod_p, out_m_barrett[p], out_S[p], prod);

                // prod < mod_p，sum_in < mod_p → 和至多 2*mod_p，一次条件减即可
                uint64_t sum_out = sum_in + prod;
                if (sum_out >= mod_p) sum_out -= mod_p;

                x_reg[q][p + 1]   = x_in;
                sum_reg[p][q + 1] = sum_out;
            }
        }

        // 直接从 t 计算输出下标，消除 valid_count 的跨迭代依赖
        Collect: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS UNROLL
            int out_idx = t - (LIMB_Q + p);
            if (p < sizeP && out_idx >= 0 && out_idx < RING_DIM) {
                out_x[p][out_idx] = sum_reg[p][LIMB_Q];
            }
        }
    }
}

// =================================================
// Compute_BConv_Systolic: 顶层接口
// =================================================
void Compute_BConv_Systolic(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_S[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
) {
    // in_x 其实是 top.cpp 里的片上 poly_buffer（BRAM），不是 DDR。
    // 告诉 HLS dim=3 按 LOAD_PAR 交错 → 对外暴露 8 个物理端口
    #pragma HLS ARRAY_PARTITION variable=in_x type=cyclic factor=LOAD_PAR dim=3

    // 标量/小数组参数走 AXI-Lite 控制（仅 cosim 顶层需要；top.cpp 调用时会被忽略）
    #pragma HLS INTERFACE s_axilite port=out_mod bundle=control
    #pragma HLS INTERFACE s_axilite port=sizeP   bundle=control
    #pragma HLS INTERFACE s_axilite port=return  bundle=control

    // 环维度按 LOAD_PAR 交错，使 Load/Store 单拍可并行 8 路访问
    uint64_t local_in_x[LIMB_Q][RING_DIM];
    #pragma HLS BIND_STORAGE variable=local_in_x type=ram_1wnr impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_in_x type=complete dim=1
    #pragma HLS ARRAY_PARTITION variable=local_in_x type=cyclic factor=LOAD_PAR dim=2

    uint64_t local_out_x[MAX_OUT_COLS][RING_DIM];
    #pragma HLS BIND_STORAGE variable=local_out_x type=ram_s2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_out_x type=complete dim=1
    #pragma HLS ARRAY_PARTITION variable=local_out_x type=cyclic factor=LOAD_PAR dim=2

    uint64_t local_w  [LIMB_Q][MAX_OUT_COLS];
    uint64_t local_mod[MAX_OUT_COLS];
    uint64_t local_S  [MAX_OUT_COLS];
    uint64_t local_m_barrett[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w          complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod        complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_S          complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_m_barrett  complete dim=0

    Load_W: for (int q = 0; q < LIMB_Q; ++q) {
        for (int p = 0; p < MAX_OUT_COLS; ++p) {
            #pragma HLS PIPELINE II=1
            local_w[q][p] = in_w[q][p];
        }
    }

    Load_Mod: for (int p = 0; p < MAX_OUT_COLS; ++p) {
        #pragma HLS PIPELINE II=1
        local_mod[p]        = out_mod[p];
        local_S[p]          = out_S[p];
        local_m_barrett[p]  = out_m_barrett[p];
    }

    // Load_X: in_x 的 dim=3 已 8 路划分，idx & SQRT_MASK 正好落在 8 个独立 bank
    Load_X: for (int l = 0; l < LIMB_Q; ++l) {
        for (int i = 0; i < RING_DIM; i += LOAD_PAR) {
            #pragma HLS PIPELINE II=1
            Load_Burst: for (int pp = 0; pp < LOAD_PAR; ++pp) {
                #pragma HLS UNROLL
                int idx = i + pp;
                local_in_x[l][idx] = in_x[l][idx >> LOG_SQRT][idx & SQRT_MASK];
            }
        }
    }

    bconv_systolic_core(local_in_x, local_out_x, local_w, local_mod, local_S, local_m_barrett, sizeP);

    // Store_X: 同样走 8 端口并行写回
    Store_X: for (int p = 0; p < sizeP; ++p) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUT_COLS avg=3
        for (int i = 0; i < RING_DIM; i += LOAD_PAR) {
            #pragma HLS PIPELINE II=1
            Store_Burst: for (int pp = 0; pp < LOAD_PAR; ++pp) {
                #pragma HLS UNROLL
                int idx = i + pp;
                in_x[LIMB_Q + p][idx >> LOG_SQRT][idx & SQRT_MASK] = local_out_x[p][idx];
            }
        }
    }
}

