#ifndef _FPGA_MANAGER_H_
#define _FPGA_MANAGER_H_

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstring>   // memset, memcpy
#include <cstdlib>   // std::getenv
#include <cmath>     // std::log2, std::ceil
#include <algorithm> // std::max, std::find
#include <numeric>   // std::gcd (C++17)

// =============================================================
// 1. XRT Configuration (不变)
// =============================================================
#ifdef OPENFHE_FPGA_ENABLE

    #ifndef XRT_VERSION_CODE
    #define XRT_VERSION_CODE ((2 << 16) | (0 << 8) | 0)
    #endif

    #ifndef XRT_MAJOR
    #define XRT_MAJOR(v) (((v) >> 16) & 0xffff)
    #endif

    #ifndef XRT_MINOR
    #define XRT_MINOR(v) (((v) >> 8) & 0xff)
    #endif

    #include <xrt/xrt_device.h>
    #include <xrt/xrt_kernel.h>
    #include <xrt/xrt_bo.h>

#endif

// =============================================================
// 2. Definitions 
// =============================================================
#define OP_INIT   0
#define OP_ADD    1
#define OP_SUB    2
#define OP_MULT   3
#define OP_NTT    4
#define OP_INTT   5
#define OP_BCONV  6

#define MAX_LIMBS 5 
#define FPGA_RING_DIM  4096
#define STAGE_NUM 12 

inline std::string GetXclbinPath() {
    const char* mode = std::getenv("XCL_EMULATION_MODE");
    std::string base = "/home/CONNECT/xmeng027/work/FHE-BERT-Tiny/openfhe/src/fpga_backend/";
    if (mode && std::string(mode) == "sw_emu") 
        return base + "fhe_kernels_sw_emu.xclbin";
    else if (mode && std::string(mode) == "hw_emu") 
        return base + "fhe_kernels_hw_emu.xclbin";
    else if (mode && std::string(mode) == "hw") 
        return base + "fhe_kernels_hw.xclbin";
    else return base + "fhe_kernels_sw_emu.xclbin";
}

// =============================================================
// 3. Math Helpers 
// =============================================================
class MathUtils {
public:
    static uint64_t Power(unsigned __int128 base, unsigned __int128 exp, uint64_t mod) {
        unsigned __int128 res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return (uint64_t)res;
    }

    static uint64_t ModInverse(uint64_t n, uint64_t mod) {
        return Power(n, mod - 2, mod);
    }

    static uint64_t GCD(uint64_t a, uint64_t b) {
        while (b) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }

    static std::vector<uint64_t> GetPrimeFactors(uint64_t n) {
        std::vector<uint64_t> factors;
        uint64_t temp = n;
        for (uint64_t i = 2; i * i <= temp; ++i) {
            if (temp % i == 0) {
                factors.push_back(i);
                while (temp % i == 0) {
                    temp /= i;
                }
            }
        }
        if (temp > 1) {
            factors.push_back(temp);
        }
        return factors;
    }

    static bool IsPrimitiveRoot(uint64_t a, uint64_t p) {
        if (GCD(a, p) != 1) return false;
        uint64_t phi = p - 1;  
        std::vector<uint64_t> factors = GetPrimeFactors(phi);
        if (factors.empty()) return false; 

        for (uint64_t q_i : factors) {
            // 使用 128 位进行中间计算
            if (Power(a, phi / q_i, p) == 1) {
                return false;
            }
        }

        return true;
    }


    static uint64_t FindSmallestPrimitiveRoot(uint64_t p) {
        if (p <= 4) return p - 1;

     
        for (uint64_t g = 2; g < p; ++g) {
            if (IsPrimitiveRoot(g, p)) {
                return g; 
            }
        }
        return 0; 
    }


