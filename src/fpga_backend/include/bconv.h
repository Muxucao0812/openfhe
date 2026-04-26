#ifndef BCONV_H
#define BCONV_H

#include <ap_int.h>
#include <hls_stream.h>
#include "define.h"
#include "arithmetic.h"

// BConv 向量内积阵列: LIMB_Q × MAX_OUT_COLS (3×5) 并行 MultMod + 加法树
// 支持任意基转换：Q→P, P→Q, 等等

// bconv_core: 核心计算（INLINE 到调用方）
// in_x [LIMB_Q][RING_DIM]       —— 只读
// out_x[MAX_OUT_COLS][RING_DIM]  —— 只写
// in_w: 权重 [LIMB_Q × MAX_OUT_COLS]，未使用列填 0
// sizeP: 有效输出列数（1 到 MAX_OUT_COLS）
void bconv_core(
    const uint64_t in_x[LIMB_Q][RING_DIM],
    uint64_t out_x[MAX_OUT_COLS][RING_DIM],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    const uint64_t out_S[MAX_OUT_COLS],
    const uint64_t out_m_barrett[MAX_OUT_COLS],
    int sizeP
);

extern "C" {
    // Compute_BConv: 顶层接口，负责 DDR ↔ 片上 BRAM 搬运 + 调用 bconv_core
    // in_x: Input[0..LIMB_Q-1], Output[LIMB_Q..LIMB_Q+sizeP-1]
    // in_w: 权重 [LIMB_Q × MAX_OUT_COLS]，未使用列填 0
    // out_mod: 各输出列的模数
    // sizeP: 有效输出列数（1 到 MAX_OUT_COLS）
    void Compute_BConv(
        uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
        const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
        const uint64_t out_mod[MAX_OUT_COLS],
        const uint64_t out_S[MAX_OUT_COLS],
        const uint64_t out_m_barrett[MAX_OUT_COLS],
        int sizeP
    );
}

#endif