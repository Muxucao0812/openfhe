#ifndef BCONV_NAIVE_H
#define BCONV_NAIVE_H

#include <ap_int.h>
#include <hls_stream.h>
#include "define.h"
#include "arithmetic.h"

extern "C" {
    // Naive BConv: 直接三重循环实现，性能较低但结构简单
    void Compute_BConv_Naive(
        uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
        const uint64_t in_w[LIMB_Q][MAX_OUT_COLS], 
        const uint64_t out_mod[MAX_OUT_COLS],
        int sizeP
    );
}

#endif // BCONV_NAIVE_H