    // [修改] 使用 FindSmallestPrimitiveRoot 替换朴素搜索
    static uint64_t Find2NthRootOfUnity(uint64_t modulus, uint64_t n) {
        uint64_t required_order = 2 * n; 
        if ((modulus - 1) % required_order != 0) {
            throw std::runtime_error("Modulus does not support 2N-th root of unity");
        }
        
        // 1. 找到最小原根 g (SymPy 的 'pr')
        // WARNING: 这个调用可能非常慢，因为它包含了大数的质因数分解。
        uint64_t primitive_root_g = FindSmallestPrimitiveRoot(modulus);
        
        if (primitive_root_g == 0) {
            throw std::runtime_error("Failed to find smallest primitive root for the modulus.");
        }
        
        unsigned __int128 exponent = (modulus - 1) / required_order;
        uint64_t psi = Power(primitive_root_g, exponent, modulus);

        // 3. 验证性质: psi^N == -1 (即 p-1)
        if (Power(psi, n, modulus) == (modulus - 1)) {
            return psi;
        } else {
             throw std::runtime_error("Calculated Psi failed the N-th power test. Primitive root found but exponentiation error.");
        }
    }


    static std::vector<int> GenerateTwiddleIndices(int n) {
        std::vector<int> index = {0};
        for (int i = 0; i < STAGE_NUM; ++i) {
            std::vector<int> index_temp;
            int offset = n / (1 << (i + 1));
            for (int j : index) {
                index_temp.push_back(j + offset);
            }
            index.insert(index.end(), index_temp.begin(), index_temp.end());
        }
        index.erase(index.begin());
        return index;
    }
};

// =============================================================
// 4. FPGA Manager Class 
// =============================================================
class FpgaManager {
public:
    static FpgaManager& GetInstance() {
        static FpgaManager instance;
        return instance;
    }

    bool IsReady() const { return m_is_ready; }

