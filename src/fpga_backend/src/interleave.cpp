#include "../include/interleave.h"

// In-Place Interleave (原地交织)
// 输入: data (既是源，也是目的)
// 逻辑: DataRAM -> (Shift) -> Temp_RAM -> DataRAM
void InterLeave(
    uint64_t data[SQRT][SQRT],   // 读写同一个片上 BRAM
    const bool is_right_shift
) {
    #pragma HLS INLINE off

    // =========================================================
    // 1. 定义临时缓存 (Ping-Pong Buffer)
    // =========================================================
    // 我们必须先把结果算到这里，防止“读写冲突”覆盖了还需要用的旧数据
    uint64_t temp_buffer[SQRT][SQRT];

    // 绑定到 BRAM (如果 SQRT 很大导致 BRAM 不够，可改用 uram)
    #pragma HLS BIND_STORAGE variable=temp_buffer type=ram_2p impl=bram

    // 关键：内存切分 (16路并行)，必须与外部传入的 data 的切分方式一致
    #pragma HLS ARRAY_PARTITION variable=temp_buffer cyclic factor=SQRT dim=2
    
    // 同时也声明对外部接口的期望 (告诉 HLS 外部传进来的也是切分好的)
    #pragma HLS ARRAY_PARTITION variable=data cyclic factor=SQRT dim=2

    const int mask = SQRT - 1; 

    // =========================================================
    // 2. Compute 阶段: Data -> Temp (执行移位)
    // =========================================================
    
    if (is_right_shift) {
        Shift_Right_Row:
        for (int i = 0; i < SQRT; ++i) {
            #pragma HLS PIPELINE II=1
            
            Shift_Right_Col:
            for (int k = 0; k < SQRT; ++k) {
                #pragma HLS UNROLL factor=SQRT

                // 逻辑：Output-Driven
                // 目标位置 k 的数据，来自源位置 (k - i)
                int src_j = (k - i + SQRT) & mask;
                
                // 读源 BRAM -> 写临时 BRAM
                temp_buffer[i][k] = data[i][src_j];
            }
        }
    } else {
        Shift_Left_Row:
        for (int i = 0; i < SQRT; ++i) {
            #pragma HLS PIPELINE II=1
            
            Shift_Left_Col:
            for (int k = 0; k < SQRT; ++k) {
                #pragma HLS UNROLL factor=SQRT

                // 目标位置 k 的数据，来自源位置 (k + i)
                int src_j = (k + i) & mask;
                
                temp_buffer[i][k] = data[i][src_j];
            }
        }
    }

    // =========================================================
    // 3. WriteBack 阶段: Temp -> Data (写回原处)
    // =========================================================
    // 数据已经安全地在 Temp 里排好序了，现在覆盖回原 BRAM
    Write_Back_Row:
    for(int i = 0; i < SQRT; i++) {
        #pragma HLS PIPELINE II=1
        
        Write_Back_Col:
        for(int j = 0; j < SQRT; j++) {
            #pragma HLS UNROLL factor=SQRT
            // 简单的 1-to-1 拷贝，带宽拉满
            data[i][j] = temp_buffer[i][j];
        }
    }
}