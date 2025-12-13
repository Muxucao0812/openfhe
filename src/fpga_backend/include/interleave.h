#ifndef SHIFT_H
#define SHIFT_H

#include <cstddef>
#include <cstdint>
#include "define.h"


void InterLeave(
    uint64_t data[SQRT][SQRT],   // 读写同一个片上 BRAM
    const bool is_right_shift
);

#endif // SHIFT_H