#ifndef LOAD_H
#define LOAD_H
#include <cstddef>
#include <cstdint>
#include "define.h"



void Load(
    const uint64_t *src_ddr,
    uint64_t dest_local[MAX_LIMBS][SQRT][SQRT],
    int num_active_limbs
);

void Store(
    const uint64_t source_local[MAX_LIMBS][SQRT][SQRT],
    uint64_t *dest_ddr,
    int num_active_limbs
);
#endif // LOAD_H