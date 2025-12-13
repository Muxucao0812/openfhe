#include "../include/arithmetic.h"

void AddMod(
    uint64_t &a,           
    const uint64_t &b,     
    const uint64_t &mod,   
    const bool &is_add     
){
    // #pragma HLS INLINE
    unsigned __int128 temp_res;
    if (is_add) { 
        temp_res = (unsigned __int128)a + b;
        if (temp_res >= mod) {
            temp_res -= mod;
        }
        a = (uint64_t)temp_res;
    } 
    else {
        if (a >= b) {
            a = a - b;
        } else {
            temp_res = (unsigned __int128)a + mod - b;
            a = (uint64_t)temp_res;
        }
    }
}

void Karatsuba(
    const uint64_t &a,      
    const uint64_t &b,      
    uint128_t &result      
){
    const uint64_t MASK_32 = 0xFFFFFFFF;
    
    // 1. 拆分高低位
    uint64_t a_low  = a & MASK_32;
    uint64_t a_high = a >> 32;
    uint64_t b_low  = b & MASK_32;
    uint64_t b_high = b >> 32;

    // 2. 计算三个中间乘积
    uint64_t z0 = a_low * b_low;
    uint64_t z2 = a_high * b_high;
    uint128_t z1 = (uint128_t)(a_low + a_high) * (b_low + b_high);

    // 3. 计算中间项 mid = z1 - z2 - z0
    uint128_t mid = z1 - z2 - z0;

    // 4. 移位拼接
    result = ((uint128_t)z2 << 64) + (mid << 32) + z0;
}


// =================================================
// Barrett Modular Multiplication (纯 DSP 版本)
// =================================================
void MultMod(
    const uint64_t &a,           
    const uint64_t &b,  
    const uint64_t &mod,
    const uint64_t &m,
    const uint64_t &k_half,
    uint64_t &res_mod
  
){
    // #pragma HLS INLINE 
    // 流水线优化：II=1 表示每个时钟周期都能处理一个新的输入
    #pragma HLS PIPELINE II=1


    // 2. 全精度乘法 (Step 0)
    // 直接用 *，HLS 会自动推断使用 DSP48
    uint128_t res_mult = (uint128_t)a * b;

    // 3. Barrett 约减 - 估算商 q
    
    // 提取高位 (相当于原来的位切片)
    uint64_t res_mult_high = (uint64_t)(res_mult >> (k_half - 1));

    // 计算 res_mult_high * m
    // 这里也是直接乘，不用 Karatsuba
    uint128_t res_mult_shift = (uint128_t)res_mult_high * m;

    // 右移得到商 q
    uint64_t q = (uint64_t)(res_mult_shift >> (k_half + 1));
    
    // 4. 计算余数 r = z - q * mod
    // q * mod 只需要低 64 位结果即可，因为减法结果肯定在 64 位范围内
    uint64_t q_times_mod = q * mod;
    
    // 利用无符号溢出特性直接相减，得到正确余数
    uint64_t r = (uint64_t)res_mult - q_times_mod;

    // 5. 最终校正 (Correction)
    // Barrett 算法保证 r < 3 * mod，所以最多减两次
    if (r >= mod) {
        r -= mod;
    }
    if (r >= mod) {
        r -= mod;
    }

    // 写回结果
    res_mod = r;
}