#include "../include/cg_ntt.h"

//============================================================================
// File   : cg_ntt.cpp
// Brief  : CG-NTT（恒定几何 NTT）核心实现
//
// 算法概述（CG-NTT 的三大硬件红利）：
//   ① 每层 PE 永远读取相距 N/2 的两个数：Read(i) 和 Read(i + N/2)
//   ② 蝶形运算完成后，按"完美洗牌"规则写回：Write(2i) 和 Write(2i+1)
//   ③ 由于上层已经洗过牌，下一层 PE 依然只需读固定位置的数
//   → MUX/交叉开关彻底消灭，变为固定物理硬连线
//
// 存储架构：
//   - buf_A[RING_DIM] / buf_B[RING_DIM]：1D 乒乓缓冲
//   - cyclic factor=16：保证 8 PE × (读u + 读v) 无 Bank Conflict
//   - ram_2p：读端 global_i 和 global_i+N/2 落同一 bank，需双端口
//
// 旋转因子：
//   - cg_twiddle[STAGE][CG_HALF_N]：由 Host 端预计算
//   - FPGA 顺序消费：tf = cg_twiddle[stage][global_i]，无运行时索引计算
//============================================================================

// ============================================================
// 2D ↔ 1D 布局转换辅助
// ============================================================

void flatten_2d_to_1d(
    const uint64_t src[SQRT][SQRT],
    uint64_t dst[RING_DIM]
) {
    #pragma HLS INLINE
    FLATTEN_ROW:
    for (int i = 0; i < SQRT; i++) {
        #pragma HLS PIPELINE II=1
        FLATTEN_COL:
        for (int j = 0; j < SQRT; j++) {
            #pragma HLS UNROLL factor=PE_PARALLEL
            dst[i * SQRT + j] = src[i][j];
        }
    }
}

void reshape_1d_to_2d(
    const uint64_t src[RING_DIM],
    uint64_t dst[SQRT][SQRT]
) {
    #pragma HLS INLINE
    RESHAPE_ROW:
    for (int i = 0; i < SQRT; i++) {
        #pragma HLS PIPELINE II=1
        RESHAPE_COL:
        for (int j = 0; j < SQRT; j++) {
            #pragma HLS UNROLL factor=PE_PARALLEL
            dst[i][j] = src[i * SQRT + j];
        }
    }
}

// ============================================================
// CG_PE：CG-NTT 专用蝶形单元
//
// 与 ntt_kernel.cpp 的 Configurable_PE 数学等价，但独立实现，避免跨文件调用、
// 消除 HLS 在不同顶层模块间共享同一 PE 实例时的资源/时序耦合。
//
// NTT 蝶形（Cooley-Tukey）：
//   t      = v * tf  mod q
//   out_u  = (u + t) mod q
//   out_v  = (u - t) mod q
//
// INTT 蝶形（Gentleman-Sande）+ /2：
//   s      = (u + v) mod q
//   d      = (u - v) mod q
//   t      = d * tf  mod q
//   out_u  = s / 2 mod q       （奇数则加 (q+1)/2）
//   out_v  = t / 2 mod q
// ============================================================
static void CG_PE(
    const uint64_t &input1,
    const uint64_t &input2,
    const uint64_t &twiddle_factor,
    uint64_t &res1,
    uint64_t &res2,
    const uint64_t &modulus,
    const uint64_t &K_HALF,
    const uint64_t &M,
    const bool &is_ntt
) {
    #pragma HLS INLINE
    uint64_t temp, temp1;
    uint64_t input1_temp = input1;
    uint64_t input2_temp = input2;
    uint64_t res1_temp, res2_temp;

    if (is_ntt) {
        MultMod(input2_temp, twiddle_factor, modulus, M, K_HALF, temp);

        AddMod(input1_temp, temp, modulus, true);
        res1_temp = input1_temp;

        input1_temp = input1;
        AddMod(input1_temp, temp, modulus, false);
        res2_temp = input1_temp;

        res1 = res1_temp;
        res2 = res2_temp;
    } else {
        AddMod(input1_temp, input2_temp, modulus, true);
        temp1 = input1_temp;

        input1_temp = input1;
        AddMod(input1_temp, input2_temp, modulus, false);
        res2_temp = input1_temp;

        res1 = (temp1 >> 1) + ((temp1 & 1) ? ((modulus + 1) >> 1) : 0);

        // --- 强制打拍：虚拟加法器方案 ---
        // 我们做一个 +0 的操作，并强制要求这个操作 latency=1
        // 这会迫使 HLS 在 mult_in_reg 处插入真实的物理寄存器，切断 AddMod 的路径
        uint64_t mult_in = res2_temp + 0;
        #pragma HLS BIND_OP variable=mult_in op=add impl=fabric latency=1

        MultMod(mult_in, twiddle_factor, modulus, M, K_HALF, temp);

        // INTT 结果除以 2（乘以 2 的逆元），同时处理奇数情况（模加上半模）
        res2 = (temp >> 1) + ((temp & 1) ? ((modulus + 1) >> 1) : 0);
    }
}

