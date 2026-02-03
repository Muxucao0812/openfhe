#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include "include/auto.h"
#include "include/define.h"

// -----------------------------------------------------------------------------
// Software reference: naive CKKS automorphism on 1D coefficient vector
// output[o] = ±input[i], where i = (o * kinv) mod 2N, negate when i >= N
// -----------------------------------------------------------------------------
static void Auto_sw(
    const uint64_t* input,
    uint32_t kinv,
    uint64_t* output,
    uint64_t mod,
    int N
) {
    const int N2 = N << 1;

    for (int o = 0; o < N; ++o) {
        uint64_t in_raw = ((uint64_t)o * (uint64_t)kinv) % (uint64_t)N2;
        bool negate = (in_raw >= (uint64_t)N);
        int i = negate ? (int)(in_raw - N) : (int)in_raw;
        uint64_t v = input[i];
        output[o] = negate ? (v == 0 ? 0 : (mod - v)) : v;
    }
}

// Extended GCD: compute x such that (a * x) % m == 1
static uint32_t mod_inverse(uint32_t a, uint32_t m) {
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t t = t0 - q * t1;  t0 = t1; t1 = t;
        int64_t r = r0 - q * r1;  r0 = r1; r1 = r;
    }
    if (r0 != 1) return 0;  // no inverse
    return (uint32_t)((t0 < 0) ? (t0 + m) : t0);
}

int main() {
    const int N = RING_DIM;
    const int N2 = N << 1;
    const int M = SQRT;
    const uint64_t mod = 0xFFFFFFFFFFFFFFC5ULL;  // 64-bit modulus

    uint64_t vec_in[RING_DIM];   // 1D for SW reference
    uint64_t vec_ref[RING_DIM];  // 1D software output
    uint64_t input[SQRT][SQRT];  // 2D for HW
    uint64_t output[SQRT][SQRT]; // 2D HW output

    auto pack_1d_to_2d = [&](const uint64_t* v, uint64_t a[SQRT][SQRT]) {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                a[i][j] = v[i * M + j];
    };
    auto unpack_2d_to_1d = [&](const uint64_t a[SQRT][SQRT], uint64_t* v) {
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                v[i * M + j] = a[i][j];
    };


    for (int i = 0; i < N; ++i) vec_in[i] = (uint64_t)i % mod;
    
    for (int r = 0; r < N/2; r++) {
        pack_1d_to_2d(vec_in, input);
        Auto_sw(vec_in, r, vec_ref, mod, N);
        Auto(input, r, r, output, mod);

        int err = 0;
        for (int i = 0; i < N; ++i) {
            uint64_t hw_val = output[i / M][i % M];
            if (hw_val != vec_ref[i]) err++;
        }
        printf("[Auto Test] Identity (k=%d): HW vs SW %s\n", r, err == 0 ? "PASS" : "FAIL");
        if (err != 0) return -1;
    }
   
}

