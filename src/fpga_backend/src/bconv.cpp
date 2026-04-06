#include "../include/bconv.h"
#include <hls_stream.h>

static const int TOTAL_CYCLES = LIMB_Q + RING_DIM + MAX_OUT_COLS - 1;

void bconv_systolic(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],       
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],  
    const uint64_t out_mod[MAX_OUT_COLS],       
    int sizeP
) {
    
    // ----------------------------------------------
    // 1. 开辟片上存储空间 (Local Memories)
    // ----------------------------------------------
    // X 数据量大，用BRAM，仅在第一维拆分满足 UNROLL
    uint64_t local_in_x[MAX_LIMBS][SQRT][SQRT];
    #pragma HLS BIND_STORAGE variable=local_in_x type=ram_2p impl=bram
    #pragma HLS ARRAY_PARTITION variable=local_in_x type=complete dim=1

    // 权重和模数数据量小，且需要极高并发，完全打散成寄存器
    uint64_t local_w[LIMB_Q][MAX_OUT_COLS];
    uint64_t local_mod[MAX_OUT_COLS];
    #pragma HLS ARRAY_PARTITION variable=local_w complete dim=0
    #pragma HLS ARRAY_PARTITION variable=local_mod complete dim=0

    // -----------------------------------------
    // 2. Load Phase: 将全局 DDR 数据搬运到片上
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

    Load_X: for(int l = 0;l < LIMB_Q; ++l){
        for(int r = 0;r < SQRT; ++r){
            for(int c = 0;c < SQRT; ++c){
                #pragma HLS PIPELINE II=1
                local_in_x[l][r][c] =in_x[l][r][c];
            }
        }
    }

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

    ap_uint<64> x_curr[LIMB_Q][MAX_OUT_COLS + 1];
    ap_uint<128> sum_curr[MAX_OUT_COLS][LIMB_Q + 1];

    Systolic_Loop: for (int t = 0; t < TOTAL_CYCLES; ++t) {
#pragma HLS PIPELINE II=1
        
        
#pragma HLS ARRAY_PARTITION variable=x_curr complete dim=0
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
            
            if (data_idx >= 0 && data_idx < RING_DIM) {
                int row = data_idx / SQRT;
                int col = data_idx % SQRT;
                //FIXME -- Done
                x_reg[q][0] = local_in_x[q][row][col];
            } else {
                x_reg[q][0] = 0;
            }
        }
        
        //because no weight comes from the top, they are inited at the begining of the Loop
        Init_Sum: for (int p = 0; p < MAX_OUT_COLS; ++p) {
#pragma HLS UNROLL
            sum_reg[p][0] = 0;
        }
        
        PE_Row: for (int q = 0; q < LIMB_Q; ++q) {
#pragma HLS UNROLL
            PE_Col: for (int p = 0; p < MAX_OUT_COLS; ++p) {
#pragma HLS UNROLL
                ap_uint<64> x_in = x_curr[q][p];
                ap_uint<128> sum_in = sum_curr[p][q];
                
                // MAC operation using column-specific modulus
                ap_uint<64> mod_p = local_mod[p];
                ap_uint<128> prod = ((ap_uint<128>)x_in * (ap_uint<128>)local_w[q][p]) % mod_p;
                ap_uint<128> sum_out = (sum_in + prod) % mod_p;
                
                // Write outputs (for next cycle)
                x_reg[q][p + 1] = x_in;        // Pass x to right
                sum_reg[p][q + 1] = sum_out;   // Pass sum to bottom
            }
        }
        
        // -----------------------------------------
        // Stage 4: Collect outputs (with de-skewing)
        // Column p output is ready after LIMB_Q + p cycles
        // Only collect for active columns (p < sizeP)
        // -----------------------------------------
        Collect: for (int p = 0; p < MAX_OUT_COLS; ++p) {
#pragma HLS UNROLL
            int latency = LIMB_Q + p;
            
            // Only collect if this column is active
            if (p < sizeP && t >= latency && valid_count[p] < RING_DIM) {
                ap_uint<128> result = sum_reg[p][LIMB_Q];
                int row = valid_count[p] / SQRT;
                int col = valid_count[p] % SQRT;
                //FIXME --Done
                local_in_x[LIMB_Q + p][row][col] = (uint64_t)(result);
                
                valid_count[p]++;
            }
        }
    }

    // -----------------------------------------
    // 4. Store Phase: 将 BRAM 中的新数据写回外部 DDR
    // -----------------------------------------
    Store_X: for (int p = 0; p < sizeP; ++p) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                #pragma HLS PIPELINE II=1
                in_x[LIMB_Q + p][r][c] = local_in_x[LIMB_Q + p][r][c];
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

    // 所有的全局总线接口（DDR）统一定义在顶层
    #pragma HLS INTERFACE m_axi port=in_x   bundle=gmem0
    #pragma HLS INTERFACE m_axi port=in_w   bundle=gmem1
    #pragma HLS INTERFACE s_axilite port=out_mod bundle=control 
    #pragma HLS INTERFACE s_axilite port=sizeP bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    
    bconv_systolic(in_x, in_w, out_mod, sizeP);

}