// ============================================================
// CG-NTT 输出重排：还原为标准 NTT 输出顺序
//
// CG-NTT 经过 STAGE 次 perfect shuffle 后，数据位于排列 perm[] 处。
// 逆映射：result[perm[i]] = data[i]  → 即 result[物理位置] = data[乱序位置]
//
// perfect shuffle 定义：
//   new[2*i]     = old[i]           (前半段 → 偶数位)
//   new[2*i + 1] = old[i + N/2]     (后半段 → 奇数位)
// ============================================================

void cg_ntt_reorder(uint64_t data[RING_DIM]) {
    #pragma HLS INLINE off

    // CG-NTT 输出为 bit-reversed 顺序（相对于标准 NTT 输出）。
    // 只需执行 bit-reversal 排列即可还原为标准顺序。
    uint64_t temp[RING_DIM];

    REORDER_LOOP:
    for (int i = 0; i < RING_DIM; i++) {
        #pragma HLS PIPELINE II=1
        // bit-reverse index i (STAGE bits = log2(RING_DIM) bits)
        int rev = 0;
        int x = i;
        for (int b = 0; b < STAGE; b++) {
            rev = (rev << 1) | (x & 1);
            x >>= 1;
        }
        temp[rev] = data[i];
    }

    // 写回
    WRITEBACK_REORDER:
    for (int i = 0; i < RING_DIM; i++) {
        #pragma HLS PIPELINE II=1
        data[i] = temp[i];
    }
}

// ============================================================
// CG_NTT_Kernel：单 limb CG-NTT / INTT 核心
// ============================================================

