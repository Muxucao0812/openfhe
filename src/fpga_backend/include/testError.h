#ifndef TESTERROR_H
#define TESTERROR_H

#include <ap_int.h>
#include <hls_stream.h>
#include <cstdint>
#include <complex>
#include <cmath>
#include <array>
#include <algorithm>

// =========================================================
// 2. 核心维度参数
// =========================================================
static const int RING_DIM = 1 << 12;  // 4096
static const int SQRT = 1 << 6;       // 64
static const int LOG_SQRT = 6;

static const int BU_NUM = 32;


// 维度定义
static const int LIMB_Q = 3;  // Q模数数量，索引 0, 1, 2
static const int LIMB_P = 2;  // P模数数量，索引 3, 4

// BConv Systolic Array维度: LIMB_Q行 × MAX_OUT_COLS列
// MAX_OUT_COLS = LIMB_Q + LIMB_P，可以处理Q→P, P→Q等任意转换
static const int MAX_OUT_COLS = LIMB_Q + LIMB_P;  // 5

static const int STAGE = 12; //log2(RING_DIM)

// 总limb数 = Q + P = 3 + 2 = 5
#define MAX_LIMBS (LIMB_Q + LIMB_P)

// 兼容旧代码的别名 (如果你的代码里混用了 LIMB)
static const int LIMB = MAX_LIMBS;

void testError(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS]
);
#endif // TESTERROR_H
