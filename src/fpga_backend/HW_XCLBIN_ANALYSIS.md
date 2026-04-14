# HW xclbin 构建可行性分析报告

> **项目**：OpenFHE-CKKS FPGA 加速后端（`src/fpga_backend/`）
> **综合工具**：Vitis HLS 2024.1（Build 5069499，2024-05-21）
> **综合日期**：2026-04-13
> **综合目标器件**：`xcu55c-fsvh2892-2L-e`（U55C）
> **Makefile hw 目标平台**：`xilinx_u250_gen3x16_xdma_4_1_202210_1`（U250）
> **结论**：⛔ **HW xclbin 无法成功，存在致命时序违例 + 资源超标 + 功能 BUG**

---

## 一、综合结果速览（来自 `csynth.rpt`）

| 资源 | 综合估算 | U55C 全片可用量 | 利用率 | 状态 |
|------|---------|----------------|--------|------|
| BRAM_18K | **8334** | 4032 | **206%** | ❌ 严重超标 |
| DSP | 1010 | 9024 | 11% | ✅ 正常 |
| FF | 352,140 | 2,607,360 | 13% | ✅ 正常 |
| LUT | 716,162 | 1,303,680 | **54%** | ⚠️ 偏高 |
| URAM | 0 | 960 | 0% | ✅ 正常 |

**顶层 `Top` 模块的 Issue Type 报告为 `Timing`，Slack = −0.33 ns**，即：

```
|+ Top  |  Timing|  -0.33|  ...  |  8334 (206%)|  ...
```

这两项——**时序违例（负 Slack）** 和 **BRAM 超标 206%**——是 hw 实现必然失败的直接原因。

---

## 二、致命问题（来自综合报告的实证）

### 🔴 问题 1：BRAM 用量超标 206%，无法布局布线

**综合报告第 22 行：**
```
|+ Top  |  Timing|  -0.33|  -|  8334 (206%)|  1010 (11%)|  352140 (13%)|  716162 (54%)| -|
```

U55C 全片只有 **4032 个 BRAM_18K**，综合报告估算需要 **8334 个**，超出约 **2 倍**。

**根因追溯**——`top.cpp` 中的两个静态 twiddle factor 数组：

```cpp
// top.cpp 第 32-33 行
static uint64_t NTTTwiddleFactor[MAX_LIMBS][BU_NUM][RING_DIM];
// = 5 × 32 × 4096 × 8 bytes ≈ 5 MB

static uint64_t INTTTwiddleFactor[MAX_LIMBS][BU_NUM][RING_DIM];
// = 5 MB，合计 ≈ 10 MB
```

加上 `poly_buffer_1/2`、`result_buffer` 等：

```cpp
static uint64_t poly_buffer_1[MAX_LIMBS][SQRT][SQRT];  // 5×64×64×8 = 160 KB
static uint64_t poly_buffer_2[MAX_LIMBS][SQRT][SQRT];  // 同上
```

10 MB twiddle factor 需要约 **5120 个 BRAM_18K**，U55C 根本放不下。

> 进一步地，`InterLeave` 中申请了 `ARRAY_PARTITION complete dim=2`（SQRT=64 列全展开），
> 将 `temp_buffer[64][64]` 展开成 64 个独立 BRAM bank，大量消耗 BRAM。
> 综合报告中 `InterLeave` 单独消耗 **16 个 BRAM_18K**，且被调用 5 次，合计 80 个。

**后果**：Vivado 实现阶段 Place & Route 直接因资源不足而失败，hw xclbin 无法生成。

---

### 🔴 问题 2：顶层时序违例（Slack = −0.33 ns），时钟约束 250 MHz 不满足

**综合报告第 22 行：**
```
|+ Top  |  Timing|  -0.33|  ...
```

`csynth.tcl` 设置的时钟周期为 4 ns（即 250 MHz）：
```tcl
create_clock -period 4ns
```

但综合估算 `Top` 的关键路径 Slack 为 **−0.33 ns**，意味着实际关键路径约需 **4.33 ns**，无法满足约束。

**根因追溯**——`NTT_Kernel_Pipeline_VITIS_LOOP_134_1` 是主要瓶颈：

| 模块 | DSP | FF | LUT | Slack |
|------|-----|----|-----|-------|
| `NTT_Kernel_Pipeline_VITIS_LOOP_134_1` | **672** | 40,568 | 46,395 | **0.05 ns** |

