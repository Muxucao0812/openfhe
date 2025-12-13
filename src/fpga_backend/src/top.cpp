#include "../include/top.h"
#include "load.cpp"
#include "arithmetic.cpp"
#include "ntt_kernel.cpp"
#include "interleave.cpp"
#include "mod_mult_kernel.cpp"
#include "mod_add_kernel.cpp"
#include "mod_sub_kernel.cpp"



// -------------------------
// Store the Memory
// -------------------------
static uint64_t poly_buffer_1[MAX_LIMBS][SQRT][SQRT];
static uint64_t poly_buffer_2[MAX_LIMBS][SQRT][SQRT];
static uint64_t result_buffer[MAX_LIMBS][SQRT][SQRT];

// -------------------------
// Store the Modulus
// -------------------------
static uint64_t MODULUS[MAX_LIMBS];
static uint64_t K_HALF[MAX_LIMBS];
static uint64_t M[MAX_LIMBS];

// ------------------------
// Store the TwiddleFactor
// ------------------------
static uint64_t NTTTwiddleFactor[MAX_LIMBS][BU_NUM][RING_DIM];
static uint64_t INTTTwiddleFactor[MAX_LIMBS][BU_NUM][RING_DIM];

void Top(
    const uint64_t *mem_in1,
    const uint64_t *mem_in2,
    uint64_t *mem_out,
    const uint8_t opcode,
    const int num_active_limbs,
    const int mod_index
){

    #pragma HLS INTERFACE m_axi port=mem_in1  offset=slave bundle=gmem0
    #pragma HLS INTERFACE m_axi port=mem_in2  offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi port=mem_out  offset=slave bundle=gmem0

    #pragma HLS INTERFACE s_axilite port=mem_in1  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_in2  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_out  bundle=control
    #pragma HLS INTERFACE s_axilite port=opcode    bundle=control
    #pragma HLS INTERFACE s_axilite port=num_active_limbs bundle=control
    #pragma HLS INTERFACE s_axilite port=mod_index bundle=control
    #pragma HLS INTERFACE s_axilite port=return    bundle=control


    #pragma HLS ARRAY_PARTITION variable=poly_buffer_1 cyclic dim=2 factor=SQRT
    #pragma HLS ARRAY_PARTITION variable=poly_buffer_2 cyclic dim=2 factor=SQRT
    #pragma HLS ARRAY_PARTITION variable=result_buffer cyclic dim=2 factor=SQRT

    #pragma HLS BIND_STORAGE variable=poly_buffer_1 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=poly_buffer_2 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=result_buffer type=ram_2p impl=bram

    switch(opcode) {
        case OP_INIT:
            std::cout << "[FPGA] Initializing Modulus Parameters..." << std::endl;
            init_Q_Loop:
            for (int i = 0; i < LIMB_Q; i++){
                MODULUS[i] = mem_in1[i];
                K_HALF[i] = mem_in1[LIMB_Q + i];
                M[i] = mem_in1[LIMB_Q*2 + i];
            }
            init_P_Loop:
            for (int j = 0; j < LIMB_P; j++){
                int idx = LIMB_Q + j;
                MODULUS[idx] = mem_in2[j];
                K_HALF[idx] = mem_in2[LIMB_P + j];
                M[idx] = mem_in2[LIMB_P*2 + j];
            }
            init_NTTTwiddle_Loop:
            for (int l = 0; l < LIMB_Q + LIMB_P; l++){
                int offset = LIMB_Q * 3;
                for (int b = 0; b < BU_NUM; b++){
                    for (int t = 0; t < RING_DIM; t++){
                        NTTTwiddleFactor[l][b][t] = mem_in1[offset + l*RING_DIM + t];
                    }
                }
            }
            init_INTTTwiddle_Loop:
            for (int l = 0; l < LIMB_Q + LIMB_P; l++){
                int offset = LIMB_P * 3;
                for (int b = 0; b < BU_NUM; b++){
                    for (int t = 0; t < RING_DIM; t++){
                        INTTTwiddleFactor[l][b][t] = mem_in2[offset + l*RING_DIM + t];
                    }
                }
            }
            break;
            
        case OP_ADD:
            Load(mem_in1, poly_buffer_1, num_active_limbs);
            Load(mem_in2, poly_buffer_2, num_active_limbs);
            Compute_Add(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, num_active_limbs);
            Store(result_buffer, mem_out, num_active_limbs);
            break;

        case OP_SUB:
            Load(mem_in1, poly_buffer_1, num_active_limbs);
            Load(mem_in2, poly_buffer_2, num_active_limbs);
            Compute_Sub(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, num_active_limbs);
            Store(result_buffer, mem_out, num_active_limbs);
            break;

        case OP_MUL:
            Load(mem_in1, poly_buffer_1, num_active_limbs);
            Load(mem_in2, poly_buffer_2, num_active_limbs);
            Compute_Mult(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, K_HALF, M, num_active_limbs);
            Store(result_buffer, mem_out, num_active_limbs);
            break;


        case OP_NTT:
            Load(mem_in1, poly_buffer_1, num_active_limbs);
            for (int l = 0; l < num_active_limbs; l++){
                l = l + mod_index;
                InterLeave(poly_buffer_1[l], true);
            }
            std::cout << "Starting NTT Compute on FPGA..." << std::endl;
            std::cout << "limbs: " << num_active_limbs << " mod_index: " << mod_index << std::endl;
            std::cout << "First modulus: " << MODULUS[mod_index] << std::endl;
            
            Compute_NTT(poly_buffer_1, NTTTwiddleFactor, INTTTwiddleFactor, MODULUS, K_HALF, M, true, num_active_limbs, mod_index);
            
            std::cout << "Finished NTT Compute on FPGA." << std::endl;
            
            Store(poly_buffer_1, mem_out, num_active_limbs);
            break;

        case OP_INTT:
            Load(mem_in1, poly_buffer_1, num_active_limbs);
            
         
            Compute_NTT(poly_buffer_1, NTTTwiddleFactor, INTTTwiddleFactor, MODULUS, K_HALF, M, false, num_active_limbs, mod_index);
            for (int l = 0; l < num_active_limbs; l++){
                l = l + mod_index;
                InterLeave(poly_buffer_1[l], false);
            }
            Store(poly_buffer_1, mem_out, num_active_limbs);
            break;

    }

                
}