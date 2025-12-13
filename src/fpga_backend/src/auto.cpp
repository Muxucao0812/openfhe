// #include "../include/auto.h"
// void Auto(
//     uint64_t input[SQRT][SQRT],
//     const int r,
//     uint64_t output[SQRT][SQRT],
//     const int mod_index
// ) {
// #pragma HLS ARRAY_PARTITION variable=input  complete dim=2
// #pragma HLS ARRAY_PARTITION variable=output complete dim=2

//     const int N_2 = RING_DIM << 1;
//     const int mask_N_2 = N_2 - 1;
//     const int mask_N = RING_DIM - 1;
//     const int M = SQRT;
//     const int LOG_M = LOG_SQRT;
//     const int mask_M = M - 1;

//     uint64_t k   = K_LIST[r];
//     uint64_t mod = MODULUS[mod_index];

//     int col_idx[M];
//     int row_idx[M];

// init_index_list:
//     for (int i = 0; i < M; ++i) {
//     #pragma HLS PIPELINE
//         col_idx[i] = (i * k) & mask_N_2;
//         row_idx[i] = (col_idx[i] * M) & mask_N_2;
//     }

//     // 告诉 HLS：output 不存在跨迭代依赖（没有 write-after-write / write-after-read 冲突）
// #pragma HLS DEPENDENCE variable=output inter false
// #pragma HLS DEPENDENCE variable=output intra false

// auto_outer_loop:
//     for (int i = 0; i < M; ++i) {
//     #pragma HLS PIPELINE  // one row per cycle (if resources允许)

//     auto_inner_loop:
//         for (int j = 0; j < M; ++j) {
//         #pragma HLS UNROLL

//             uint64_t val = input[i][j];

//             int row_term = row_idx[i];
//             int col_term = col_idx[j];

//             int new_index_1d_raw = (row_term + col_term) & mask_N_2;

//             int  new_index_1d = new_index_1d_raw & mask_N;
//             bool is_neg       = (new_index_1d_raw >= RING_DIM);

//             uint64_t t          = mod - val;
//             uint64_t signed_val = is_neg ? t : val;

//             int new_row = new_index_1d >> LOG_M;
//             int new_col = new_index_1d & mask_M;

//             output[new_row][new_col] = signed_val;
//         }
//     }
// }