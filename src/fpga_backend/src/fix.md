结合我们刚才分析的 时序违例（-0.33ns）、蝴蝶操作假性依赖（II违例） 以及 BRAM 端口冲突（II=3或4），我帮你把需要修改的地方精确定位出来了。

请按照下面的步骤，依次修改这 4 个文件：

1. 修改 arithmetic.cpp (解决最核心的时序违例)
目标：删掉没用的 LATENCY 约束，用 BIND_REGISTER 强行切断 MultMod 的超长组合逻辑。操作：找到 MultMod 函数的后半部分（大约在第 120 行之后，#else 分支里面），将它整体替换为以下代码：

C++
#else
    ap_uint<128> q_shifted = 0;
    if (S > 64) {
        q_shifted = (p_high >> (S - 64)) + (p_low >> S);
    } else if (S < 64) {
        q_shifted = (p_high << (64 - S)) + (p_low >> S);
    } else {
        q_shifted = p_high + (p_low >> 64);
    }

    // --- 修改点：强制打拍截断路径 ---
    ap_uint<128> q = q_shifted;
    #pragma HLS BIND_REGISTER variable=q

    ap_uint<128> q_times_mod = q * (ap_uint<128>)mod;
    #pragma HLS BIND_OP variable=q_times_mod op=mul impl=dsp latency=4
    
    ap_uint<128> r_full = res_mult - q_times_mod;
    uint64_t r_stg1 = (uint64_t)r_full;
    #pragma HLS BIND_REGISTER variable=r_stg1 

    uint64_t r_stg2 = (r_stg1 >= mod) ? (r_stg1 - mod) : r_stg1;
    #pragma HLS BIND_REGISTER variable=r_stg2

    uint64_t r_stg3 = (r_stg2 >= mod) ? (r_stg2 - mod) : r_stg2;
    #pragma HLS BIND_REGISTER variable=r_stg3

    res_mod = r_stg3;
#endif
2. 修改 ntt_kernel.cpp (配合修复时序)
目标：把 Configurable_PE 里面没用的 LATENCY 换成物理寄存器，保护 INTT 的流水线。操作：找到 Configurable_PE 函数里的 else 分支（处理 INTT 的地方，大约在第 230 行左右），将其替换为：

C++
    } else {
        AddMod(input1_temp, input2_temp, modulus, true);
        temp1 = input1_temp;

        input1_temp = input1;
        AddMod(input1_temp, input2_temp, modulus, false);
        res2_temp = input1_temp;

        res1 = (temp1 >> 1) + ((temp1 & 1) ? ((modulus + 1) >> 1) : 0);

        // --- 修改点：删掉 #pragma HLS LATENCY，改为寄存器打拍 ---
        uint64_t mult_in = res2_temp;
        #pragma HLS BIND_REGISTER variable=mult_in
        
        MultMod(mult_in, twiddle_factor, modulus, M, K_HALF, temp);

        res2 = (temp >> 1) + ((temp & 1) ? ((modulus + 1) >> 1) : 0);
    }
3. 修改 cg_ntt.cpp (解决蝴蝶操作 II 违例)
目标：解除 HLS 对 buf_A 和 buf_B 的读写恐惧，消除 False Dependency，让 II 降回 1。操作：找到 cg_ntt_kernel 函数，在 BUTTERFLY_LOOP: 这一行下面，紧接着加上这四行 Pragma：

C++
        BUTTERFLY_LOOP:
        // --- 修改点：告诉 HLS 这里的数组读写绝对不会地址冲突 ---
        #pragma HLS DEPENDENCE variable=buf_A type=inter dependent=false
        #pragma HLS DEPENDENCE variable=buf_B type=inter dependent=false
        #pragma HLS DEPENDENCE variable=buf_A type=intra dependent=false
        #pragma HLS DEPENDENCE variable=buf_B type=intra dependent=false
        for (int j = 0; j < RING_SIZE / PE_PARALLEL; j++) {
            #pragma HLS pipeline II=1
            // ... 后面的代码保持不变 ...
4. 修改 top.cpp (解决 BRAM 端口不够用的问题)
目标：避免 FLATTEN_ROW 和 RESHAPE_ROW 循环在展开（Unroll）时去抢夺同一个 BRAM 的读取端口。操作：在 top 函数的开头，找到三个大数组的声明和 #pragma HLS array_partition。把原来的 cyclic factor=PE_PARALLEL 改成 complete（完全打散该维度）：

C++
    // 假设这是你原来的定义处，将对应的 pragma 修改如下：
    uint64_t poly_buffer_1[MAX_ACTIVE_LIMBS][RING_SIZE / PE_PARALLEL][PE_PARALLEL];
    // --- 修改点：将 dim=3 完全展开为寄存器/独立BRAM，根除端口竞争 ---
    #pragma HLS array_partition variable=poly_buffer_1 complete dim=3

    uint64_t poly_buffer_2[MAX_ACTIVE_LIMBS][RING_SIZE / PE_PARALLEL][PE_PARALLEL];
    #pragma HLS array_partition variable=poly_buffer_2 complete dim=3

    uint64_t result_buffer[MAX_ACTIVE_LIMBS][RING_SIZE / PE_PARALLEL][PE_PARALLEL];
    #pragma HLS array_partition variable=result_buffer complete dim=3
(注：如果你的代码里原来用的是 cyclic，改成 complete 后综合出来的连线会更加直接且安全，完美适配你在内层循环写的 #pragma HLS unroll factor=PE_PARALLEL)。