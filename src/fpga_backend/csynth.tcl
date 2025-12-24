open_project Solution
# 设置你的顶层函数名（请确保它与 C++ 代码中的函数名一致）
set_top Auto

set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# ================= 修改开始 =================

add_files {
    ./src/auto.cpp

} -cflags $my_cflags

# ================= 修改结束 =================

# 注意：如果有测试文件（包含 main 函数），请使用 -tb 参数单独添加
# add_files -tb ./src/testbench.cpp -cflags $my_cflags

open_solution "solution1"
set_part xcu250-figd2104-2l-e
create_clock -period 4ns

csynth_design
exit