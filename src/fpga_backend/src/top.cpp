#include "../include/top.h"
#include "../include/load.h"
#include "../include/arithmetic.h"
#include "../include/ntt_kernel.h"
#include "../include/interleave.h"
#include "../include/mod_mult_kernel.h"
#include "../include/mod_add_kernel.h"
#include "../include/mod_sub_kernel.h"
#include "../include/bconv.h"
#include "../include/auto.h"



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
static uint64_t NTTTwiddleFactor[MAX_LIMBS][RING_DIM];
static uint64_t INTTTwiddleFactor[MAX_LIMBS][RING_DIM];

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
    #pragma HLS INTERFACE m_axi port=mem_out  offset=slave bundle=gmem2

    #pragma HLS INTERFACE s_axilite port=mem_in1  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_in2  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_out  bundle=control
    #pragma HLS INTERFACE s_axilite port=opcode    bundle=control
    #pragma HLS INTERFACE s_axilite port=num_active_limbs bundle=control
    #pragma HLS INTERFACE s_axilite port=mod_index bundle=control
    #pragma HLS INTERFACE s_axilite port=return    bundle=control


    #pragma HLS ARRAY_PARTITION variable=poly_buffer_1 cyclic dim=3 factor=SQRT
    #pragma HLS ARRAY_PARTITION variable=poly_buffer_2 cyclic dim=3 factor=SQRT
    #pragma HLS ARRAY_PARTITION variable=result_buffer cyclic dim=3 factor=SQRT

    #pragma HLS BIND_STORAGE variable=poly_buffer_1 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=poly_buffer_2 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=result_buffer type=ram_2p impl=bram

    switch(opcode) {
        case OP_INIT: {
            std::cout << "[FPGA] Initializing Modulus Parameters..." << std::endl;
            
            // 简单布局：Q模数在索引0,1,2，P模数在索引3,4
            // 无padding，与Host端一致
            
            init_Q_MOD:
            for (int i = 0; i < LIMB_Q; i++){
                #pragma HLS PIPELINE II=1
                MODULUS[i] = mem_in1[i];
            }
            init_Q_KHALF:
            for (int i = 0; i < LIMB_Q; i++){
                #pragma HLS PIPELINE II=1
                K_HALF[i] = mem_in1[LIMB_Q + i];
            }
            init_Q_M:
            for (int i = 0; i < LIMB_Q; i++){
                #pragma HLS PIPELINE II=1
                M[i] = mem_in1[LIMB_Q*2 + i];
            }
            init_P_Loop:
            for (int j = 0; j < LIMB_P; j++){
                // P模数从索引LIMB_Q开始，即索引3,4
                int idx = LIMB_Q + j;
                MODULUS[idx] = mem_in2[j];
                K_HALF[idx] = mem_in2[LIMB_P + j];
                M[idx] = mem_in2[LIMB_P*2 + j];
                
                #ifndef __SYNTHESIS__
                std::cout << "[FPGA Init] P[" << j << "] (idx=" << idx << "): MOD=" << MODULUS[idx] 
                          << ", K=" << K_HALF[idx] << ", M=" << M[idx] << std::endl;
                #endif
            }
            // mem_in1 布局: [MODULUS×LIMB_Q] [K_HALF×LIMB_Q] [M×LIMB_Q]
            //               [NTT_TF : MAX_LIMBS × RING_DIM]   ← Host 只传 RING_DIM 个/limb
            // mem_in2 布局: [MODULUS×LIMB_P] [K_HALF×LIMB_P] [M×LIMB_P]
            //               [INTT_TF: MAX_LIMBS × RING_DIM]   ← 同上
            //
            // BU_NUM 是 FPGA 内部并行度，Host 不感知；
            // 加载时将每 limb 的 RING_DIM 个 TF 广播给所有 BU。
            static const int NTT_TF_BASE  = LIMB_Q * 3;   // mem_in1 中 NTT_TF 起始偏移
            static const int INTT_TF_BASE = LIMB_P * 3;   // mem_in2 中 INTT_TF 起始偏移

            init_NTTTwiddle_Loop:
            for (int l = 0; l < MAX_LIMBS; l++){
                for (int t = 0; t < RING_DIM; t++){
                    NTTTwiddleFactor[l][t] = mem_in1[NTT_TF_BASE + l * RING_DIM + t];
                }
            }
            init_INTTTwiddle_Loop:
            for (int l = 0; l < MAX_LIMBS; l++){
                for (int t = 0; t < RING_DIM; t++){
                    INTTTwiddleFactor[l][t] = mem_in2[INTT_TF_BASE + l * RING_DIM + t];
                }
            }
            break;
        } 
            
        case OP_ADD:
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            Load(mem_in2, poly_buffer_2, num_active_limbs, mod_index);
            Compute_Add(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, num_active_limbs, mod_index);
            Store(result_buffer, mem_out, num_active_limbs, mod_index);
            break;

        case OP_SUB:
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            Load(mem_in2, poly_buffer_2, num_active_limbs, mod_index);
            Compute_Sub(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, num_active_limbs, mod_index);
            Store(result_buffer, mem_out, num_active_limbs, mod_index);
            break;

        case OP_MULT:
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            Load(mem_in2, poly_buffer_2, num_active_limbs, mod_index);
            Compute_Mult(poly_buffer_1, poly_buffer_2, result_buffer, MODULUS, K_HALF, M, num_active_limbs, mod_index);
            Store(result_buffer, mem_out, num_active_limbs, mod_index);
            break;


        case OP_NTT:
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            for (int l = 0; l < MAX_LIMBS; l++){
                InterLeave(poly_buffer_1[l], true);
            }
            Compute_NTT(poly_buffer_1, NTTTwiddleFactor, INTTTwiddleFactor, MODULUS, K_HALF, M, true, num_active_limbs, mod_index);
            Store(poly_buffer_1, mem_out, num_active_limbs, mod_index);
            break;

        case OP_INTT:
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            Compute_NTT(poly_buffer_1, NTTTwiddleFactor, INTTTwiddleFactor, MODULUS, K_HALF, M, false, num_active_limbs, mod_index);
            for (int l = 0; l < MAX_LIMBS; l++){
                InterLeave(poly_buffer_1[l], false);
            }
            Store(poly_buffer_1, mem_out, num_active_limbs, mod_index);
            break;

    
        case OP_BCONV: {
            // num_active_limbs = sizeP (输出列数)
            int sizeP = num_active_limbs;
            
            // Load Q limbs (输入) 到 poly_buffer_1[0..LIMB_Q-1]
            Load(mem_in1, poly_buffer_1, LIMB_Q, 0);
            
            // mem_in2布局: [权重矩阵 LIMB_Q*MAX_OUT_COLS] [输出模数 MAX_OUT_COLS]
            // 权重矩阵: in_w[q][p] = mem_in2[q * MAX_OUT_COLS + p]
            // 输出模数: out_mod[p] = mem_in2[LIMB_Q * MAX_OUT_COLS + p]
            
            static uint64_t in_w[LIMB_Q][MAX_OUT_COLS];
            for (int q = 0; q < LIMB_Q; q++){
                for (int p = 0; p < MAX_OUT_COLS; p++){
                    in_w[q][p] = mem_in2[q * MAX_OUT_COLS + p];
                }
            }
            
            static uint64_t out_mod[MAX_OUT_COLS];
            static uint64_t out_k_half[MAX_OUT_COLS];
            static uint64_t out_m_barrett[MAX_OUT_COLS];
            int mod_offset = LIMB_Q * MAX_OUT_COLS;
            int khalf_offset = mod_offset + MAX_OUT_COLS;
            int m_offset     = khalf_offset + MAX_OUT_COLS;
            for (int p = 0; p < MAX_OUT_COLS; p++){
                out_mod[p]      = mem_in2[mod_offset + p];
                out_k_half[p]   = mem_in2[khalf_offset + p];
                out_m_barrett[p]= mem_in2[m_offset + p];
            }
            
            #ifndef __SYNTHESIS__
            std::cout << "[BCONV] sizeP=" << sizeP << std::endl;
            for (int p = 0; p < sizeP; p++) {
                std::cout << "  out_mod[" << p << "] = " << out_mod[p] << ", k_half=" << out_k_half[p] << ", m_barrett=" << out_m_barrett[p] << std::endl;
            }
            #endif
            // 计算 BConv, 结果写到 poly_buffer_1[LIMB_Q..LIMB_Q+sizeP-1]
            Compute_BConv(poly_buffer_1, in_w, out_mod, out_k_half, out_m_barrett, sizeP);
            
            // Store sizeP limbs (输出)
            for (int l = 0; l < sizeP; l++) {
                for (int i = 0; i < SQRT; i++) {
                    for (int j = 0; j < SQRT; j++) {
                        mem_out[l * RING_DIM + i * SQRT + j] = poly_buffer_1[LIMB_Q + l][i][j];
                    }
                }
            }
            break;
        }

        case OP_AUTO: {
            // mem_in1 = polynomial (all limbs), mem_in2 = [k, kinv] (two uint64_t)
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
            uint32_t k    = (uint32_t)mem_in2[0];
            uint32_t kinv = (uint32_t)mem_in2[1];
            Compute_Auto(poly_buffer_1, k, kinv, result_buffer, MODULUS, num_active_limbs, mod_index);
            Store(result_buffer, mem_out, num_active_limbs, mod_index);
            break;
        }

        default:
            std::cout << "[FPGA] Unknown opcode: " << opcode << std::endl;
            break;
    }
}