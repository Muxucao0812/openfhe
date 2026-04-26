将 Constant Geometry NTT (CG-NTT) 彻底整合进你的异构加速架构，是一次从 **CPU 端数据准备 (Host) -> PCIe 传输 (XRT) -> FPGA 顶层调度 (top.cpp) -> 核心引擎 (cg_ntt.cpp)** 的端到端重构。

结合你提供的 `FpgaManager.h`，我将整个升级过程分为 **Host 端（软件）** 和 **FPGA 端（硬件）** 两个维度，为你梳理出完整的实现步骤和数据流路径。

---

### 第一阶段：Host 端 (CPU) 的重构 `FpgaManager.h`

这是最容易被忽略但最致命的一环。FPGA 端的 CG-NTT 吞吐量虽高，但它要求每层的旋转因子在空间上完全展开。原来的标准 NTT 每条 Limb 只需要 $N=4096$ 个旋转因子，而 CG-NTT 每条 Limb 需要 $STAGE \times (N/2) = 12 \times 2048 = 24576$ 个。

如果你不改 Host 端直接烧录，FPGA 在 `OP_INIT` 阶段读取 DDR 时会发生严重的**越界访问（AXI Hang）**，导致整个板卡卡死。

**1. 修改 Twiddle Factor 预计算逻辑**
在 `FpgaManager::InitModuli` 中，你需要废弃原有的 `GenerateTwiddleIndices` 和 `permuted_ntt` 逻辑。

```cpp
// 废弃原有的 N 长度数组
// std::vector<uint64_t> all_ntt_twiddles(total_limbs * N);

// 引入 CG-NTT 专用的尺寸
const int STAGE = 12; // 对于 4096 点
const int CG_HALF_N = FPGA_RING_DIM / 2;
const int CG_TF_SIZE = STAGE * CG_HALF_N; // 24576

std::vector<uint64_t> cg_ntt_twiddles(total_limbs * CG_TF_SIZE);
std::vector<uint64_t> cg_intt_twiddles(total_limbs * CG_TF_SIZE);

for(size_t limb = 0; limb < total_limbs; limb++) {
    uint64_t mod = m_stored_moduli[limb];
    uint64_t psi = combined_roots[limb]; 
    uint64_t psi_inv = MathUtils::ModInverse(psi, mod);

    // 【核心算法】你需要根据 Perfect Shuffle (完美洗牌) 路由网络，
    // 预先计算出每一层 (0~11) 的每一个蝶形单元 (0~2047) 需要的旋转因子。
    // 这取代了 FPGA 内部的寻址逻辑，把计算压力转移给 CPU 的初始化阶段。
    for (int s = 0; s < STAGE; s++) {
        for (int t = 0; t < CG_HALF_N; t++) {
            int flat_idx = limb * CG_TF_SIZE + s * CG_HALF_N + t;
            // 获取在 stage s, index t 时的正确单位根幂次 (根据你 CG-NTT 的具体数学定义)
            uint64_t power = GetCGTwiddlePower(s, t); // 你需要在 MathUtils 中实现此映射
            cg_ntt_twiddles[flat_idx] = MathUtils::Power(psi, power, mod);
            cg_intt_twiddles[flat_idx] = MathUtils::Power(psi_inv, power, mod);
        }
    }
}
```

**2. 修改 XRT/PCIe 传输的 Buffer 大小**
组装发送给 FPGA 的 Buffer 时，必须按照扩容后的尺寸申请空间：

```cpp
// 以前是 total_limbs * N，现在必须是 total_limbs * CG_TF_SIZE
size_t buf1_size = n_q * PARAMS_PER_LIMB + total_limbs * CG_TF_SIZE;
std::vector<uint64_t> buf1_Q(buf1_size);
// ... 拷贝 modulus, K, M ...
memcpy(buf1_Q.data() + n_q * PARAMS_PER_LIMB, cg_ntt_twiddles.data(), total_limbs * CG_TF_SIZE * sizeof(uint64_t));

size_t buf2_size = n_p * PARAMS_PER_LIMB + total_limbs * CG_TF_SIZE;
std::vector<uint64_t> buf2_P(buf2_size);
// ... 拷贝 modulus, K, M ...
memcpy(buf2_P.data() + n_p * PARAMS_PER_LIMB, cg_intt_twiddles.data(), total_limbs * CG_TF_SIZE * sizeof(uint64_t));
```

