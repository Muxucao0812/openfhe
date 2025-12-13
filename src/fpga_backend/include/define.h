#ifndef DEFINE_H
#define DEFINE_H

#include <cstdint>
#include <complex>
#include <cmath>
#include <array>
#include <algorithm>
#include <ap_int.h>

// =========================================================
// 1. 类型定义
// =========================================================
#ifdef __SYNTHESIS__
  typedef ap_uint<128> uint128_t;
#else
  typedef unsigned __int128 uint128_t;
#endif

// =========================================================
// 2. 核心维度参数
// =========================================================
static const int RING_DIM = 1 << 12;  // 4096
static const int SQRT = 1 << 6;       // 64
static const int LOG_SQRT = 6;

static const int BU_NUM = 32;

static const int LIMB_Q = 3;
static const int LIMB_P = 2;

static const int STAGE = 12;

#define MAX_LIMBS (LIMB_Q + LIMB_P)

// 兼容旧代码的别名 (如果你的代码里混用了 LIMB)
static const int LIMB = MAX_LIMBS;

// =========================================================
// 3. 操作码定义 (Top 函数的 switch 需要这些)
// =========================================================
#define OP_INIT   0
#define OP_ADD    1
#define OP_SUB    2
#define OP_MULT   3
#define OP_NTT    4
#define OP_INTT   5
#define OP_AUTO   6

// =========================================================
// 4. 辅助常量
// =========================================================
static const int K_LIST[SQRT] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15
    // ... 请确保这里填满或者逻辑正确
};

#endif // DEFINE_H