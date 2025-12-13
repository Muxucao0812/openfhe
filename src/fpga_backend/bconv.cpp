#include "bconv.h"
#include <hls_stream.h>
#include <iostream>
// ------------------------------------------------
// [CONFIG] 统一运行周期
// ------------------------------------------------
// 总周期 = Skew(Input + Sum) + Sequence + Flush
static const int TOTAL_CYCLES = SIZE_P + SIZE_Q + RING_DIM ;

// =================================================
// Weight Loading
// =================================================
void load_weights(
    const ap_uint<64>* weights_linear,
    ap_uint<64>        local_w[SIZE_Q][SIZE_P]
) {
    for(int i=0; i<SIZE_Q; ++i) {
        for(int j=0; j<SIZE_P; ++j) {
#pragma HLS PIPELINE II=1
            local_w[i][j] = weights_linear[i * SIZE_P + j];
        }
    }
}

// =================================================
// PE: Weight Stationary (ModAdd + ModMult)
// =================================================
// [CHANGE] 新增 mod 参数，内部执行模运算
// =================================================
// PE: Weight Stationary (Output Registered / 打拍)
// =================================================
void PE_WS(
    hls::stream<ap_uint<64>>& x_in,
    hls::stream<ap_uint<64>>& x_out,
    hls::stream<ap_uint<64>>& sum_in,
    hls::stream<ap_uint<64>>& sum_out,
    ap_uint<64>               weight,
    ap_uint<64>               mod,
    int                       row_idx,
    int                       col_idx
) {
#pragma HLS INLINE off

    // [1] 定义寄存器变量 (Registers)
    // 它们必须在 Loop 之外定义，这样才能在迭代之间保持数值
    ap_uint<64> reg_x_out = 0;
    ap_uint<64> reg_sum_out = 0;

    PE_Loop: for (int t = 0; t < TOTAL_CYCLES; ++t) {
#pragma HLS PIPELINE II=1
        
        // [2] 输出阶段 (Output Stage)
        // 先把“上一个周期”计算好并存下来的值送出去
        // 注意：在 t=0 时，这里送出的是初始值 0 (相当于 Reset 后的值)
        x_out.write(reg_x_out);
        sum_out.write(reg_sum_out);

        // [3] 输入阶段 (Input Stage)
        ap_uint<64> x = x_in.read();
        ap_uint<64> sum_prev = sum_in.read();
        
        // [4] 计算阶段 (Compute Stage)
        // 逻辑与之前完全一致
        ap_uint<128> prod = (ap_uint<128>)x * (ap_uint<128>)weight;
        ap_uint<128> sum_temp = (ap_uint<128>)sum_prev + prod;
        ap_uint<64> sum_curr = (ap_uint<64>)(sum_temp);

//         // [DEBUG] 打印 (可选)
// #ifndef __SYNTHESIS__
//         if (x != 0 || sum_prev != 0) {
//              std::cout << "[Time " << t << "] PE(" << row_idx << "," << col_idx << ") "
//                        << " X=" << x 
//                        << " Pre=" << sum_prev 
//                        << " Calc=" << sum_curr 
//                        << " (Registered for next cycle)" << std::endl;
//         }
// #endif

        // [5] 注册阶段 (Register Stage)
        // 不要直接 write sum_curr，而是把它存入寄存器变量
        // 它将在 t+1 时刻被写出
        reg_x_out = x;
        reg_sum_out = sum_curr;
    }
}

// =================================================
// Feeder X (Rows)
// =================================================
void feeder_x(
    const ap_uint<64>* x_linear,
    hls::stream<ap_uint<64>> (&x_grid)[SIZE_Q][SIZE_P + 1]
) {
    Row_Loop: 
    for (int i = 0; i < SIZE_Q; ++i) {
        #pragma HLS UNROLL
        for (int t = 0; t < TOTAL_CYCLES; ++t) {
// #pragma HLS PIPELINE II=1
            ap_uint<64> val = 0;
            
            // Logic: Row i 延迟 i 个周期开始
            if (t >= i && t < (i + RING_DIM)) {
                val = x_linear[(t - i) * SIZE_Q + i];
            }
            x_grid[i][0].write(val);
        }
    }
}