该 pipeline 对应 `compute_core` 中的 `Configurable_PE` 展开后的蝶形计算，在 4 ns 时钟下已几乎压线（Slack 仅 0.05 ns），任何 RTL 到实现的 wire delay 都会导致时序违例。

而整个 `Top` 包含两次 `NTT_Kernel` 调用，整体 Slack 为 −0.33 ns。这在 hw 实现阶段（Place & Route 之后）通常会进一步恶化，最终时序收敛失败。

---

### 🔴 问题 3：`Compute_BConv` 脉动阵列使用 `%` 运算符，生成 30 个硬件除法器，导致 LUT 爆炸

**综合报告（`Compute_BConv_Pipeline_Systolic_Loop_csynth.rpt`）：**

```
|urem_128ns_64ns_64_132_1_U3856  |urem_128ns_64ns_64_132_1  | 0| 0| 8651| 6607| 0|
|urem_128ns_64ns_64_132_1_U3857  |...                        | 0| 0| 8651| 6607| 0|
...（共 30 个 urem 实例）
```

`bconv.cpp` 第 114–115 行的 `%` 运算：

```cpp
ap_uint<128> prod = ((ap_uint<128>)x_in * (ap_uint<128>)in_w[q][p]) % mod_p;
ap_uint<128> sum_out = (sum_in + prod) % mod_p;
```

HLS 将 128 位 `%` 综合为硬件除法器（`urem_128ns_64ns`），每个约需 **8651 FF + 6607 LUT**，共 30 个实例：

- FF：30 × 8651 = **259,530**（单此一处占全片 10%）
- LUT：30 × 6607 = **198,210**（单此一处占全片 15%）

这是 LUT 达到 54% 的主要原因，且这些除法器的时序路径极长，是时序违例的另一重要来源。

---

### 🔴 问题 4：`OP_MUL` 未定义，hw 综合必然编译报错

**位置**：`top.cpp` 第 131 行

```cpp
case OP_MUL:   // ❌ 未定义的宏
```

`define.h` 中定义的是 `OP_MULT`（值为 3），`OP_MUL` 根本不存在。
虽然 `sw_emu` 侥幸通过（v++ sw_emu 编译路径容忍度更高），但 Vitis HLS hw 综合会直接报编译错误。

> **注意**：本次综合报告之所以能生成（`csynth.tcl` 设 `set_top Top`），
> 是因为 `switch-case` 中未定义的宏在某些 HLS 版本可能被当作 `0` 处理或 warning，
> 但在严格的 `v++` hw compile 流程中会阻断编译。

---

### 🔴 问题 5：Twiddle Factor 加载逻辑 BUG（`OP_INIT`），BU 轴数据全部相同

**位置**：`top.cpp` 第 96–113 行

```cpp
// OP_INIT 中 NTT twiddle 加载
int offset = LIMB_Q * 3;  // = 9
for (int l = 0; l < LIMB_Q + LIMB_P; l++) {
    for (int b = 0; b < BU_NUM; b++) {     // b = 0..31
        for (int t = 0; t < RING_DIM; t++) {
            NTTTwiddleFactor[l][b][t] = mem_in1[offset + l*RING_DIM + t];
            //                                   ^^^^^^^^^^^^^^^^^^^^^^^^
            // ❌ 索引与 b 无关！32 次循环读同一段数据，
            //    所有 BU 的 twiddle factor 完全一样
        }
    }
}
```

数组声明为 `[MAX_LIMBS][BU_NUM][RING_DIM]`，意味着不同 BU 应持有不同的旋转因子段，
但加载时完全忽略了 `b` 维度的 offset，导致所有 32 个 BU 读到完全相同的 twiddle factor。

**后果**：NTT 蝶形运算的旋转因子全错，所有 NTT/INTT 计算结果均不正确。
即便 hw xclbin 侥幸生成，功能验证也会全面失败。

---

### 🔴 问题 6：`Compute_BConv` 子函数中声明了 `m_axi` 接口，与顶层冲突

**位置**：`bconv.cpp` 第 144–148 行

```cpp
void Compute_BConv(...) {
    #pragma HLS INTERFACE m_axi port=in_x   bundle=gmem0  // ❌ 子函数不能声明 m_axi
    #pragma HLS INTERFACE m_axi port=in_w   bundle=gmem1
    #pragma HLS INTERFACE s_axilite port=out_mod bundle=control
    ...
}
```