 // ----------------------------------------------------------------------
    // InitModuli: Compute -> Index -> Permute -> Pack
    // ----------------------------------------------------------------------
    void InitModuli(const std::vector<uint64_t>& q_mods, const std::vector<uint64_t>& p_mods) {
    #ifdef OPENFHE_FPGA_ENABLE
        if (!m_is_ready) return;
        
        // 简单布局：Q(0,1,2) + P(3,4)，无padding
        size_t n_q = q_mods.size();
        size_t n_p = p_mods.size();
        size_t total_limbs = n_q + n_p;
        
        const int N = FPGA_RING_DIM;

        std::cout << "[Host] InitModuli: Q=" << n_q << " (idx 0-" << (n_q-1) << ")"
                  << ", P=" << n_p << " (idx " << n_q << "-" << (total_limbs-1) << ")" << std::endl;

        // 1. 存储模数 (用于 GetModIndex 查找)
        // 布局: [Q0, Q1, Q2, P0, P1] -> 索引 0, 1, 2, 3, 4
        m_stored_moduli.clear();
        m_stored_moduli.insert(m_stored_moduli.end(), q_mods.begin(), q_mods.end());
        m_stored_moduli.insert(m_stored_moduli.end(), p_mods.begin(), p_mods.end());

        // 2. 计算 Barrett 参数 K 和 M
        std::vector<uint64_t> K_vals(total_limbs), M_vals(total_limbs);
        for(size_t i=0; i<total_limbs; i++) {
            uint64_t p = m_stored_moduli[i];
            int k = (int)std::ceil(std::log2((double)p));
            unsigned __int128 power = (unsigned __int128)1 << (2 * k);
            uint64_t m = (uint64_t)(power / p);
            K_vals[i] = k;
            M_vals[i] = m;
            
            std::cout << "  [Barrett] idx=" << i << ": mod=" << p 
                      << ", k=" << k << ", m=" << m << std::endl;
        }

        // 3. 计算 Twiddle Factors
        std::vector<uint64_t> all_ntt_twiddles(total_limbs * N);
        std::vector<uint64_t> all_intt_twiddles(total_limbs * N);

        for(size_t limb = 0; limb < total_limbs; limb++) {
            uint64_t mod = m_stored_moduli[limb];

            uint64_t psi = MathUtils::Find2NthRootOfUnity(mod, N); 
            uint64_t psi_inv = MathUtils::ModInverse(psi, mod);

            uint64_t w = 1;
            uint64_t w_inv = 1;

            for(int i = 0; i < N; i++) {
                all_ntt_twiddles[limb * N + i] = w;
                all_intt_twiddles[limb * N + i] = w_inv;
                w = ((unsigned __int128)w * psi) % mod;
                w_inv = ((unsigned __int128)w_inv * psi_inv) % mod;
            }
        }

        // 4. Permute Twiddles
        std::vector<int> perm_index = MathUtils::GenerateTwiddleIndices(N);
        std::vector<uint64_t> permuted_ntt(total_limbs * N);
        std::vector<uint64_t> permuted_intt(total_limbs * N);

        for (size_t limb = 0; limb < total_limbs; limb++) {
            size_t base_offset = limb * N;
            for (size_t i = 0; i < perm_index.size(); ++i) {
                permuted_ntt[base_offset + i]  = all_ntt_twiddles[base_offset + perm_index[i]];
                permuted_intt[base_offset + i] = all_intt_twiddles[base_offset + perm_index[i]];
            }
        }

        // ============================================================
        // 5. Pack Buffers (发送给 FPGA)
        // ============================================================
        const int PARAMS_PER_LIMB = 3; // MOD, K, M
        
        // --- Buffer 1 (Q Params + NTT Twiddles) ---
        // 布局: [Q0_mod, Q1_mod, Q2_mod, Q0_k, Q1_k, Q2_k, Q0_m, Q1_m, Q2_m, twiddles...]
        size_t buf1_size = n_q * PARAMS_PER_LIMB + total_limbs * N;
        std::vector<uint64_t> buf1_Q(buf1_size);

        for(size_t i=0; i<n_q; i++) {
            buf1_Q[i] = m_stored_moduli[i];           // MOD
            buf1_Q[n_q + i] = K_vals[i];              // K
            buf1_Q[n_q*2 + i] = M_vals[i];            // M
        }
        // Copy NTT Twiddles
        memcpy(
            buf1_Q.data() + n_q * PARAMS_PER_LIMB, 
            permuted_ntt.data(), 
            total_limbs * N * sizeof(uint64_t)
        );

        // --- Buffer 2 (P Params + INTT Twiddles) ---
        // 布局: [P0_mod, P1_mod, P0_k, P1_k, P0_m, P1_m, twiddles...]
        size_t buf2_size = n_p * PARAMS_PER_LIMB + total_limbs * N;
        std::vector<uint64_t> buf2_P(buf2_size);

        for(size_t i=0; i<n_p; i++) {
            // P 在 stored_moduli 中的索引从 n_q 开始
            size_t global_idx = n_q + i; 
            buf2_P[i] = m_stored_moduli[global_idx];        // MOD
            buf2_P[n_p + i] = K_vals[global_idx];           // K
            buf2_P[n_p*2 + i] = M_vals[global_idx];         // M
        }
        // Copy INTT Twiddles
        memcpy(buf2_P.data() + n_p * PARAMS_PER_LIMB, 
               permuted_intt.data(), 
               total_limbs * N * sizeof(uint64_t)
            );

        // 7. Send to FPGA (OP_INIT)
        auto bo_1 = xrt::bo(m_device, buf1_Q.size() * sizeof(uint64_t), m_kernel_top.group_id(0));
        auto bo_2 = xrt::bo(m_device, buf2_P.size() * sizeof(uint64_t), m_kernel_top.group_id(1));
        auto bo_out = xrt::bo(m_device, 8, m_kernel_top.group_id(2)); 

        bo_1.write(buf1_Q.data());
        bo_2.write(buf2_P.data());
        
        bo_1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        bo_2.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        std::cout << "[Host] Launching Init Kernel..." << std::endl;
        auto run = m_kernel_top(bo_1, bo_2, bo_out, OP_INIT, 0, 0); 
        run.wait();
        
        std::cout << "[Host] FPGA Parameter Init Complete." << std::endl;
    #endif
    }
    
    // --- 其他函数 (Execute, Wrappers) 保持不变 ---
    void Execute(
        uint8_t opcode, 
        const uint64_t* in1, 
        const uint64_t* in2, 
        uint64_t* out, 
        int num_limbs,
        int mod_idx 
    ) {
    #ifdef OPENFHE_FPGA_ENABLE
        if (!m_is_ready) return;
        try {
            size_t size_bytes = (size_t)num_limbs * FPGA_RING_DIM * sizeof(uint64_t);
            size_t out_size_bytes = size_bytes;
            auto bo_in1 = xrt::bo(m_device, size_bytes, m_kernel_top.group_id(0));
            auto bo_out = xrt::bo(m_device, out_size_bytes, m_kernel_top.group_id(2));

            bo_in1.write(in1);
            bo_in1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            xrt::bo bo_in2;
            if (in2 && in2 != in1) {
                size_t in2_size_bytes = size_bytes;
                bo_in2 = xrt::bo(m_device, in2_size_bytes, m_kernel_top.group_id(1));
                bo_in2.write(in2);
                bo_in2.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            } else { 
                bo_in2 = bo_in1; 
            }

            auto run = m_kernel_top(bo_in1, bo_in2, bo_out, opcode, num_limbs, mod_idx);
            run.wait();
            

            bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            bo_out.read(out);
        } catch (const std::exception& e) {
            std::cerr << "[FPGA Exec Error] " << e.what() << std::endl;
        }
    #endif
    }