// =================================================
// Feeder Sum (Cols)
// =================================================
void feeder_sum(
    hls::stream<ap_uint<64>> (&sum_grid)[SIZE_P][SIZE_Q + 1]
) {
    Col_Loop: for (int j = 0; j < SIZE_P; ++j) {
        for (int t = 0; t < TOTAL_CYCLES; ++t) {
#pragma HLS PIPELINE II=1
            // 初始部分和为 0
            sum_grid[j][0].write(0); 
        }
    }
}

// =================================================
// Drain X
// =================================================
void drain_x(
    hls::stream<ap_uint<64>> (&x_grid)[SIZE_Q][SIZE_P + 1]
) {
    for(int i=0; i<SIZE_Q; ++i) {
#pragma HLS UNROLL
        for(int t=0; t < TOTAL_CYCLES; ++t) {
#pragma HLS PIPELINE II=1
            x_grid[i][SIZE_P].read();
        }
    }
}

// =================================================
// Collector
// =================================================
void collector(
    hls::stream<ap_uint<64>> (&sum_grid)[SIZE_P][SIZE_Q + 1],
    ap_uint<64>  mod[SIZE_P],
    ap_uint<64>* out_linear
) {
    Col_Out_Loop: 
    for (int j = 0; j < SIZE_P; ++j) {
        #pragma HLS UNROLL
        int valid_count = 0;
        int discard_count = 0;
        
        // 等待时间 = Input Skew (j) + Vertical Latency (SIZE_Q)
        int wait_cycles = SIZE_Q + j;

        for (int t = 0; t < TOTAL_CYCLES; ++t) {
            #pragma HLS PIPELINE II=1
            ap_uint<64> val = sum_grid[j][SIZE_Q].read();
            
            // 1. 丢弃无效的前导数据
            if (discard_count < wait_cycles) {
                discard_count++;
            }
            // 2. 收集有效数据
            else if (valid_count < RING_DIM) {
                // [CHANGE] 不需要再取模了，PE 已经做完了
                out_linear[valid_count * SIZE_P + j] = val % mod[j];
                valid_count++;
            }
        }
    }
}

// =================================================
// Top Level
// =================================================
extern "C" void bconv_systolic(
    const ap_uint<64>* x_linear,    
    const ap_uint<64>* w_linear,    
    const ap_uint<64>* mod_linear,  
    ap_uint<64>* out_linear         
) {
#pragma HLS INTERFACE m_axi port=x_linear   bundle=gmem0
#pragma HLS INTERFACE m_axi port=w_linear   bundle=gmem1
#pragma HLS INTERFACE m_axi port=mod_linear bundle=gmem2
#pragma HLS INTERFACE m_axi port=out_linear bundle=gmem3
#pragma HLS INTERFACE s_axilite port=return bundle=control

    // 缓存配置
    ap_uint<64> local_w[SIZE_Q][SIZE_P];
    ap_uint<64> local_mods[SIZE_P];
#pragma HLS ARRAY_PARTITION variable=local_w complete
#pragma HLS ARRAY_PARTITION variable=local_mods complete

    load_weights(w_linear, local_w);
    // 加载模数到本地缓存
    for(int i=0; i<SIZE_P; i++) {
        #pragma HLS UNROLL
        local_mods[i] = mod_linear[i];
    }

    // Stream 网格
    static hls::stream<ap_uint<64>> x_grid[SIZE_Q][SIZE_P + 1];
    static hls::stream<ap_uint<64>> sum_grid[SIZE_P][SIZE_Q + 1];

    #pragma HLS STREAM variable=x_grid depth=2
    #pragma HLS STREAM variable=sum_grid depth=2

// #pragma HLS DATAFLOW

    feeder_x(x_linear, x_grid);
    feeder_sum(sum_grid);

    // 4x8 PE Array
    PE_Loop:
    Row_Gen: for (int i = 0; i < SIZE_Q; ++i) {
        #pragma HLS UNROLL
        Col_Gen: for (int j = 0; j < SIZE_P; ++j) {
        #pragma HLS UNROLL
            // [CHANGE] 将对应列的模数传入 PE
            PE_WS(
                x_grid[i][j],       
                x_grid[i][j+1],     
                sum_grid[j][i],     
                sum_grid[j][i+1],   
                local_w[i][j],
                local_mods[j],
                i,
                j
            );

        }
    }

    drain_x(x_grid);
    

    collector(sum_grid, local_mods, out_linear);
}