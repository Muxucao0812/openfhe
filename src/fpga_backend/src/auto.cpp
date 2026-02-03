#include "../include/auto.h"

// CKKS automorphism in coefficient domain (OpenFHE poly-impl.h style):
// For each output index o: in_raw = (o * kinv) mod 2N; negate = (in_raw >= N);
// in_idx = in_raw < N ? in_raw : in_raw - N;
// output[o] = negate ? (mod - input[in_idx]) : input[in_idx].

static const int N   = RING_DIM;
static const int N2  = RING_DIM << 1;
static const int M   = SQRT;

// Single-limb Auto: one [SQRT][SQRT] polynomial, one modulus.
void Auto(
    uint64_t input[SQRT][SQRT],
    uint32_t k,
    uint32_t kinv,
    uint64_t output[SQRT][SQRT],
    uint64_t mod
) {
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=input  cyclic factor=M dim=2
#pragma HLS ARRAY_PARTITION variable=output cyclic factor=M dim=2

    (void)k;  // used by host to verify; kinv is sufficient for pull model

ROW:
    for (int row = 0; row < M; ++row) {
    COL:
        for (int col = 0; col < M; ++col) {
#pragma HLS PIPELINE II=1

            int out_idx = row * M + col;
            // in_raw = (out_idx * kinv) mod 2N
            uint64_t in_raw = ((uint64_t)out_idx * (uint64_t)kinv) % (uint64_t)N2;
            bool negate     = (in_raw >= (uint64_t)N);
            int in_idx      = negate ? (int)(in_raw - N) : (int)in_raw;
            int in_row      = in_idx / M;
            int in_col      = in_idx % M;

            uint64_t v = input[in_row][in_col];
            output[row][col] = negate ? (v == 0 ? 0 : (mod - v)) : v;
        }
    }
}

// Limb-parallel: same permutation for all limbs; each limb uses MODULUS[l] for negation.
void Compute_Auto(
    uint64_t input[MAX_LIMBS][SQRT][SQRT],
    uint32_t k,
    uint32_t kinv,
    uint64_t output[MAX_LIMBS][SQRT][SQRT],
    uint64_t MODULUS[MAX_LIMBS],
    int num_active_limbs,
    int mod_index
) {
#pragma HLS INLINE off
#pragma HLS ARRAY_PARTITION variable=input   cyclic factor=M dim=2
#pragma HLS ARRAY_PARTITION variable=output  cyclic factor=M dim=2
#pragma HLS ARRAY_PARTITION variable=MODULUS complete

    (void)k;

ROW:
    for (int row = 0; row < M; ++row) {
    COL:
        for (int col = 0; col < M; ++col) {
#pragma HLS PIPELINE II=1

            int out_idx = row * M + col;
            uint64_t in_raw = ((uint64_t)out_idx * (uint64_t)kinv) % (uint64_t)N2;
            bool negate     = (in_raw >= (uint64_t)N);
            int in_idx      = negate ? (int)(in_raw - N) : (int)in_raw;
            int in_row      = in_idx / M;
            int in_col      = in_idx % M;

        LIMB:
            for (int l = mod_index; l < mod_index + num_active_limbs; ++l) {
#pragma HLS UNROLL
                uint64_t v = input[l][in_row][in_col];
                uint64_t q = MODULUS[l];
                output[l][row][col] = negate ? (v == 0 ? 0 : (q - v)) : v;
            }
        }
    }
}
