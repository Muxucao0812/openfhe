#ifndef TOP_H
#define TOP_H

#include "define.h"
#include "opcode.h"
#include "memory.h"
#include "load.h"
#include "arithmetic.h"
#include "mod_add_kernel.h"
#include "mod_sub_kernel.h"
#include "mod_mult_kernel.h"
#include "ntt_kernel.h"

extern "C" {
    void Top(
        const uint64_t *mem_in1,
        const uint64_t *mem_in2,
        uint64_t *mem_out,
        const uint8_t opcode,
        const int num_active_limbs,
        const int mod_index = 0
    );
}

#endif // TOP_H

