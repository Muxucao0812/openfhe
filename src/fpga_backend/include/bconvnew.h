#ifndef BOCNVNEW_H
#define BCONVNEW_H

#include <ap_int.h>
#include <hls_stream.h>
#include "define.h"
#include "arithmetic.h"

extern "C" {
    // Compute BConv using systolic array
    // in_x: Input[0..LIMB_Q-1], Output[LIMB_Q..LIMB_Q+sizeP-1]
    // in_w: Weights [LIMB_Q × MAX_OUT_COLS], padded with 0 for unused columns
    // out_mod: Output moduli for each column
    // sizeP: Number of active output columns (1 to MAX_OUT_COLS)
    void Compute_BConvNew(
        uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
        const uint64_t in_w[LIMB_Q][MAX_OUT_COLS], 
        const uint64_t out_mod[MAX_OUT_COLS],
        int sizeP
    );
}

#endif