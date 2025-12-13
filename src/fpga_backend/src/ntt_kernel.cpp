#include "ntt_kernel.h"

void compute_indices(int j, int k, int InputIndex[SQRT], int OutputIndex[SQRT]) {
    generate_input_index(j, k, InputIndex);
    generate_output_index(j, k, OutputIndex);
}



int exact_log2(int x) {
    int result = 0;
    while (x > 1) {
        x >>= 1;
        result++;
    }
    return result;
}

void generate_input_index(int stage, int address, int output_indices[SQRT]) {

    int stage_cnt = (stage < exact_log2(SQRT)) ? stage : stage - exact_log2(SQRT);
    int ramnum_log = exact_log2(SQRT) - 1;
    int dis_log = ramnum_log - stage_cnt;
    int mask1 = (1 << (dis_log + 1)) - 1;
    int mask2 = ~((1 << (dis_log + 1)) - 1) & ((1 << exact_log2(SQRT)) - 1);

    for (int i = 0; i < SQRT; i++) {
        #pragma HLS UNROLL
        int iwire = i;
        int temp2 = (iwire & 1) << dis_log;
        int index = ((iwire & mask2) | temp2 | ((iwire & mask1) >> 1)) + address;
        output_indices[i] = index & (SQRT - 1);
    }
}

void generate_output_index(int stage, int address, int output_indices[SQRT]) {

    int stage_cnt = (stage < exact_log2(SQRT)) ? stage : stage - exact_log2(SQRT);
    int ramnum_log = exact_log2(SQRT) - 1;
    int dis_log = ramnum_log - stage_cnt;
    int mask1 = 1 << dis_log;
    int mask2 = (1 << dis_log) - 1;
    int mask3 = (~((1 << (dis_log + 1)) - 1)) & ((1 << exact_log2(SQRT)) - 1);

    for (int i = 0; i < SQRT; i++) {
        #pragma HLS UNROLL
        int shift_amount = exact_log2(SQRT);
        int mask = (1 << shift_amount) - 1;
        int iwire = (i - address ) & mask;
        int temp2 = (iwire & mask2) << 1;
        int index = (iwire & mask3) | temp2 | ((iwire & mask1) >> dis_log);

        output_indices[i] = index;
    }
}

void read_data(
    int j, 
    int k, 
    uint64_t ReadData[SQRT],
    const uint64_t DataRAM[SQRT][SQRT]
) {
    for (int l = 0; l < SQRT; l++) {
        if (j < (STAGE >> 1)) {
            int ReadAddr = (l - k + SQRT) % (1 << ((STAGE >> 1) - j)) + 
                           (k >> ((STAGE >> 1) - j)) * (SQRT >> j);
            ReadData[l] = DataRAM[ReadAddr][l];
        } else {
            ReadData[l] = DataRAM[k][l];
        }
    }
}

void permutate_data(
    uint64_t ReadData[SQRT], 
    uint64_t PermuteData[SQRT],
    int InputIndex[SQRT]
) {
    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=SQRT
        PermuteData[l] = ReadData[InputIndex[l]];
    }
}

void generate_twiddle_index(int j, int k, int TwiddleIndex[BU_NUM]) {
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=BU_NUM
        if (j < (STAGE >> 1)) {
            TwiddleIndex[l] = (1 << j) - 1 + (k >> ((STAGE >> 1) - j)) + (l >> (STAGE - j - 1));
        } else {
            TwiddleIndex[l] = (1 << j) - 1 + (k << (j - (STAGE >> 1))) + (l >> (STAGE - j - 1));
        }
    }
}   

void permute_twiddle_factors(
    uint64_t TwiddleFactor[BU_NUM], 
    const uint64_t NTTTWiddleRAM[BU_NUM][RING_DIM], 
    int TwiddleIndex[BU_NUM]
) {
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=BU_NUM
        TwiddleFactor[l] = NTTTWiddleRAM[l][TwiddleIndex[l]];
    }
}



void compute_core(
    uint64_t PermuteData[SQRT], 
    uint64_t TwiddleFactor[BU_NUM], 
    uint64_t NTTData[SQRT], 

    uint64_t modulus,
    uint64_t K_HALF,
    uint64_t M,

    bool is_ntt
) {
    int data_pairs_total = SQRT >> 1;
    int pairs_per_pe = data_pairs_total / BU_NUM;
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=BU_NUM
        for (int m = 0; m < pairs_per_pe; m++) {
            
            int global_pair_index = l * pairs_per_pe + m;
            int idx1 = global_pair_index * 2;
            int idx2 = global_pair_index * 2 + 1;
            Configurable_PE(
                PermuteData[idx1],
                PermuteData[idx2], 
                TwiddleFactor[l], 
                NTTData[idx1], 
                NTTData[idx2], 
                modulus, 
                K_HALF, 
                M, 
                is_ntt
            );
        }
    }
}

