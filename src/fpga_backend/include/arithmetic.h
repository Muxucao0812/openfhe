#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "define.h"   // 包含项目的全局定义

// is_add: True, ModAdd; False, SubAdd
extern "C"  {
    void AddMod(
        uint64_t &a, 
        const uint64_t &b, 
        const uint64_t &mod, 
        const bool &is_add);
}

extern "C"  {
    void MultMod(
        const uint64_t &a,
        const uint64_t &b, 
        const uint64_t &mod, 
        const uint64_t &m, 
        const uint64_t &k_half, 
        uint64_t       &res_mod
    );
}

extern "C"  {
    void Karatsuba(
        const uint64_t &a, 
        const uint64_t &b, 
        uint128_t &result
    );
}

#endif // ARITHMETIC_H