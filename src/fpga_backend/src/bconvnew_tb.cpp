#include "../include/bconvnew.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

// 如果 bconvnew.h 中没有定义这些宏，请在这里定义，以供 TB 使用
// #define LIMB_Q 3
// #define MAX_OUT_COLS 5
// #define MAX_LIMBS 10
// #define SQRT 4
// #define RING_DIM (SQRT * SQRT)

// 声明硬件顶层函数 (Device Under Test)
void bconv_systolic(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT],
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS],
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP
);

void Compute_BConvNew(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS], 
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP
);

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Starting bconv_systolic Testbench    " << std::endl;
    std::cout << "========================================" << std::endl;

    // --------------------------------------------------------
    // 1. 分配并初始化测试数据
    // --------------------------------------------------------
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT] = {0};
    uint64_t in_w[LIMB_Q][MAX_OUT_COLS] = {0};
    uint64_t out_mod[MAX_OUT_COLS] = {0};
    int sizeP = 2; // 测试目标：转换出 2 个新的列

    // 初始化模数 (使用较小的素数方便验证)
    out_mod[0] = 17;
    out_mod[1] = 19;
    out_mod[2] = 23;
    out_mod[3] = 29;
    out_mod[4] = 31;

    // 初始化权重矩阵 (填入随机测试数据)
    for (int q = 0; q < LIMB_Q; ++q) {
        for (int p = 0; p < MAX_OUT_COLS; ++p) {
            in_w[q][p] = (q + 1) * 10 + p; 
        }
    }

    // 初始化输入多项式系数
    for (int q = 0; q < LIMB_Q; ++q) {
        for (int row = 0; row < SQRT; ++row) {
            for (int col = 0; col < SQRT; ++col) {
                // 生成一些规律的数字，确保不越界
                in_x[q][row][col] = (q * RING_DIM + row * SQRT + col) % 100;
            }
        }
    }

    // --------------------------------------------------------
    // 2. 计算黄金参考模型 (Golden Reference)
    // --------------------------------------------------------
    uint64_t golden_out[MAX_OUT_COLS][SQRT][SQRT] = {0};

    std::cout << "[INFO] Computing Golden Reference..." << std::endl;
    for (int p = 0; p < sizeP; ++p) {
        uint64_t mod = out_mod[p];
        for (int row = 0; row < SQRT; ++row) {
            for (int col = 0; col < SQRT; ++col) {
                
                unsigned __int128 sum = 0; // 使用 128 位防止中间乘加溢出
                for (int q = 0; q < LIMB_Q; ++q) {
                    unsigned __int128 x_val = in_x[q][row][col];
                    unsigned __int128 w_val = in_w[q][p];
                    unsigned __int128 prod = (x_val * w_val) % mod;
                    sum = (sum + prod) % mod;
                }
                golden_out[p][row][col] = (uint64_t)sum;
                
            }
        }
    }

    // --------------------------------------------------------
    // 3. 执行 FPGA 硬件逻辑 (DUT)
    // --------------------------------------------------------
    std::cout << "[INFO] Running bconv_systolic hardware function..." << std::endl;
    bconv_systolic(in_x, in_w, out_mod, sizeP);

    // 顺便测试你写的封装函数
    std::cout << "[INFO] Running Compute_BConvNew function..." << std::endl;
    Compute_BConvNew(in_x, in_w, out_mod, sizeP);

    // --------------------------------------------------------
    // 4. 验证结果 (Validation)
    // --------------------------------------------------------
    int errors = 0;
    std::cout << "[INFO] Validating results..." << std::endl;

    for (int p = 0; p < sizeP; ++p) {
        for (int row = 0; row < SQRT; ++row) {
            for (int col = 0; col < SQRT; ++col) {
                // 注意：硬件代码将第 p 列的结果写回到了 in_x[LIMB_Q + p] 的位置
                uint64_t hw_result = in_x[LIMB_Q + p][row][col];
                uint64_t sw_result = golden_out[p][row][col];

                if (hw_result != sw_result) {
                    errors++;
                    if (errors <= 10) { // 只打印前 10 个错误，防止刷屏
                        std::cout << "Mismatch at Out_Col[" << p << "] Row[" << row << "] Col[" << col << "]: "
                                  << "HW=" << hw_result << ", SW=" << sw_result << std::endl;
                    }
                }
            }
        }
    }

    // --------------------------------------------------------
    // 5. 输出测试结论
    // --------------------------------------------------------
    std::cout << "========================================" << std::endl;
    if (errors == 0) {
        std::cout << "   *** TEST PASSED! ***" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0; // 返回 0 告诉 HLS 工具仿真成功
    } else {
        std::cout << "   *** TEST FAILED! " << errors << " errors found. ***" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1; // 返回非 0 告诉 HLS 工具仿真失败
    }
}