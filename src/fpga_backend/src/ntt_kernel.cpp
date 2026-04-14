#include "../include/ntt_kernel.h"

void compute_indices(int j, int k, int InputIndex[SQRT], int OutputIndex[SQRT]) {
    #pragma HLS INLINE
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
    #pragma HLS INLINE
    int stage_cnt = (stage < exact_log2(SQRT)) ? stage : stage - exact_log2(SQRT);
    int ramnum_log = exact_log2(SQRT) - 1;
    int dis_log = ramnum_log - stage_cnt;
    int mask1 = (1 << (dis_log + 1)) - 1;
    int mask2 = ~((1 << (dis_log + 1)) - 1) & ((1 << exact_log2(SQRT)) - 1);

    for (int i = 0; i < SQRT; i++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        int iwire = i;
        int temp2 = (iwire & 1) << dis_log;
        int index = ((iwire & mask2) | temp2 | ((iwire & mask1) >> 1)) + address;
        output_indices[i] = index & (SQRT - 1);
    }
}

void generate_output_index(int stage, int address, int output_indices[SQRT]) {
    #pragma HLS INLINE
    int stage_cnt = (stage < exact_log2(SQRT)) ? stage : stage - exact_log2(SQRT);
    int ramnum_log = exact_log2(SQRT) - 1;
    int dis_log = ramnum_log - stage_cnt;
    int mask1 = 1 << dis_log;
    int mask2 = (1 << dis_log) - 1;
    int mask3 = (~((1 << (dis_log + 1)) - 1)) & ((1 << exact_log2(SQRT)) - 1);

    for (int i = 0; i < SQRT; i++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
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
    #pragma HLS INLINE

    // 提前计算移位和掩码，将取模运算转化为位运算 (Bitwise AND)
    int shift_val = (STAGE >> 1) - j;
    int mask = (1 << shift_val) - 1;

    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        if (j < (STAGE >> 1)) {
            // 使用 & mask 替代 % (1 << shift_val)
            int ReadAddr = ((l - k + SQRT) & mask) + (k >> shift_val) * (SQRT >> j);
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
    #pragma HLS INLINE
    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        PermuteData[l] = ReadData[InputIndex[l]];
    }
}

void generate_twiddle_index(int j, int k, int TwiddleIndex[BU_NUM]) {
    #pragma HLS INLINE
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        if (j < (STAGE >> 1)) {
            TwiddleIndex[l] = (1 << j) - 1 + (k >> ((STAGE >> 1) - j)) + (l >> (STAGE - j - 1));
        } else {
            TwiddleIndex[l] = (1 << j) - 1 + (k << (j - (STAGE >> 1))) + (l >> (STAGE - j - 1));
        }
    }
}   

void permute_twiddle_factors(
    uint64_t TwiddleFactor[BU_NUM],
    const uint64_t NTTTWiddleRAM[PE_PARALLEL][RING_DIM],
    int TwiddleIndex[BU_NUM]
) {
    #pragma HLS INLINE
    // PE_PARALLEL 个副本 complete dim=1，每个 PE(l) 访问自己的副本 [l % PE_PARALLEL]
    // 即使 TwiddleIndex 完全相同，也落在不同物理 RAM → 零冲突
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        TwiddleFactor[l] = NTTTWiddleRAM[l % PE_PARALLEL][TwiddleIndex[l]];
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
    #pragma HLS INLINE
    int data_pairs_total = SQRT >> 1;
    int pairs_per_pe = data_pairs_total / BU_NUM;
    for (int l = 0; l < BU_NUM; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
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
    #pragma HLS INLINE
    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        RepermuteData[l] = NTTData[OutputIndex[l]];
    }
}