void CG_NTT_Kernel(
    const uint64_t in_data[RING_DIM],
    uint64_t out_data[RING_DIM],
    const uint64_t modulus,
    const uint64_t K_HALF,
    const uint64_t M_barrett,
    const uint64_t cg_twiddle[STAGE][CG_HALF_N],
    bool is_ntt
) {
    // ============================================================
    // 乒乓缓冲：消除 RAW 依赖
    // NTT 方向：读 [i, i+N/2]，写 [2i, 2i+1]（perfect shuffle）
    // INTT 方向：读 [2i, 2i+1]，写 [i, i+N/2]（perfect unshuffle）
    // ============================================================
    uint64_t buf_A[RING_DIM];
    uint64_t buf_B[RING_DIM];

    #pragma HLS ARRAY_PARTITION variable=buf_A cyclic factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=buf_B cyclic factor=16 dim=1
    #pragma HLS BIND_STORAGE variable=buf_A type=ram_2p impl=bram
    #pragma HLS BIND_STORAGE variable=buf_B type=ram_2p impl=bram

    // 旋转因子：stage 维 complete（8 副本），位置维 cyclic 按 PE_PARALLEL
    // 保证 8 个 PE 同时读 cg_twiddle[s][i*8+0..7] 无冲突
    #pragma HLS ARRAY_PARTITION variable=cg_twiddle cyclic factor=CG_PE_NUM dim=2

    // ============================================================
    // 初始化：in_data → buf_A
    // ============================================================
    INIT_LOOP:
    for (int i = 0; i < RING_DIM / CG_PE_NUM; i++) {
        #pragma HLS PIPELINE II=1
        CG_INIT_PE:
        for (int p = 0; p < CG_PE_NUM; p++) {
            #pragma HLS UNROLL
            buf_A[i * CG_PE_NUM + p] = in_data[i * CG_PE_NUM + p];
        }
    }

    // ============================================================
    // 主循环：STAGE 层 × (CG_HALF_N / CG_PE_NUM) 次迭代
    // 乒乓协议：偶数 stage 读 A 写 B，奇数 stage 读 B 写 A
    //
    // NTT（正向）：stage 0→11，每层 shuffle 写
    //   读 buf[i] 和 buf[i+N/2]，写 buf[2i] 和 buf[2i+1]
    //
    // INTT（逆向）：stage 11→0，每层 unshuffle 写
    //   读 buf[2i] 和 buf[2i+1]，写 buf[i] 和 buf[i+N/2]
    // ============================================================
    STAGE_LOOP:
    for (int stage = 0; stage < STAGE; stage++) {
        // 禁止与外层/内层展平，保证 ping-pong 在 stage 边界完成排空
        #pragma HLS LOOP_FLATTEN off

        // NTT 正序（0,1,...,11），INTT 逆序（11,10,...,0）
        int actual_stage = is_ntt ? stage : (STAGE - 1 - stage);

        BUTTERFLY_LOOP:
        for (int i = 0; i < CG_HALF_N / CG_PE_NUM; i++) {
            // 带宽核算：buf_A/buf_B cyclic factor=16 + ram_2p → 32 访问/周期，
            // 每迭代实际需 8 PE × (2 读 + 2 写) = 32 访问，刚好吻合，II=1 可行。
            #pragma HLS PIPELINE II=1
            // 解除 HLS 对 buf_A/buf_B 的假性依赖（ping-pong 保证跨迭代/迭代内均无地址冲突）
            #pragma HLS DEPENDENCE variable=buf_A type=inter dependent=false
            #pragma HLS DEPENDENCE variable=buf_B type=inter dependent=false
            #pragma HLS DEPENDENCE variable=buf_A type=intra dependent=false
            #pragma HLS DEPENDENCE variable=buf_B type=intra dependent=false

            PE_UNROLL:
            for (int p = 0; p < CG_PE_NUM; p++) {
                #pragma HLS UNROLL

                int global_i = i * CG_PE_NUM + p;  // 0 ~ CG_HALF_N-1

                uint64_t u, v;
                if (is_ntt) {
                    // NTT 读：固定跨度 [global_i, global_i + N/2]
                    if ((stage & 1) == 0) {
                        u = buf_A[global_i];
                        v = buf_A[global_i + CG_HALF_N];
                    } else {
                        u = buf_B[global_i];
                        v = buf_B[global_i + CG_HALF_N];
                    }
                } else {
                    // INTT 读：连续对 [2*global_i, 2*global_i + 1]
                    if ((stage & 1) == 0) {
                        u = buf_A[2 * global_i];
                        v = buf_A[2 * global_i + 1];
                    } else {
                        u = buf_B[2 * global_i];
                        v = buf_B[2 * global_i + 1];
                    }
                }

                // 这将彻底斩断 16x16 交叉开关 到 DSP 乘法器 之间的致命走线约束。
                #pragma HLS LATENCY min=1 max=1

                // ② 顺序读取旋转因子（无运行时索引计算！）
                uint64_t tf = cg_twiddle[actual_stage][global_i];

                // ③ 蝶形运算（CG-NTT 专用 PE，与 NTT_Kernel 完全独立）
                uint64_t out_u, out_v;
                CG_PE(u, v, tf, out_u, out_v, modulus, K_HALF, M_barrett, is_ntt);

                if (is_ntt) {
                    // NTT 写：完美洗牌 [2*global_i, 2*global_i + 1]
                    if ((stage & 1) == 0) {
                        buf_B[2 * global_i]     = out_u;
                        buf_B[2 * global_i + 1] = out_v;
                    } else {
                        buf_A[2 * global_i]     = out_u;
                        buf_A[2 * global_i + 1] = out_v;
                    }
                } else {
                    // INTT 写：完美逆洗牌 [global_i, global_i + N/2]
                    if ((stage & 1) == 0) {
                        buf_B[global_i]              = out_u;
                        buf_B[global_i + CG_HALF_N]  = out_v;
                    } else {
                        buf_A[global_i]              = out_u;
                        buf_A[global_i + CG_HALF_N]  = out_v;
                    }
                }
            }
        }
    }

    // ============================================================
    // 回写到 out_data
    // STAGE=12（偶数）→ 最后写的 stage=11（奇）→ 写入 buf_A → 结果在 buf_A
    // 通用：偶数 STAGE 结果在 buf_A，奇数 STAGE 结果在 buf_B
    // ============================================================
    WRITEBACK_LOOP:
    for (int i = 0; i < RING_DIM / CG_PE_NUM; i++) {
        #pragma HLS PIPELINE II=1
        CG_WB_PE:
        for (int p = 0; p < CG_PE_NUM; p++) {
            #pragma HLS UNROLL
            if ((STAGE & 1) == 0) {
                out_data[i * CG_PE_NUM + p] = buf_A[i * CG_PE_NUM + p];
            } else {
                out_data[i * CG_PE_NUM + p] = buf_B[i * CG_PE_NUM + p];
            }
        }
    }
}

// ============================================================
// Compute_CG_NTT：多 limb 包装器
// 与现有 Compute_NTT 接口风格一致
// ============================================================

