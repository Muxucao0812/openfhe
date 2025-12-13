#ifndef NTT_KERNEL_H
#define NTT_KERNEL_H

#include "define.h"
#include "arithmetic.h"

extern "C" {
    void compute_indices(
        int j, 
        int k, 
        int InputIndex[SQRT],
        int OutputIndex[SQRT]
    );
}

extern "C" {
    void read_data(
        int j,
        int k,
        uint64_t ReadData[SQRT],
        const uint64_t DataRAM[SQRT][SQRT]
    );
}

extern "C" {
    void permutate_data(
        uint64_t ReadData[SQRT],
        uint64_t PermuteData[SQRT],
        int InputIndex[SQRT]
    );
}

extern "C" {
    void generate_twiddle_index(
        int j,
        int k,
        int TwiddleIndex[BU_NUM]
    );
}

extern "C" {
    void permute_twiddle_factors(
        uint64_t TwiddleFactor[BU_NUM],
        const uint64_t NTTTWiddleRAM[BU_NUM][RING_DIM],
        int TwiddleIndex[BU_NUM]
    );
}


extern "C" {
    void compute_core(
        uint64_t PermuteData[SQRT],
        uint64_t TwiddleFactor[BU_NUM],
        uint64_t NTTData[SQRT],
        
        uint64_t modulus,
        uint64_t K_HALF,
        uint64_t M,

        bool is_ntt
    );
}

extern "C" {
    void repermute_data(
        uint64_t NTTData[SQRT],
        int OutputIndex[SQRT],
        uint64_t RepermuteData[SQRT]
    );
}

extern "C" {
    int exact_log2(int x);
}

extern "C" {
    void generate_input_index(
        int stage,
        int address,
        int output_indices[SQRT]
    );
}


extern "C" {
    void generate_output_index(
        int stage,
        int address,
        int output_indices[SQRT]
    );
}

extern "C" {
    void rewrite_data(
        int j,
        int k,
        uint64_t RepermuteData[SQRT],
        uint64_t DataRAM[SQRT][SQRT]
    );
}

extern "C" {
    void Configurable_PE(
        const uint64_t &input1,
        const uint64_t &input2,
        const uint64_t &twiddle_factor,

        uint64_t &res1,
        uint64_t &res2,

        const uint64_t &modulus,
        const uint64_t &K_HALF,
        const uint64_t &M,
        const bool &is_ntt     
    );
}

extern "C" {
    void NTT_Kernel(
        uint64_t in_memory[SQRT][SQRT],
        
        const uint64_t modulus,
        const uint64_t K_HALF,
        const uint64_t M,
        
        const uint64_t ntt_twiddle_memory[BU_NUM][RING_DIM],
        const uint64_t intt_twiddle_memory[BU_NUM][RING_DIM],

        bool is_ntt
    );
}



extern "C" {
    void Compute_NTT(
        // memory for ntt and tf
        uint64_t in_memory[MAX_LIMBS][SQRT][SQRT],
        const uint64_t ntt_twiddle_memory[MAX_LIMBS][BU_NUM][RING_DIM],
        const uint64_t intt_twiddle_memory[MAX_LIMBS][BU_NUM][RING_DIM],

        const uint64_t modulus[MAX_LIMBS],
        const uint64_t K_HALF[MAX_LIMBS],
        const uint64_t M[MAX_LIMBS],

        bool is_ntt,
        
        int num_active_limbs,
        int mod_idx_offset

    );
}

#endif // NTT_KERNEL_H