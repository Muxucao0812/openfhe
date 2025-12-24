#include "../include/auto.h"
#include <hls_stream.h>

// 定义数据包结构，方便流水线携带控制信号
struct DataPacket {
    uint64_t val;
    bool     negate;
};

// --------------------------------------------------------
// 硬件原语：通用桶形移位器 (Crossbar / Barrel Shifter)
// --------------------------------------------------------
// 这是一个纯组合逻辑网络，HLS 会将其综合为高效的 MUX 网络
// input:  输入数组
// shift:  移位量 (0 ~ SIZE-1)
// output: 移位后的数组
template<int SIZE, typename T>
void barrel_rotator(const T input[SIZE], int shift, T output[SIZE]) {
    #pragma HLS INLINE
    
    // 这种写法 HLS 会综合成 Log(N) 级的 MUX 网络，非常高效
    for (int i = 0; i < SIZE; ++i) {
        #pragma HLS UNROLL
        // 循环左移逻辑: output[i] = input[(i + shift) % SIZE]
        // 注意：这里使用位运算优化取模 (前提 SIZE 是 2 的幂)
        int src_idx = (i + shift) & (SIZE - 1);
        output[i] = input[src_idx];
    }
}

// --------------------------------------------------------
// 核心模块：Auto (Crossbar 架构)
// --------------------------------------------------------
void Auto(
    uint64_t input[SQRT][SQRT],
    const int r,
    uint64_t output[SQRT][SQRT],
    uint64_t MODULUS[MAX_LIMBS],
    const int mod_index
) {
    // 1. 数组切分：完全并行读写
    // dim=2 表示将每一行的 SQRT 个元素完全打散，提供并行端口
    #pragma HLS ARRAY_PARTITION variable=input  cyclic factor=SQRT dim=2
    #pragma HLS ARRAY_PARTITION variable=output cyclic factor=SQRT dim=2
    
    const int M = SQRT;
    const int LOG_M = LOG_SQRT;
    const int N_2 = RING_DIM << 1;
    const int mask_N_2 = N_2 - 1;

    uint64_t k   = K_LIST[r];
    uint64_t mod = MODULUS[mod_index];

    // 预计算控制信号 (AGU 部分)
    // 只需计算一次，后续流水线复用
    int row_shifts[M];    // 每一行需要进行的列移位量
    int global_row_perm[M]; // 行与行之间的置换模式
    #pragma HLS ARRAY_PARTITION variable=row_shifts complete
    #pragma HLS ARRAY_PARTITION variable=global_row_perm complete

    // 预计算逻辑 (AGU)
    // 可以在 Host 端算好传进来，或者在这里用极短时间算完
calc_ctrl:
    for (int i = 0; i < M; ++i) {
        #pragma HLS PIPELINE
        // 这里的数学逻辑需根据具体 Automorphism 映射推导
        // 假设 Automorphism 导致的是规律性的位移
        // 例如：第 i 行的数据整体移动到了 row_perm[i] 行，且内部移动了 row_shifts[i]
        
        // *注：此处为了演示 Crossbar 结构，使用了简化逻辑*
        // *实际应用中，你需要将 i*k 的映射分解为 行移位 和 列移位*
        int idx_raw = (i * k) & mask_N_2;
        row_shifts[i]      = idx_raw & (M - 1);      // 低位决定列移位
        global_row_perm[i] = (idx_raw >> LOG_M) & (M - 1); // 高位决定行置换
    }

    // --------------------------------------------------------
    // 主流水线 (Main Pipeline)
    // --------------------------------------------------------
    // 我们的目标是 II=1，即每个周期处理一行 (M 个元素)
auto_pipeline:
    for (int i = 0; i < M; ++i) {
        #pragma HLS PIPELINE

        // Stage 1: 并行读取一行数据
        uint64_t row_buffer_in[M];
        #pragma HLS ARRAY_PARTITION variable=row_buffer_in complete
        
        for (int j = 0; j < M; ++j) {
            #pragma HLS UNROLL
            row_buffer_in[j] = input[i][j];
        }

        // Stage 2: 变号检测与预处理 (Element-wise)
        // 计算这一行每个元素的新位置是否越界导致变号
        DataPacket packet_buffer[M];
        #pragma HLS ARRAY_PARTITION variable=packet_buffer complete

        for (int j = 0; j < M; ++j) {
            #pragma HLS UNROLL
            // 计算原始 1D 索引 -> 变换后 1D 索引 -> 判断变号
            // 这里需要根据 r 实时计算，或者预先存好 LUT
            int raw_1d = (i * M + j); 
            int mapped_1d = (raw_1d * k) & mask_N_2;
            
            packet_buffer[j].val = row_buffer_in[j];
            packet_buffer[j].negate = (mapped_1d >= RING_DIM);
        }

        // Stage 3: 列内旋转 (Column Rotation / Intra-row Permutation)
        // 每一行内部的数据进行循环移位。这是一个 1-to-1 Crossbar。
        DataPacket col_rotated[M];
        #pragma HLS ARRAY_PARTITION variable=col_rotated complete
        
        // 使用之前定义的 barrel_rotator
        // 注意：每行移位量可能不同，这里取当前行的移位量
        barrel_rotator<M, DataPacket>(packet_buffer, row_shifts[i], col_rotated);

        // Stage 4: 变号执行 (Negation)
        uint64_t final_row_vals[M];
        #pragma HLS ARRAY_PARTITION variable=final_row_vals complete

        for (int j = 0; j < M; ++j) {
            #pragma HLS UNROLL
            uint64_t v = col_rotated[j].val;
            if (col_rotated[j].negate) {
                final_row_vals[j] = (v == 0) ? 0 : (mod - v);
            } else {
                final_row_vals[j] = v;
            }
        }

        // Stage 5: 行写入 (Row Permutation / Write Back)
        // 将处理好的一整行数据，写入到 output 的目标行
        // 因为我们在开头就计算了 global_row_perm，知道当前第 i 行要去哪里
        int target_row = global_row_perm[i];
        
        for (int j = 0; j < M; ++j) {
            #pragma HLS UNROLL
            output[target_row][j] = final_row_vals[j];
        }
    }
}