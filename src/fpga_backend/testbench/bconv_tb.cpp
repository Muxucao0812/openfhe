#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ap_int.h>

// 假设你的宏定义在 bconv.h 中，如果没有，请确保它们被正确包含
// 为了方便阅读，这里列出假设的宏大小
// #define SQRT 8
// #define RING_DIM (SQRT * SQRT)
// #define LIMB_Q 3
// #define MAX_OUT_COLS 4
// #define MAX_LIMBS 10

#include "../include/bconv.h" 

using namespace std;

// =================================================================
// 软件黄金模型 (Golden Model) - 纯数学计算，没有任何时序逻辑
// =================================================================
void software_bconv_golden(
    uint64_t in_x[MAX_LIMBS][SQRT][SQRT], 
    const uint64_t in_w[LIMB_Q][MAX_OUT_COLS], 
    const uint64_t out_mod[MAX_OUT_COLS],
    int sizeP,
    uint64_t golden_out[MAX_OUT_COLS][SQRT][SQRT] // 存放正确答案
) {
    // 遍历每一个输出列 (Base P 的每一个 Limb)
    for (int p = 0; p < sizeP; ++p) {
        uint64_t mod_p = out_mod[p];
        
        // 遍历整个多项式环的每一个元素
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                
                ap_uint<128> sum = 0;
                // MAC 操作：累加输入 Limb_Q
                for (int q = 0; q < LIMB_Q; ++q) {
                    ap_uint<128> term = (ap_uint<128>)in_x[q][r][c] * (ap_uint<128>)in_w[q][p];
                    sum = (sum + term) % mod_p;
                }
                golden_out[p][r][c] = (uint64_t)sum;
            }
        }
    }
}

// =================================================================
// 主测试函数
// =================================================================
int main() {
    cout << "===========================================" << endl;
    cout << "  Starting BConv Systolic Array Testbench  " << endl;
    cout << "===========================================" << endl;

    // 初始化随机数种子
    srand(12345);

    // 1. 分配内存 (使用堆内存防止栈溢出)
    typedef uint64_t (*ArrayX)[SQRT][SQRT];
    typedef uint64_t (*ArrayOut)[SQRT][SQRT]; // 新增：为输出数组定义类型

    ArrayX hw_in_x = new uint64_t[MAX_LIMBS][SQRT][SQRT];
    ArrayX sw_in_x = new uint64_t[MAX_LIMBS][SQRT][SQRT];
    ArrayOut golden_out = new uint64_t[MAX_OUT_COLS][SQRT][SQRT];

    uint64_t in_w[LIMB_Q][MAX_OUT_COLS];
    uint64_t out_mod[MAX_OUT_COLS];
    uint64_t out_k_half[MAX_OUT_COLS];
    uint64_t out_m_barrett[MAX_OUT_COLS];
    
    // 假设本次测试需要输出 3 列
    int test_sizeP = 2; 

    // 2. 生成随机测试激励 (Stimulus)
    // 随机生成 out_mod (使用常见的 60-bit 左右的素数大小进行模拟)
    for (int p = 0; p < MAX_OUT_COLS; ++p) {
        // 为了防止 % 0 报错，给一个非零的随机模数
        out_mod[p] = (rand() % 100000) + 10000; 
        uint64_t mod = out_mod[p];
        int k_half = 64 - __builtin_clzll(mod);
        unsigned __int128 m_val = ((unsigned __int128)1 << (k_half + 1)) / mod;
        out_k_half[p] = (uint64_t)k_half;
        out_m_barrett[p] = (uint64_t)m_val;
    }

    // 随机生成权重矩阵 w
    for (int q = 0; q < LIMB_Q; ++q) {
        for (int p = 0; p < MAX_OUT_COLS; ++p) {
            in_w[q][p] = rand() % out_mod[p]; // 权重通常小于模数
        }
    }

    // 随机生成输入数据 x
    for (int l = 0; l < LIMB_Q; ++l) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                uint64_t val = rand() % 0xFFFFFFFF; // 随机 32-bit/64-bit 数据
                hw_in_x[l][r][c] = val;
                sw_in_x[l][r][c] = val; // 软硬件输入保持一致
            }
        }
    }

    // 3. 运行软件黄金模型
    cout << "-> Running Software Golden Model..." << endl;
    software_bconv_golden(sw_in_x, in_w, out_mod, test_sizeP, golden_out);

    // 4. 运行硬件 HLS 模块
    cout << "-> Running Hardware HLS Module..." << endl;
    Compute_BConv(hw_in_x, in_w, out_mod, out_k_half, out_m_barrett, test_sizeP);

    // 5. 对比验证 (Validation)
    cout << "-> Comparing Results..." << endl;
    int error_count = 0;

    for (int p = 0; p < test_sizeP; ++p) {
        for (int r = 0; r < SQRT; ++r) {
            for (int c = 0; c < SQRT; ++c) {
                // 注意：硬件模块的结果是追加在 LIMB_Q 后面的
                uint64_t hw_result = hw_in_x[LIMB_Q + p][r][c];
                uint64_t sw_result = golden_out[p][r][c];

                if (hw_result != sw_result) {
                    if (error_count < 10) { // 只打印前 10 个错误
                        cout << "Mismatch at [p=" << p << "][r=" << r << "][c=" << c << "]: "
                             << "HW = " << hw_result << ", SW = " << sw_result << endl;
                    }
                    error_count++;
                }
            }
        }
    }

    // 6. 输出最终结果
    delete[] hw_in_x;
    delete[] sw_in_x;
    delete[] golden_out; // 【新增释放】

    if (error_count == 0) {
        cout << "===========================================" << endl;
        cout << "  [SUCCESS] All results match perfectly!   " << endl;
        cout << "===========================================" << endl;
        return 0; // 返回 0 表示 C Simulation 通过
    } else {
        cout << "===========================================" << endl;
        cout << "  [FAILED] Total errors: " << error_count << endl;
        cout << "===========================================" << endl;
        return 1; // 返回非 0 表示 C Simulation 失败
    }
}