void rewrite_data(
    int j,
    int k,
    uint64_t RepermuteData[SQRT],
    uint64_t DataRAM[SQRT][SQRT]
) {
    #pragma HLS INLINE

    int shift_val = (STAGE >> 1) - j;
    int mask = (1 << shift_val) - 1;

    for (int l = 0; l < SQRT; l++) {
        #pragma HLS UNROLL factor=PE_PARALLEL
        if (j < (STAGE >> 1)) {
            // 使用 & mask 替代 % (1 << shift_val)
            int WriteAddr = ((l - k + SQRT) & mask) + (k >> shift_val) * (SQRT >> j);
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

    const uint64_t ntt_twiddle_memory[PE_PARALLEL][RING_DIM],
    const uint64_t intt_twiddle_memory[PE_PARALLEL][RING_DIM],

    bool is_ntt
){
    // ============================================================
    // PING-PONG 双缓冲：消除 RAW 依赖
    // ============================================================
    uint64_t buf_A[SQRT][SQRT];
    uint64_t buf_B[SQRT][SQRT];

    // -- ping-pong 缓冲区按列 cyclic 展开，匹配 UNROLL factor
    //    不再使用 complete（避免 64:1 MUX 爆炸）和 BIND_STORAGE（避免与 partition 冲突）
    #pragma HLS ARRAY_PARTITION variable=buf_A cyclic factor=PE_PARALLEL dim=2
    #pragma HLS ARRAY_PARTITION variable=buf_B cyclic factor=PE_PARALLEL dim=2

    // -- in_memory 同样 cyclic factor（与 top.cpp poly_buffer 一致）
    #pragma HLS ARRAY_PARTITION variable=in_memory cyclic factor=PE_PARALLEL dim=2

    // -- Twiddle: 8 个副本 complete dim=1，每个副本独立物理 RAM
    #pragma HLS ARRAY_PARTITION variable=ntt_twiddle_memory complete dim=1
    #pragma HLS ARRAY_PARTITION variable=intt_twiddle_memory complete dim=1

    // ============================================================
    // 临时数组 — 全部 complete 展开（消除 bank conflict）
    // ============================================================
    int InputIndex[SQRT], OutputIndex[SQRT];
    int TwiddleIndex[BU_NUM];
    int stage_index;
    uint64_t ReadData[SQRT], PermuteData[SQRT], TwiddleFactor[BU_NUM];
    uint64_t NTTData[SQRT], RepermuteData[SQRT];

    #pragma HLS ARRAY_PARTITION variable=InputIndex complete
    #pragma HLS ARRAY_PARTITION variable=OutputIndex complete
    #pragma HLS ARRAY_PARTITION variable=TwiddleIndex complete
    #pragma HLS ARRAY_PARTITION variable=ReadData complete
    #pragma HLS ARRAY_PARTITION variable=PermuteData complete
    #pragma HLS ARRAY_PARTITION variable=TwiddleFactor complete
    #pragma HLS ARRAY_PARTITION variable=NTTData complete
    #pragma HLS ARRAY_PARTITION variable=RepermuteData complete

    // ============================================================
    // 初始化：in_memory → buf_A
    // ============================================================
    INIT_ROWS:
    for (int i = 0; i < SQRT; i++) {
        INIT_COLS:
        for (int l = 0; l < SQRT; l++) {
            #pragma HLS UNROLL
            buf_A[i][l] = in_memory[i][l];
        }
    }

    // ============================================================
    // 主循环：12 stages × 64 rows
    // Ping-pong 协议：
    //   偶数 stage (j=0,2,4,...): 读 buf_A, 写 buf_B
    //   奇数 stage (j=1,3,5,...): 读 buf_B, 写 buf_A
    // 读写永远在不同物理数组 → 无 RAW 依赖
    // ============================================================
    STAGE_LOOP:
    for (int j = 0; j < STAGE; j++) {
        ROW_LOOP:
        for (int k = 0; k < SQRT; k++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS DEPENDENCE variable=buf_A inter false
            #pragma HLS DEPENDENCE variable=buf_B inter false

            // -- 计算 stage 索引（NTT 正序，INTT 逆序）
            if (is_ntt) {
                stage_index = j;
            } else {
                stage_index = STAGE - 1 - j;
            }

            // -- Step 1: 计算读写置换索引
            compute_indices(stage_index, k, InputIndex, OutputIndex);

            // -- Step 2: 从源缓冲区读数据
            if ((j & 1) == 0) {
                read_data(stage_index, k, ReadData, buf_A);
            } else {
                read_data(stage_index, k, ReadData, buf_B);
            }

            // -- Step 3: 输入置换
            permutate_data(ReadData, PermuteData, InputIndex);

            // -- Step 4: Twiddle 因子获取
            generate_twiddle_index(stage_index, k, TwiddleIndex);
            if (is_ntt) {
                permute_twiddle_factors(TwiddleFactor, ntt_twiddle_memory, TwiddleIndex);
            } else {
                permute_twiddle_factors(TwiddleFactor, intt_twiddle_memory, TwiddleIndex);
            }

            // -- Step 5: 蝶形运算（8 个 PE × 4 次串行迭代）
            compute_core(PermuteData, TwiddleFactor, NTTData, modulus, K_HALF, M, is_ntt);

            // -- Step 6: 输出置换
            repermute_data(NTTData, OutputIndex, RepermuteData);

            // -- Step 7: 写入目标缓冲区
            if ((j & 1) == 0) {
                rewrite_data(stage_index, k, RepermuteData, buf_B);
            } else {
                rewrite_data(stage_index, k, RepermuteData, buf_A);
            }
        }
    }

    // ============================================================
    // 回写：将结果拷贝回 in_memory
    // STAGE=12 为偶数 → 最后一级 j=11 (奇) 写 buf_A → 结果在 buf_A
    // ============================================================
    WRITEBACK_ROWS:
    for (int i = 0; i < SQRT; i++) {
        WRITEBACK_COLS:
        for (int l = 0; l < SQRT; l++) {
            #pragma HLS UNROLL
            if ((STAGE & 1) == 0) {
                in_memory[i][l] = buf_A[i][l];
            } else {
                in_memory[i][l] = buf_B[i][l];
            }
        }
    }
}



void Compute_NTT(
    // memory for ntt and tf
    uint64_t in_memory[MAX_LIMBS][SQRT][SQRT],
    const uint64_t ntt_twiddle_memory[MAX_LIMBS][PE_PARALLEL][RING_DIM],
    const uint64_t intt_twiddle_memory[MAX_LIMBS][PE_PARALLEL][RING_DIM],

    const uint64_t modulus[MAX_LIMBS],
    const uint64_t K_HALF[MAX_LIMBS],
    const uint64_t M[MAX_LIMBS],

    bool is_ntt,

    int num_active_limbs,
    int mod_idx_offset

) {

    // 匹配 top.cpp 的 cyclic factor（与 UNROLL factor 对齐）
    #pragma HLS ARRAY_PARTITION variable=in_memory cyclic factor=PE_PARALLEL dim=3

    // Twiddle: 8 副本 complete dim=2，每副本独立物理 RAM
    #pragma HLS ARRAY_PARTITION variable=ntt_twiddle_memory complete dim=2
    #pragma HLS ARRAY_PARTITION variable=intt_twiddle_memory complete dim=2

    LIMB_LOOP:
    for (int l = mod_idx_offset; l < mod_idx_offset + num_active_limbs; l++){
        #pragma HLS LOOP_TRIPCOUNT min=1 max=5 avg=3
        NTT_Kernel(
            in_memory[l],
            modulus[l],
            K_HALF[l],
            M[l],
            ntt_twiddle_memory[l],
            intt_twiddle_memory[l],
            is_ntt
        );
    }


}