void repermute_data(
    uint64_t NTTData[SQRT], 
    int OutputIndex[SQRT], 
    uint64_t RepermuteData[SQRT]
) {
    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=SQRT
        RepermuteData[l] = NTTData[OutputIndex[l]];
    }
}

void rewrite_data(
    int j, 
    int k, 
    uint64_t RepermuteData[SQRT],             
    uint64_t DataRAM[SQRT][SQRT]
) {
    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=SQRT
        if (j < (STAGE >> 1)) {
            int WriteAddr = (l - k + SQRT) % (1 << ((STAGE >> 1) - j)) + 
                           (k >> ((STAGE >> 1) - j)) * (SQRT >> j);

            DataRAM[WriteAddr][l] = RepermuteData[l];
        } else {
            DataRAM[k][l] = RepermuteData[l];
        }
    }
}

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
) {
    #pragma HLS INLINE
    uint64_t temp, temp1;
    uint64_t input1_temp = input1;
    uint64_t input2_temp = input2;
    uint64_t res1_temp, res2_temp;



    if (is_ntt) {
        MultMod(input2_temp, twiddle_factor, modulus, M, K_HALF, temp);

        AddMod(input1_temp, temp, modulus, true);
        res1_temp = input1_temp;
        
        input1_temp = input1; 
        AddMod(input1_temp, temp, modulus, false);
        res2_temp = input1_temp;
   

        res1 = res1_temp;
        res2 = res2_temp;
    } else {
        AddMod(input1_temp, input2_temp, modulus, true);
        temp1 = input1_temp;
        
        input1_temp = input1; 
        AddMod(input1_temp, input2_temp, modulus, false);
        res2_temp = input1_temp;
        
       
        res1 = (temp1 >> 1) + ((temp1 & 1) ? ((modulus + 1) >> 1) : 0);
        
        MultMod(res2_temp, twiddle_factor, modulus, M, K_HALF, temp);
        
       
        res2 = (temp >> 1) + ((temp & 1) ? ((modulus + 1) >> 1) : 0);
    } 
}

void NTT_Kernel(
    uint64_t in_memory[SQRT][SQRT],
    
    const uint64_t modulus,
    const uint64_t K_HALF,
    const uint64_t M,
    
    const uint64_t ntt_twiddle_memory[BU_NUM][RING_DIM],
    const uint64_t intt_twiddle_memory[BU_NUM][RING_DIM],

    bool is_ntt
){
    int InputIndex[SQRT], OutputIndex[SQRT];
    int TwiddleIndex[BU_NUM];
    int stage_index;
    uint64_t ReadData[SQRT], PermuteData[SQRT], TwiddleFactor[BU_NUM];
    uint64_t NTTData[SQRT], RepermuteData[SQRT];

    for (int j = 0; j < STAGE; j++){
        for (int k = 0; k < SQRT; k++){
            if (is_ntt){
                stage_index = j;
            }else{
                stage_index = STAGE - 1 - j;
            }
            compute_indices(stage_index, k, InputIndex, OutputIndex);
            read_data(stage_index, k, ReadData, in_memory);
            permutate_data(ReadData, PermuteData, InputIndex);
            generate_twiddle_index(stage_index, k, TwiddleIndex);
            if (is_ntt){
                permute_twiddle_factors(TwiddleFactor, ntt_twiddle_memory, TwiddleIndex);
            }else{
                permute_twiddle_factors(TwiddleFactor, intt_twiddle_memory, TwiddleIndex);
            }
            compute_core(PermuteData, TwiddleFactor, NTTData, modulus, K_HALF, M, is_ntt);
            repermute_data(NTTData, OutputIndex, RepermuteData);
            rewrite_data(stage_index, k, RepermuteData, in_memory);
        }            
    }

}



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

) {
    // for (uint i = 0; i < RING_DIM; i++){

    // }

    for (int l = 0; l < num_active_limbs; l++){
        NTT_Kernel(
            in_memory[l],
    
            modulus[mod_idx_offset + l],
            K_HALF[mod_idx_offset + l],
            M[mod_idx_offset + l],
            ntt_twiddle_memory[mod_idx_offset + l],
            intt_twiddle_memory[mod_idx_offset + l],
            is_ntt
        );
    }
 

}