    int GetModIndex(uint64_t modulus) {
        auto it = std::find(m_stored_moduli.begin(), m_stored_moduli.end(), modulus);
        if (it != m_stored_moduli.end()) {
            return (int)std::distance(m_stored_moduli.begin(), it);
        } else {
            std::cerr << "[FPGA Warning] Modulus " << modulus << " not found! Using 0." << std::endl;
            return 0;
        }
    }

    void NttForwardOffload(
        const uint64_t* in, 
        uint64_t* out, 
        uint64_t modulus, 
        size_t n
    ) {
        std::cout << "=== [FPGA] Execute NTT ===" << std::endl;
        int mod_idx = GetModIndex(modulus);
        Execute(OP_NTT, in, nullptr, out, 1, mod_idx); 
    }

    void NttInverseOffload(
        const uint64_t* in, 
        uint64_t* out, 
        uint64_t modulus, 
        size_t n
    ) {
        std::cout << "=== [FPGA] Execute INTT ===" << std::endl;
        int mod_idx = GetModIndex(modulus);
        Execute(OP_INTT, in, nullptr, out, 1, mod_idx);
    }

    // ============================================================
    // 单limb操作（保留向后兼容）
    // ============================================================
    void ModMultOffload(
        const uint64_t* a, 
        const uint64_t* b, 
        uint64_t* result, 
        uint64_t modulus, 
        size_t n
    ) {
        int mod_idx = GetModIndex(modulus);
        Execute(OP_MULT, a, b, result, 1, mod_idx);
    }

    void ModAddOffload(
        const uint64_t* a, 
        const uint64_t* b, 
        uint64_t* result, 
        uint64_t modulus, 
        size_t n
    ) {
        int mod_idx = GetModIndex(modulus);
        Execute(OP_ADD, a, b, result, 1, mod_idx);
    }
    
    void ModSubOffload(
        const uint64_t* a, 
        const uint64_t* b, 
        uint64_t* result, 
        uint64_t modulus, 
        size_t n
    ) {
        int mod_idx = GetModIndex(modulus);
        Execute(OP_SUB, a, b, result, 1, mod_idx);
    }

    // ============================================================
    // 批量limb操作（一次kernel调用处理所有limb）
    // 要求：所有limb的模数索引是连续的（从mod_idx_start开始）
    // ============================================================
    void ModOpBatchOffload(
        int opcode,             // OP_ADD, OP_SUB, or OP_MUL (注意是OP_MUL不是OP_MULT)
        const uint64_t* a,      // [numLimbs × ringDim]
        const uint64_t* b,      // [numLimbs × ringDim]
        uint64_t* result,       // [numLimbs × ringDim]
        int mod_idx_start,      // 起始模数索引
        size_t ringDim,
        size_t numLimbs
    ) {
    #ifdef OPENFHE_FPGA_ENABLE
        if (!m_is_ready) return;
        
        std::cout << "=== [FPGA] Batch Op=" << opcode << " numLimbs=" << numLimbs 
                  << " modStart=" << mod_idx_start << " ===" << std::endl;

        try {
            size_t total_size = numLimbs * ringDim * sizeof(uint64_t);
            
            auto bo_a = xrt::bo(m_device, total_size, m_kernel_top.group_id(0));
            auto bo_b = xrt::bo(m_device, total_size, m_kernel_top.group_id(1));
            auto bo_out = xrt::bo(m_device, total_size, m_kernel_top.group_id(2));

            // 一次性传输所有数据
            bo_a.write(a);
            bo_a.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            
            bo_b.write(b);
            bo_b.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            // 一次kernel调用处理所有limb
            // num_active_limbs = numLimbs, mod_index = mod_idx_start
            auto run = m_kernel_top(bo_a, bo_b, bo_out, opcode, (int)numLimbs, mod_idx_start);
            run.wait();

            // 一次性读取所有结果
            bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            bo_out.read(result);
        } catch (const std::exception& e) {
            std::cerr << "[FPGA Batch Op Error] " << e.what() << std::endl;
        }
    #endif
    }