综合报告中 `Top` 模块只生成了 `m_axi_gmem0` 和 `m_axi_gmem1` 两个 AXI 接口，
`Compute_BConv` 的 `m_axi` 声明被忽略（HLS Warning），但也引发了如下后果：

- `bconv.cpp` 的 `in_w` 参数（`const uint64_t in_w[LIMB_Q][MAX_OUT_COLS]`）在子函数声明为 m_axi，
  在顶层 `Top` 中该参数是 **局部栈变量**（`static uint64_t in_w[LIMB_Q][MAX_OUT_COLS]`），
  接口声明逻辑矛盾；
- 综合报告显示 `m_axi_gmem1` 的 `Compute_BConv` 子树中实际上没有外部 AXI 访问，
  Load_W/Load_X 均通过 Top 传递的局部 buffer，与预期不符。

---

### 🔴 问题 7：`u55c.cfg` 中 `part=` 为空，链接必然报错

**位置**：`u55c.cfg` 第 1 行

```ini
part=    ← 空值
```

若用 `--config u55c.cfg` 执行 hw 链接，Vitis 无法确定目标 part，直接报错。

---

### 🔴 问题 8：`u55c.cfg` 引用了不存在的 kernel `ntt_forward_kernel`

**位置**：`u55c.cfg` 第 7 行

```ini
nk=ntt_forward_kernel:1:ntt_forward_1   ← 不存在的顶层函数
```

整个项目只有一个顶层 kernel `Top`，链接阶段报错退出。

---

## 三、中等问题（综合数据印证）

### 🟡 问题 9：`init_Q_Loop` 和 `init_P_Loop` 出现 II 违例（Issue Type = II）

**综合报告第 53、64 行：**

```
| o init_Q_Loop  |  II|  2.92|  18|  72.000|  13|  3|  3|  yes|
| o init_P_Loop  |  II|  2.92|  15|  60.000|  13|  3|  2|  yes|
```

`init_Q_Loop` 目标 II=1，实际 achieved=3。原因是 `OP_INIT` 中同一个循环里对 `mem_in1` 存在多次不同 stride 的访问（见综合报告 AXI Burst 分析第 214 行）：

```
| m_axi_gmem0  | -  | src/top.cpp:73:13  | read  | Fail  | Could not burst due to
                                                           multiple potential reads
                                                           to the same bundle in the
                                                           same region.
```

`init_Q_Loop` 内同时读取 `mem_in1[i]`、`mem_in1[LIMB_Q+i]`、`mem_in1[LIMB_Q*2+i]`，三个不连续地址映射到同一 bundle，导致 burst 失败且 II 升至 3。

---

### 🟡 问题 10：`InterLeave` 中 `Shift_Left/Right` 循环 II=4，实际并行度远低于预期

**综合报告第 100–102 行：**

```
|  o Shift_Left_Row_Shift_Left_Col   |  II|  2.92|  2050|  8.200e+03|  7|  4|  512|  yes|
|  o Shift_Right_Row_Shift_Right_Col |  II|  2.92|  2050|  8.200e+03|  7|  4|  512|  yes|
```

`InterLeave` 设计意图是 `#pragma HLS PIPELINE II=1`，但实际 II=4（目标 1 未达到），
Trip Count=512（SQRT×SQRT/PARALLEL_FACTOR = 4096/8 = 512）。

原因：`data[i][src_j]` 的 `src_j = (k - i + SQRT) & mask`，地址非连续，
导致 BRAM 访问冲突，HLS 无法压缩到 II=1。

---

### 🟡 问题 11：`mem_out` 与 `mem_in1` 共享 `gmem0`，AXI 读写冲突

**综合报告 HW Interface 节：**

```
| m_axi_gmem0 | READ_WRITE | 64 -> 64 | ...
```

```
SW-to-HW Mapping:
| mem_in1  | m_axi_gmem0 |   ← 读
| mem_out  | m_axi_gmem0 |   ← 写，同一 bundle
```

`mem_in1` 和 `mem_out` 共享 `gmem0`，HLS 生成的仲裁逻辑会导致带宽减半。
在 `OP_ADD/SUB/MULT` 操作中，先读 `in1`，后写 `out`，虽然串行不会导致数据错误，
但若 host 传入的 `mem_in1` 与 `mem_out` 地址重叠则会出现 RAW hazard。

---

## 四、设计规模警告（来自 `csynth_design_size.rpt`）