void Compute_CG_NTT(
    ap_uint<512> *in_data,
    const ap_uint<512> *cg_ntt_twiddle,
    const ap_uint<512> *cg_intt_twiddle,
    const uint64_t modulus[MAX_LIMBS],
    const uint64_t K_HALF[MAX_LIMBS],
    const uint64_t M_barrett[MAX_LIMBS],
    bool is_ntt,
    int num_active_limbs,
    int mod_idx_offset
) {
    // ============================================================
    // 顶层 AXI 接口配置
    // ap_uint<512> 指针 → HLS 自动生成 512-bit m_axi 端口
    // ============================================================
   #pragma HLS INTERFACE m_axi port=in_data         bundle=gmem0 offset=slave depth=4096
    #pragma HLS INTERFACE m_axi port=cg_ntt_twiddle  bundle=gmem1 offset=slave depth=24576
    #pragma HLS INTERFACE m_axi port=cg_intt_twiddle bundle=gmem2 offset=slave depth=24576
    #pragma HLS INTERFACE m_axi port=modulus         bundle=gmem3 offset=slave depth=8
    #pragma HLS INTERFACE m_axi port=K_HALF          bundle=gmem3 offset=slave depth=8
    #pragma HLS INTERFACE m_axi port=M_barrett       bundle=gmem3 offset=slave depth=8

    #pragma HLS INTERFACE s_axilite port=in_data
    #pragma HLS INTERFACE s_axilite port=cg_ntt_twiddle
    #pragma HLS INTERFACE s_axilite port=cg_intt_twiddle
    #pragma HLS INTERFACE s_axilite port=modulus
    #pragma HLS INTERFACE s_axilite port=K_HALF
    #pragma HLS INTERFACE s_axilite port=M_barrett
    #pragma HLS INTERFACE s_axilite port=is_ntt
    #pragma HLS INTERFACE s_axilite port=num_active_limbs
    #pragma HLS INTERFACE s_axilite port=mod_idx_offset
    #pragma HLS INTERFACE s_axilite port=return

    // ============================================================
    // 片上缓冲（Local BRAM/URAM）
    // ============================================================
    uint64_t local_in_data[RING_DIM];
    uint64_t local_out_data[RING_DIM];
    uint64_t local_twiddle[STAGE][CG_HALF_N];

    #pragma HLS ARRAY_PARTITION variable=local_in_data  cyclic factor=CG_PE_NUM dim=1
    #pragma HLS ARRAY_PARTITION variable=local_out_data cyclic factor=CG_PE_NUM dim=1
    #pragma HLS ARRAY_PARTITION variable=local_twiddle  cyclic factor=CG_PE_NUM dim=2

    LIMB_LOOP:
    for (int l = mod_idx_offset; l < mod_idx_offset + num_active_limbs; l++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=8 avg=3

        // 512-bit burst 读入 in_data → local_in_data
        LOAD_IN:
        for (int i = 0; i < PACKED_RING_DIM; i++) {
            #pragma HLS PIPELINE II=1
            ap_uint<512> pack = in_data[l * PACKED_RING_DIM + i];
            for (int j = 0; j < PACK_RATIO; j++) {
                #pragma HLS UNROLL
                local_in_data[i * PACK_RATIO + j] = pack.range((j + 1) * 64 - 1, j * 64);
            }
        }

        // 512-bit burst 读入旋转因子 → local_twiddle
        if (is_ntt) {
            LOAD_NTT_TW:
            for (int i = 0; i < PACKED_TW_SIZE; i++) {
                #pragma HLS PIPELINE II=1
                ap_uint<512> pack = cg_ntt_twiddle[l * PACKED_TW_SIZE + i];
                int base = i * PACK_RATIO;
                for (int j = 0; j < PACK_RATIO; j++) {
                    #pragma HLS UNROLL
                    int idx = base + j;
                    local_twiddle[idx / CG_HALF_N][idx % CG_HALF_N] = pack.range((j + 1) * 64 - 1, j * 64);
                }
            }
        } else {
            LOAD_INTT_TW:
            for (int i = 0; i < PACKED_TW_SIZE; i++) {
                #pragma HLS PIPELINE II=1
                ap_uint<512> pack = cg_intt_twiddle[l * PACKED_TW_SIZE + i];
                int base = i * PACK_RATIO;
                for (int j = 0; j < PACK_RATIO; j++) {
                    #pragma HLS UNROLL
                    int idx = base + j;
                    local_twiddle[idx / CG_HALF_N][idx % CG_HALF_N] = pack.range((j + 1) * 64 - 1, j * 64);
                }
            }
        }

        // 计算（全部在片上 BRAM 完成）
        CG_NTT_Kernel(
            local_in_data,
            local_out_data,
            modulus[l],
            K_HALF[l],
            M_barrett[l],
            local_twiddle,
            is_ntt
        );

        // 512-bit burst 写回 local_out_data → in_data
        STORE_OUT:
        for (int i = 0; i < PACKED_RING_DIM; i++) {
            #pragma HLS PIPELINE II=1
            ap_uint<512> pack;
            for (int j = 0; j < PACK_RATIO; j++) {
                #pragma HLS UNROLL
                pack.range((j + 1) * 64 - 1, j * 64) = local_out_data[i * PACK_RATIO + j];
            }
            in_data[l * PACKED_RING_DIM + i] = pack;
        }
    }
}
