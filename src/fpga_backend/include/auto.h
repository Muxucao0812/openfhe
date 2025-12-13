#ifndef AUTO_H
#define AUTO_H

#include <ap_int.h>
#include <hls_stream.h>
#include <iostream>
#include "define.h"

void Auto(
     ap_uint<64> input[SQRT][SQRT],
    const int r,
     ap_uint<64> output[SQRT][SQRT],
    const int mod_index
);

#endif // AUTO_H