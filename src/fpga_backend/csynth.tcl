open_project Solution
# 设置你的顶层函数名（请确保它与 C++ 代码中的函数名一致）
set_top Top

set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# ================= 修改开始 =================

# Add all source files (no comments inside block - TCL treats words as file names)
add_files {
    ./src/top.cpp
    ./src/load.cpp
    ./src/arithmetic.cpp
    ./src/bconv.cpp
    ./src/auto.cpp
    ./src/mod_add_kernel.cpp
    ./src/mod_sub_kernel.cpp
    ./src/mod_mult_kernel.cpp
    ./src/ntt_kernel.cpp
    ./src/interleave.cpp
} -cflags $my_cflags

# ================= 修改结束 =================

# 注意：如果有测试文件（包含 main 函数），请使用 -tb 参数单独添加
# add_files -tb ./src/testbench.cpp -cflags $my_cflags

open_solution "solution1"
set_part xcu250-figd2104-2l-e
create_clock -period 4ns

csynth_design
exit