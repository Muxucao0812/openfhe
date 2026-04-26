#include "../include/top.h"
#include "../include/load.h"
#include "../include/arithmetic.h"
#include "../include/ntt_kernel.h"
#include "../include/cg_ntt.h"
#include "../include/interleave.h"
#include "../include/mod_mult_kernel.h"
#include "../include/mod_add_kernel.h"
#include "../include/mod_sub_kernel.h"
#include "../include/bconv.h"
#include "../include/bconv_systolic.h"
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
// Store the CG-NTT TwiddleFactor
// CG-NTT 每 limb 需要 STAGE × CG_HALF_N = 12 × 2048 = 24576 个旋转因子
// （相比标准 NTT 的 RING_DIM=4096，扩容 6 倍，但省去 PE_PARALLEL 副本）
// 绑定到 URAM（U55C 有 960 块 URAM ≈ 34MB，远大于 BRAM）
// ------------------------
static uint64_t NTTTwiddleFactor[MAX_LIMBS][STAGE][CG_HALF_N];
static uint64_t INTTTwiddleFactor[MAX_LIMBS][STAGE][CG_HALF_N];

void Top(
    const uint64_t *mem_in1,
    const uint64_t *mem_in2,
    uint64_t *mem_out,
    const uint8_t opcode,
    const int num_active_limbs,
    const int mod_index
){

    // depth = max elements accessed per call:
    //   mem_in1/2: OP_INIT loads NTT/INTT twiddles = 3 + MAX_LIMBS*STAGE*CG_HALF_N = 196617
    //   mem_out:   MAX_LIMBS * RING_DIM = 32768
    #pragma HLS INTERFACE m_axi port=mem_in1  offset=slave bundle=gmem0 depth=196617
    #pragma HLS INTERFACE m_axi port=mem_in2  offset=slave bundle=gmem1 depth=196614
    #pragma HLS INTERFACE m_axi port=mem_out  offset=slave bundle=gmem2 depth=32768

    #pragma HLS INTERFACE s_axilite port=mem_in1  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_in2  bundle=control
    #pragma HLS INTERFACE s_axilite port=mem_out  bundle=control
    #pragma HLS INTERFACE s_axilite port=opcode    bundle=control
    #pragma HLS INTERFACE s_axilite port=num_active_limbs bundle=control
    #pragma HLS INTERFACE s_axilite port=mod_index bundle=control
    #pragma HLS INTERFACE s_axilite port=return    bundle=control


    #pragma HLS ARRAY_PARTITION variable=poly_buffer_1 complete dim=3
    #pragma HLS ARRAY_PARTITION variable=poly_buffer_2 complete dim=3
    #pragma HLS ARRAY_PARTITION variable=result_buffer complete dim=3

    #pragma HLS BIND_STORAGE variable=poly_buffer_1 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=poly_buffer_2 type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=result_buffer type=ram_2p impl=bram

<<<<<<< HEAD
    // Twiddle Factor: PE_PARALLEL 个独立副本，complete dim=2 物理隔离，URAM 存储
    // 注意：rom_1p + uram 在 U55C 上非法；且 OP_INIT 需要写入，必须用 ram 类型
=======
    // CG-NTT Twiddle Factor: [MAX_LIMBS][STAGE][CG_HALF_N]
    // stage 维 complete 展开（共 12 层），CG_HALF_N 维 cyclic=PE_PARALLEL
    // → STAGE 层物理隔离，每层内 8 PE 同时读无 Bank Conflict
    // 注意：rom_1p + uram 在 U55C 上非法；OP_INIT 需要写入，必须用 ram_2p
>>>>>>> 280a70d (perf(fpga): 重构 MultMod/AddMod 流水线并独立 CG-NTT PE 以收敛时序)
    #pragma HLS ARRAY_PARTITION variable=NTTTwiddleFactor complete dim=2
    #pragma HLS ARRAY_PARTITION variable=NTTTwiddleFactor cyclic factor=PE_PARALLEL dim=3
    #pragma HLS ARRAY_PARTITION variable=INTTTwiddleFactor complete dim=2
<<<<<<< HEAD
=======
    #pragma HLS ARRAY_PARTITION variable=INTTTwiddleFactor cyclic factor=PE_PARALLEL dim=3
>>>>>>> 280a70d (perf(fpga): 重构 MultMod/AddMod 流水线并独立 CG-NTT PE 以收敛时序)
    #pragma HLS BIND_STORAGE variable=NTTTwiddleFactor type=ram_2p impl=uram
    #pragma HLS BIND_STORAGE variable=INTTTwiddleFactor type=ram_2p impl=uram

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
            init_P_MOD:
            for (int j = 0; j < LIMB_P; j++){
                #pragma HLS PIPELINE II=1
                MODULUS[LIMB_Q + j] = mem_in2[j];
            }
            init_P_KHALF:
            for (int j = 0; j < LIMB_P; j++){
                #pragma HLS PIPELINE II=1
                K_HALF[LIMB_Q + j] = mem_in2[LIMB_P + j];
            }
            init_P_M:
            for (int j = 0; j < LIMB_P; j++){
                #pragma HLS PIPELINE II=1
                M[LIMB_Q + j] = mem_in2[LIMB_P*2 + j];

                #ifndef __SYNTHESIS__
                int idx = LIMB_Q + j;
                std::cout << "[FPGA Init] P[" << j << "] (idx=" << idx << "): MOD=" << MODULUS[idx]
                          << ", K=" << K_HALF[idx] << ", M=" << M[idx] << std::endl;
                #endif
            }
            // mem_in1 布局: [MODULUS×LIMB_Q] [K_HALF×LIMB_Q] [M×LIMB_Q]
            //               [NTT_TF : MAX_LIMBS × STAGE × CG_HALF_N]
            //               CG-NTT 每 limb 共 24576 个旋转因子，Host 端预计算
            // mem_in2 布局: [MODULUS×LIMB_P] [K_HALF×LIMB_P] [M×LIMB_P]
            //               [INTT_TF: MAX_LIMBS × STAGE × CG_HALF_N]
            static const int CG_TF_SIZE   = STAGE * CG_HALF_N;    // 24576
            static const int NTT_TF_BASE  = LIMB_Q * 3;           // mem_in1 中 NTT_TF 起始偏移
            static const int INTT_TF_BASE = LIMB_P * 3;           // mem_in2 中 INTT_TF 起始偏移

            init_NTTTwiddle_Loop:
            for (int l = 0; l < MAX_LIMBS; l++){
                for (int s = 0; s < STAGE; s++){
                    for (int t = 0; t < CG_HALF_N; t++){
                        #pragma HLS PIPELINE II=1
                        NTTTwiddleFactor[l][s][t] =
                            mem_in1[NTT_TF_BASE + l * CG_TF_SIZE + s * CG_HALF_N + t];
                    }
                }
            }
            init_INTTTwiddle_Loop:
            for (int l = 0; l < MAX_LIMBS; l++){
                for (int s = 0; s < STAGE; s++){
                    for (int t = 0; t < CG_HALF_N; t++){
                        #pragma HLS PIPELINE II=1
                        INTTTwiddleFactor[l][s][t] =
                            mem_in2[INTT_TF_BASE + l * CG_TF_SIZE + s * CG_HALF_N + t];
                    }
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


        case OP_NTT: {
            // CG-NTT 正向变换
            // ① 从 DDR 加载多项式到片上 poly_buffer_1（2D BRAM）
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
<<<<<<< HEAD
            for (int l = mod_index; l < mod_index + num_active_limbs; l++){
                #pragma HLS LOOP_TRIPCOUNT min=1 max=5 avg=3
                InterLeave(poly_buffer_1[l], true);
=======
            // ② 逐 limb 调用 CG-NTT Kernel（全程片上，零额外 DDR 流量）
            //    flatten_2d_to_1d: [SQRT][SQRT] → [RING_DIM]（位截取，0周期）
            //    CG_NTT_Kernel:    12 层完美洗牌蝶形，顺序消费 NTTTwiddleFactor
            //    reshape_1d_to_2d: [RING_DIM] → [SQRT][SQRT]（位截取，0周期）
            //    CG-NTT 天然使用完美洗牌网络，无需 InterLeave
            NTT_CG_LIMB_LOOP:
            for (int l = mod_index; l < mod_index + num_active_limbs; l++){
                #pragma HLS LOOP_TRIPCOUNT min=1 max=5 avg=3
                uint64_t flat[RING_DIM];
                uint64_t flat_out[RING_DIM];
                #pragma HLS ARRAY_PARTITION variable=flat     cyclic factor=PE_PARALLEL dim=1
                #pragma HLS ARRAY_PARTITION variable=flat_out cyclic factor=PE_PARALLEL dim=1
                flatten_2d_to_1d(poly_buffer_1[l], flat);
                CG_NTT_Kernel(flat, flat_out, MODULUS[l], K_HALF[l], M[l],
                              NTTTwiddleFactor[l], true);
                reshape_1d_to_2d(flat_out, poly_buffer_1[l]);
>>>>>>> 280a70d (perf(fpga): 重构 MultMod/AddMod 流水线并独立 CG-NTT PE 以收敛时序)
            }
            // ③ 将结果写回 DDR
            Store(poly_buffer_1, mem_out, num_active_limbs, mod_index);
            break;
        }

        case OP_INTT: {
            // CG-NTT 逆向变换（INTT）
            // ① 从 DDR 加载多项式到片上 poly_buffer_1
            Load(mem_in1, poly_buffer_1, num_active_limbs, mod_index);
<<<<<<< HEAD
            Compute_NTT(poly_buffer_1, NTTTwiddleFactor, INTTTwiddleFactor, MODULUS, K_HALF, M, false, num_active_limbs, mod_index);
            for (int l = mod_index; l < mod_index + num_active_limbs; l++){
                #pragma HLS LOOP_TRIPCOUNT min=1 max=5 avg=3
                InterLeave(poly_buffer_1[l], false);
=======
            // ② 逐 limb 调用 CG-INTT Kernel
            //    INTT 方向：stage 11→0，每层 unshuffle 读写（完美逆洗牌）
            //    无需 InterLeave，CG-NTT 几何结构天然消除显式交叉开关
            INTT_CG_LIMB_LOOP:
            for (int l = mod_index; l < mod_index + num_active_limbs; l++){
                #pragma HLS LOOP_TRIPCOUNT min=1 max=5 avg=3
                uint64_t flat[RING_DIM];
                uint64_t flat_out[RING_DIM];
                #pragma HLS ARRAY_PARTITION variable=flat     cyclic factor=PE_PARALLEL dim=1
                #pragma HLS ARRAY_PARTITION variable=flat_out cyclic factor=PE_PARALLEL dim=1
                flatten_2d_to_1d(poly_buffer_1[l], flat);
                CG_NTT_Kernel(flat, flat_out, MODULUS[l], K_HALF[l], M[l],
                              INTTTwiddleFactor[l], false);
                reshape_1d_to_2d(flat_out, poly_buffer_1[l]);
>>>>>>> 280a70d (perf(fpga): 重构 MultMod/AddMod 流水线并独立 CG-NTT PE 以收敛时序)
            }
            // ③ 将结果写回 DDR
            Store(poly_buffer_1, mem_out, num_active_limbs, mod_index);
            break;
        }

    
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
            static uint64_t out_S[MAX_OUT_COLS];
            static uint64_t out_m_barrett[MAX_OUT_COLS];
            int mod_offset = LIMB_Q * MAX_OUT_COLS;
            int khalf_offset = mod_offset + MAX_OUT_COLS;
            int m_offset     = khalf_offset + MAX_OUT_COLS;
            for (int p = 0; p < MAX_OUT_COLS; p++){
                out_mod[p]      = mem_in2[mod_offset + p];
                out_S[p]        = mem_in2[khalf_offset + p];
                out_m_barrett[p]= mem_in2[m_offset + p];
            }

            #ifndef __SYNTHESIS__
            std::cout << "[BCONV] sizeP=" << sizeP << std::endl;
            for (int p = 0; p < sizeP; p++) {
                std::cout << "  out_mod[" << p << "] = " << out_mod[p] << ", S=" << out_S[p] << ", m_barrett=" << out_m_barrett[p] << std::endl;
            }
            #endif
            // 计算 BConv, 结果写到 poly_buffer_1[LIMB_Q..LIMB_Q+sizeP-1]
            Compute_BConv_Systolic(poly_buffer_1, in_w, out_mod, out_S, out_m_barrett, sizeP);
            
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