    // BConv with dynamic output moduli
    // Kernel: LIMB_Q rows × MAX_OUT_COLS columns (3×5)
    static const int KERNEL_LIMB_Q = 3;
    static const int KERNEL_MAX_OUT_COLS = 5;  // LIMB_Q + LIMB_P
    
    void BConvOffload(
        const uint64_t* x,              // 输入: [KERNEL_LIMB_Q × RING_DIM]
        const uint64_t* w,              // 权重: [KERNEL_LIMB_Q × KERNEL_MAX_OUT_COLS]
        const uint64_t* out_mod,        // 输出模数: [sizeP]
        uint64_t* result,               // 输出: [sizeP × RING_DIM]
        size_t ringDim, 
        int sizeP                       // 实际输出列数
    ) {
    #ifdef OPENFHE_FPGA_ENABLE
        if (!m_is_ready) return;
        
        std::cout << "=== [FPGA] Execute BConv === sizeP=" << sizeP << std::endl;

        try {
            // Buffer sizes
            size_t in_size = KERNEL_LIMB_Q * ringDim * sizeof(uint64_t);
            // mem_in2 = weights + out_mod
            size_t weights_count = KERNEL_LIMB_Q * KERNEL_MAX_OUT_COLS;
            size_t mod_count = KERNEL_MAX_OUT_COLS;
            size_t meta_size = (weights_count + mod_count) * sizeof(uint64_t);
            size_t out_size = sizeP * ringDim * sizeof(uint64_t);
            
            // Pack weights + moduli into single buffer
            std::vector<uint64_t> meta_buffer(weights_count + mod_count, 0);
            // Copy weights
            std::memcpy(meta_buffer.data(), w, weights_count * sizeof(uint64_t));
            // Copy output moduli (padded with 0)
            for (int i = 0; i < sizeP && i < KERNEL_MAX_OUT_COLS; i++) {
                meta_buffer[weights_count + i] = out_mod[i];
            }
            
            auto bo_in = xrt::bo(m_device, in_size, m_kernel_top.group_id(0));
            auto bo_meta = xrt::bo(m_device, meta_size, m_kernel_top.group_id(1));
            auto bo_out = xrt::bo(m_device, out_size, m_kernel_top.group_id(2));

            bo_in.write(x);
            bo_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);
            
            bo_meta.write(meta_buffer.data());
            bo_meta.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            // num_active_limbs = sizeP (输出列数)
            auto run = m_kernel_top(bo_in, bo_meta, bo_out, OP_BCONV, sizeP, 0);
            run.wait();

            bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
            bo_out.read(result);
        } catch (const std::exception& e) {
            std::cerr << "[FPGA BConv Error] " << e.what() << std::endl;
        }
    #endif
    }


private:
#ifdef OPENFHE_FPGA_ENABLE
    xrt::device m_device;
    xrt::kernel m_kernel_top;
#endif
    bool m_is_ready = false;
    std::vector<uint64_t> m_stored_moduli;

    FpgaManager() {
#ifdef OPENFHE_FPGA_ENABLE
        try {
            m_device = xrt::device(0); 
            std::cout << "[FPGA] Device connected: " << m_device.get_info<xrt::info::device::name>() << std::endl;
            auto uuid = m_device.load_xclbin(GetXclbinPath());
            m_kernel_top = xrt::kernel(m_device, uuid, "Top");
            m_is_ready = true;
            std::cout << "[FPGA] Unified Top Kernel Loaded.\n";
        } catch (const std::exception& e) { 
            std::cerr << "[FPGA Setup Error] " << e.what() << std::endl;
            m_is_ready = false; 
        }
#endif
    }
    FpgaManager(const FpgaManager&) = delete;
    void operator=(const FpgaManager&) = delete;
};

#endif // _FPGA_MANAGER_H_