---

### 第二阶段：FPGA 端的重构 (`top.cpp` & `cg_ntt.cpp`)

这一阶段的目标是：**打通片上 BRAM -> CG-NTT 内核的零开销数据流**。

**1. `top.cpp`：扩展 URAM 并更新 `OP_INIT`**
* **资源映射**：将原来的 `NTTTwiddleFactor[MAX_LIMBS][PE_PARALLEL][RING_DIM]` 替换为 `CG_NTTTwiddle[MAX_LIMBS][STAGE][CG_HALF_N]`。
* **绑定 URAM**：由于容量暴增至每 Limb 192KB，依然必须使用 `#pragma HLS BIND_STORAGE type=ram_2p impl=uram`，防止 BRAM 被榨干。
* **更新初始化逻辑**：在 `OP_INIT` 分支中，把从 CPU 传进来的 `mem_in1` 和 `mem_in2` 的一维长数组，按 `STAGE` 和 `CG_HALF_N` 的嵌套循环，写入 URAM 中。

**2. `top.cpp`：清理 `OP_NTT` 和 `OP_INTT` 调度逻辑**
* **砍掉 InterLeave**：在之前的架构中，由于标准 NTT 算法结构，你在 `OP_NTT` 前后调用了 `InterLeave(poly_buffer_1[l], true)` 来调整数据排布。CG-NTT 天然使用完美洗牌网络，不再需要显式的跨步穿插。把它们全部删掉，省下大量时钟周期。
* 直接调用新的包装器函数 `Compute_CG_NTT`。

**3. `cg_ntt.cpp`：零开销地址映射（位截取）**
正如我们之前讨论的，这是性能优化的核心：
* **抛弃展平拷贝**：删掉之前通过 AXI 读取和 `flatten_2d_to_1d` 的代码。
* **修改 Kernel 签名**：让 `CG_NTT_Kernel_Opt` 直接接收 `uint64_t poly_limb[SQRT][SQRT]`（这是片上的 2D BRAM）。
* **硬件连线映射**：在 `INIT_LOOP` 将数据推入内部 Ping-Pong buffer 时，直接利用 `row = flat_idx >> LOG_SQRT` 和 `col = flat_idx & SQRT_MASK` 获取 2D 地址。在 `WRITEBACK_LOOP` 写回 BRAM 时做同样的操作。这在 RTL 层面就是单纯的导线交叉（Wire cross assignment），0 周期延迟，0 DSP 开销。

---

### 完整的端到端生命周期总结

完成上述修改后，当你调用 `NttForwardOffload` 时，系统将按以下完美流线运转：

1. **Host (CPU) 离线阶段**：调用 `InitModuli`，根据完美洗牌几何计算出 $24576$ 个位宽的旋转因子，通过 PCIe 发送给 FPGA 的 DDR，并触发 `OP_INIT`。
2. **FPGA (OP_INIT)**：硬件将 DDR 里的旋转因子搬运并持久化到片内的 **URAM** 中（按 PE 并行度做了划分）。
3. **Host 运行时阶段**：程序执行同态加密运算，产生了一个 $4096$ 维的多项式，调用 `NttForwardOffload`，将其丢入 `bo_in1`。
4. **FPGA (OP_NTT)**：
   * **Load**：从 DDR burst read 多项式，存入 `poly_buffer_1`（2D BRAM 数组）。
   * **Compute**：CG-NTT Kernel 启动。利用位截取逻辑，0 周期将其当作 1D 数组读入内核的 Ping-Pong FIFO。
   * **Pipeline**：由于每一层都是读相邻 $N/2$、写 $2i/2i+1$，并且 Twiddle Factor 从 URAM 顺序流式读出，无任何访存冲突（Bank Conflict），以 II=1 的全速流水线跑完 12 层。
   * **Reorder & Store**：在最后一步写回 `poly_buffer_1` 时，顺带完成 Bit-Reverse 重排，然后 burst write 回 DDR。

通过这种重构，你消灭了原来硬件里的复杂 MUX 寻址、消灭了 `InterLeave` 带来的停顿、也消灭了 3D 变 1D 的显式搬运。整个数据通路变得像水管一样顺滑，这将大幅度提升你这套 FHE 加速器的最高运行频率（Fmax）。