```
| Array/Struct  |  (1) array partition  |  255,212 *  |  ← 超阈值 100,000
|               |  (2) simplification   |  133,512 *  |  ← 超阈值
| Performance   |  (1) loop simplification | 133,588 * |  ← 超阈值
| HW Transforms |  (2) optimizations    |  132,820 *  |  ← 超阈值
```

`*` 号表示超过了 HLS 设计规模警告阈值（100,000 条指令），综合后指令规模达到 **13.3 万条**，
主要来源是 `Load` 函数（被调用 10 次，共贡献 33,680 条指令）和 `InterLeave`（两次调用，39,596 条）。
这说明设计已经大到难以高效综合，编译时间也会极长。

---

## 五、问题汇总与修复优先级

| 优先级 | # | 问题 | 综合依据 | 修复方向 |
|--------|---|------|---------|---------|
| P0 ❌ | 1 | BRAM 超标 206%（8334 > 4032） | `csynth.rpt` 第 22 行 | 将 twiddle factor 从片上 static 改为 DDR 流式加载；移除 `InterLeave` 的 complete partition |
| P0 ❌ | 2 | 顶层时序违例 Slack=−0.33 ns | `csynth.rpt` 第 22 行 | 将时钟周期从 4 ns 改为 5 ns（200 MHz）；或优化 `NTT_Kernel` 关键路径 |
| P0 ❌ | 3 | `BConv` 脉动阵列用 `%` 运算生成 30 个硬件除法器 | `Compute_BConv_Pipeline_Systolic_Loop_csynth.rpt` | 用 Barrett 约减替换 `%`，消除 `urem` 实例 |
| P0 ❌ | 4 | `OP_MUL` 未定义，hw 综合编译错误 | `top.cpp` 第 131 行 | 改为 `case OP_MULT:` |
| P0 ❌ | 5 | Twiddle Factor 加载缺少 `b` 轴 offset | `top.cpp` 第 99–113 行 | 修正索引公式加入 `b*RING_DIM` |
| P0 ❌ | 6 | `Compute_BConv` 子函数声明 `m_axi` | `bconv.cpp` 第 144 行 | 删除子函数中的 `#pragma HLS INTERFACE` |
| P0 ❌ | 7 | `u55c.cfg` `part=` 为空 | `u55c.cfg` 第 1 行 | 填入 `xcu55c-fsvh2892-2L-e` |
| P0 ❌ | 8 | `u55c.cfg` 引用不存在的 `ntt_forward_kernel` | `u55c.cfg` 第 7 行 | 改为 `nk=Top:1:Top` |
| P1 ⚠️ | 9 | `init_Q_Loop` II=3，AXI burst fail | `csynth.rpt` 第 53 行，AXI 分析第 214 行 | 将三段读操作拆分为三个独立的循环，每段连续地址 |
| P1 ⚠️ | 10 | `InterLeave` Shift Loop II=4（目标 1） | `csynth.rpt` 第 100 行 | 改用 ping-pong BRAM 实现，确保访问地址连续 |
| P1 ⚠️ | 11 | `mem_out` 与 `mem_in1` 共享 `gmem0` | `csynth.rpt` HW Interface 节 | `mem_out` 改用 `bundle=gmem2` |
| P2 💡 | 12 | `csynth.tcl` 目标器件为 U55C，Makefile hw 目标为 U250 | `csynth.rpt` 第 12 行 vs `Makefile` 第 10 行 | 统一目标器件 |
| P2 💡 | 13 | 设计规模超 HLS 警告阈值 13.3 万条指令 | `csynth_design_size.rpt` | 减少 `Load` 调用次数，复用缓冲区 |

---

## 六、结论

当前代码库经 Vitis HLS 2024.1 综合后，存在以下**不可绕过的失败条件**：

1. **BRAM 需求 206% 超标**——U55C 物理上放不下，Place 直接失败。
2. **顶层时序 Slack = −0.33 ns**——即便强行实现，时序收敛后仍会有大量 Timing 违例路径，bitstream 不可用。
3. **`BConv` 脉动阵列 30 个硬件除法器**——LUT 占 54%，再加上 BRAM 超标，几乎不可能通过实现。

在解决上述三个根本性资源/时序问题之前，无论做何其他修复，hw xclbin 均无法生成。

---

*本报告综合了静态代码分析与 HLS 综合报告（`Solution/solution1/syn/report/`）的实测数据。*
