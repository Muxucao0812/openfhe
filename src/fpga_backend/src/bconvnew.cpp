#include "../include/bconvnew.h"
#include <hls_stream.h>
#include <iostream>

static const int TOTAL_CYCLES = LIMB_Q + RING_DIM +MAX_OUT_COLS - 1;

void bconv_systolic(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP
){
#pragma HLS INTERFACE m_axi port=in_x   bundle=gmem0
#pragma HLS INTERFACE m_axi port=in_w   bundle=gmem1
#pragma HLS INTERFACE s_axilite port=out_mod bundle=control
#pragma HLS INTERFACE s_axilite port=sizeP bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    ap_uint<64> local_w[LIMB_Q][MAX_OUT_COLS];
#pragma HLS ARRAY_PARTITION variable=local_w complete dim=0

    Load_W: for(int q = 0; q < LIMB_Q; ++q){
        for(int p = 0; p < MAX_OUT_COLS; ++p){
#pragma HLS PIPELINE II=1
            local_w[q][p] = in_w[q][p];
        }
    }

    ap_uint<64> local_mod[MAX_OUT_COLS];
#pragma HLS ARRAY_PARTITION variable=local_mod complete
    
    Load_Mod: for (int p = 0; p < MAX_OUT_COLS; ++p) {
#pragma HLS PIPELINE II=1
        local_mod[p] = out_mod[p];
    }

   ap_uint<64> x_reg[LIMB_Q][MAX_OUT_COLS + 1];
   ap_uint<128> sum_reg[MAX_OUT_COLS][LIMB_Q + 1];

#pragma HLS ARRAY_PARTITION variable=x_reg complete dim=0
#pragma HLS ARRAY_PARTITION variable=sum_reg complete dim=0

    Init_X_Reg: for(int q = 0; q < LIMB_Q; ++q){
#pragma HLS UNROLL
        for(int p = 0; p <= MAX_OUT_COLS; ++p){
#pragma HLS UNROLL
            x_reg[q][p] = 0;
        }
    }

    Init_Sum_Reg: for(int p = 0; p < MAX_OUT_COLS; ++p){
#pragma HLS UNROLL
        for(int q = 0; q <=LIMB_Q; ++q){
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

    Systolic_Loop: for (int t = 0; t < TOTAL_CYCLES; ++t){
#pragma HLS PIPELINE II=1

        ap_uint<64> x_curr[LIMB_Q][MAX_OUT_COLS + 1];
        ap_uint<128> sum_curr[MAX_OUT_COLS][LIMB_Q + 1];
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
                x_reg[q][0] = in_x[q][row][col];
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
                in_x[LIMB_Q + p][row][col] = (uint64_t)(result);
                
                valid_count[p]++;
            }
        }
    }
    
}

void Compute_BConvNew(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
        const uint64_t in_w[LIMB_Q][MAX_OUT_COLS], 
        const uint64_t out_mod[MAX_OUT_COLS],
        int sizeP
){

    std::cout << "New Operation is working!" << std::endl;
    
    //in_x *= in_x;
    std::cout << "reuslt of in_x *= in_x: " << in_x[0][0][0] << std::endl;

    sizeP = in_x[0][0][0] +1;
    std::cout << "sizeP is set to: " << sizeP << std::endl;

    std::cout << "New Operation finished!" << std::endl;
}