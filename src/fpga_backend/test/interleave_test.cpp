#include "../include/interleave.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip> // 用于打印对齐



void interleave_test() {

    uint64_t input[SQRT][SQRT];
    uint64_t output_right[SQRT][SQRT];
    
    // 初始化 output 为 0，防止有残留数据干扰测试
    for(int i=0; i<SQRT; ++i)
        for(int j=0; j<SQRT; ++j)
            output_right[i][j] = 0;

    // 1. 初始化输入数据 (比如 i*M + j)
    std::cout << "Initializing Input..." << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            input[i][j] = i * SQRT + j;
        }
    }

    // 2. 调用待测函数 (Right Interleave)
    // assuming bool flag: true for right, false for left
    std::cout << "Running InterLeave Kernel..." << std::endl;
    InterLeave(input, output_right, true); 

    // 3. 计算“标准答案” (Expected Result)
    uint64_t expected_right[SQRT][SQRT];
    

    // input(i, j) 移动到 expected(new_i, new_j)
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            int new_i = i;
            int new_j = (i + j) & (SQRT - 1); // 模 SQRT
            
            // 赋值：输入的数据 搬家到 新坐标
            expected_right[new_i][new_j] = input[i][j];
        }
    }

    // 4. 打印矩阵 (方便调试，M太大时可以注释掉)

    std::cout << "\n--- Input Matrix ---" << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            std::cout << std::setw(4) << input[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n--- HW Output (Right) ---" << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            std::cout << std::setw(4) << output_right[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n--- Expected Output ---" << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            std::cout << std::setw(4) << expected_right[i][j] << " ";
        }
        std::cout << std::endl;
    }


    // 5. 自动验证 (Assertion)
    std::cout << "\nVerifying results..." << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            if (output_right[i][j] != expected_right[i][j]) {
                std::cerr << "Mismatch at [" << i << "][" << j << "]: "
                          << "HW=" << output_right[i][j] 
                          << " Expected=" << expected_right[i][j] << std::endl;
                exit(1); // 报错退出
            }
        }
    }


    // Left Interleave Test
    uint64_t output_left[SQRT][SQRT];
    // 初始化 output 为 0，防止有残留数据干扰测试
    for(int i=0; i<SQRT; ++i)
        for(int j=0; j<SQRT; ++j)
            output_left[i][j] = 0;
    InterLeave(input, output_left, false);

    uint64_t expected_left[SQRT][SQRT];
    // input(i, j) 移动到 expected(new_i, new_j)
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            int new_i = i;
            int new_j = (j - i + SQRT) & (SQRT - 1); // 模 SQRT
            // 赋值：输入的数据 搬家到 新坐标
            expected_left[new_i][new_j] = input[i][j];
        }
    }
    // 验证 Left Interleave 结果
    std::cout << "\nVerifying Left InterLeave results..." << std::endl;
    for (int i = 0; i < SQRT; ++i) {
        for (int j = 0; j < SQRT; ++j) {
            if (output_left[i][j] != expected_left[i][j]) {
                std::cerr << "Left Mismatch at [" << i << "][" << j << "]: "
                          << "HW=" << output_left[i][j] 
                          << " Expected=" << expected_left[i][j] << std::endl;
                exit(1); // 报错退出
            }
        }
    }

    std::cout << "✅ InterLeave tests passed!" << std::endl;
}

