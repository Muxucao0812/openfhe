#include "../include/testError.h"

void testError(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS]
) {
   ap_uint<64> x[MAX_LIMBS][SQRT][SQRT];
   ap_uint<64> w[LIMB_Q][MAX_OUT_COLS];
#pragma HLS ARRAY_PARTITION variable=x complete dim=1
#pragma HLS ARRAY_PARTITION variable=w complete dim=1
    initial_x: for (int i = 0; i < MAX_LIMBS; i++) {
        for (int j = 0; j < SQRT; j++) {
            for (int k = 0; k < SQRT; k++) {
                x[i][j][k] = in_x[i][j][k];
            }
        }
    }

    initial_w: for (int i = 0; i < LIMB_Q; i++) {
        for (int j = 0; j < MAX_OUT_COLS; j++) {
            w[i][j] = in_w[i][j];
        }
    }
    ap_uint<128> acc[MAX_LIMBS][SQRT][SQRT];
#pragma HLS ARRAY_PARTITION variable=acc complete dim=1
    compute_error: for (int i = 0; i < MAX_LIMBS; i++) {
        for (int j = 0; j < SQRT; j++) {
            for (int k = 0; k < SQRT; k++) {
                acc[i][j][k] = 0;
                for (int l = 0; l < LIMB_Q; l++) {
                    acc[i][j][k] += x[i][j][k] * w[l][j];
                }
                // Here you can store or output the computed error as needed
            }
        }
    }
    store_error: for (int i = 0; i < MAX_LIMBS; i++) {
        for (int j = 0; j < SQRT; j++) {
            for (int k = 0; k < SQRT; k++) {
                // Store or output the computed error for x[i][j][k]
                in_x[i][j][k] = (uint64_t)(acc[i][j][k]);
            }
        }
    }
}
