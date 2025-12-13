#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include "bconv.h"

// ------------------------------------------------------------------
// 软件参考模型 (Golden Reference)
// ------------------------------------------------------------------
// 逻辑：标准的矩阵乘法 C = (A x B) % mod
// A (Input X) : [RING_DIM][SIZE_Q] (M x K)
// B (Weight)  : [SIZE_Q][SIZE_P]   (K x N)
// Mod         : [SIZE_P]           (N) - 每一列对应一个模数
// C (Output)  : [RING_DIM][SIZE_P] (M x N)
void bconv_ref(
    const uint64_t* x, 
    const uint64_t* w, 
    const uint64_t* mod, 
    uint64_t* out
) {
    for (int r = 0; r < RING_DIM; ++r) {       // 遍历行 (Time Step / M)
        for (int c = 0; c < SIZE_P; ++c) {     // 遍历列 (Output Channel / N)
            
            // 使用 128 位累加防止溢出
            unsigned __int128 acc = 0;
            
            for (int k = 0; k < SIZE_Q; ++k) { // 遍历中间维 (Input Channel / K)
                // 索引计算: Row-Major
                // x[r][k]
                uint64_t val_x = x[r * SIZE_Q + k];
                // w[k][c]
                uint64_t val_w = w[k * SIZE_P + c];
                
                acc += (unsigned __int128)val_x * (unsigned __int128)val_w;
            }
            
            // 按列取模
            uint64_t current_mod = mod[c];
            out[r * SIZE_P + c] = (uint64_t)(acc % current_mod);
        }
    }
}

// ------------------------------------------------------------------
// 辅助函数：打印矩阵 (仅打印小部分)
// ------------------------------------------------------------------
void print_matrix_head(const char* name, const uint64_t* data, int rows, int cols, int max_rows=16) {
    std::cout << "Matrix " << name << " [" << rows << "x" << cols << "] (First " << max_rows << " rows):\n";
    int limit_r = (rows < max_rows) ? rows : max_rows;
    for (int r = 0; r < limit_r; ++r) {
        for (int c = 0; c < cols; ++c) {
            std::cout << std::setw(6) << data[r * cols + c] << " ";
        }
        std::cout << "\n";
    }
    if (rows > max_rows) std::cout << "...\n";
    std::cout << std::endl;
}

int main() {
    std::cout << "=============================================" << std::endl;
    std::cout << "   Testbench for 2D Weight Stationary Array  " << std::endl;
    std::cout << "   Dims: X[" << RING_DIM << "x" << SIZE_Q << "] * W[" 
              << SIZE_Q << "x" << SIZE_P << "] -> Out[" << RING_DIM << "x" << SIZE_P << "]" << std::endl;
    std::cout << "=============================================" << std::endl;

    // 1. 使用 std::vector 在堆上分配内存 (防止栈溢出)
    std::vector<uint64_t> x_host(RING_DIM * SIZE_Q);
    std::vector<uint64_t> mat_host(SIZE_Q * SIZE_P); // Weight
    std::vector<uint64_t> mod_host(SIZE_P);
    std::vector<uint64_t> out_ref(RING_DIM * SIZE_P);
    std::vector<uint64_t> out_hw(RING_DIM * SIZE_P);

    // 2. 初始化数据
    // 模数: 每一列使用不同的模数，方便验证列独立性
    // e.g., 101, 103, 107...
    for (int j = 0; j < SIZE_P; ++j) {
        mod_host[j] = 100 + j*2 + 1; 
    }

    // 输入 X: 简单的规律数据 + 少量随机
    for (int i = 0; i < RING_DIM * SIZE_Q; ++i) {
        // x_host[i] = (i % 100) + 1; // 简单模式
        x_host[i] = rand() % 100;     // 随机模式
    }

    // 权重 W
    for (int i = 0; i < SIZE_Q * SIZE_P; ++i) {
        mat_host[i] = (i % 50) + 1; 
    }

    // 3. 运行软件参考模型
    std::cout << "Running Reference Model..." << std::endl;
    bconv_ref(x_host.data(), mat_host.data(), mod_host.data(), out_ref.data());

    // 4. 运行硬件 Kernel (HLS)
    std::cout << "Running Hardware Kernel..." << std::endl;
    
    // 需要强制转换为 ap_uint<64>* 接口
    // std::vector.data() 返回底层指针
    ap_uint<64>* x_dev   = reinterpret_cast<ap_uint<64>*>(x_host.data());
    ap_uint<64>* mat_dev = reinterpret_cast<ap_uint<64>*>(mat_host.data());
    ap_uint<64>* mod_dev = reinterpret_cast<ap_uint<64>*>(mod_host.data());
    ap_uint<64>* out_dev = reinterpret_cast<ap_uint<64>*>(out_hw.data());

    bconv_systolic(x_dev, mat_dev, mod_dev, out_dev);

    // 5. 结果比对
    std::cout << "Verifying Results..." << std::endl;
    int errors = 0;
    for (int r = 0; r < RING_DIM; ++r) {
        for (int c = 0; c < SIZE_P; ++c) {
            int idx = r * SIZE_P + c;
            if (out_hw[idx] != out_ref[idx]) {
                if (errors < 20) { // 只打印前20个错误
                    std::cout << "Mismatch at Row=" << r << " Col=" << c 
                              << " | Ref=" << out_ref[idx] << " HW=" << out_hw[idx] << std::endl;
                }
                errors++;
            }
        }
    }

    // 6. 打印部分结果用于观察 (只打印前几行)
    std::cout << "\n--- Sample Data Inspection ---\n";
    // 如果 RING_DIM 很大，不要全部打印
    print_matrix_head("X (Input)", x_host.data(), RING_DIM, SIZE_Q);
    print_matrix_head("W (Weight)", mat_host.data(), SIZE_Q, SIZE_P);
    print_matrix_head("Out (Ref)", out_ref.data(), RING_DIM, SIZE_P);
    print_matrix_head("Out (HW) ", out_hw.data(),  RING_DIM, SIZE_P);

    if (errors == 0) {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "  TEST PASSED! Output Matches perfectly. " << std::endl;
        std::cout << "=============================================" << std::endl;
        return 0;
    } else {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "  TEST FAILED with " << errors << " errors." << std::endl;
        std::cout << "=============================================" << std::endl;
        return 1;
    }
}