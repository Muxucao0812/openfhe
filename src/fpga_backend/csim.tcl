open_project Solution

# 使用 Auto 作为顶层函数测试 CKKS automorphism
set_top BConv

set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# 添加源文件
add_files ./src/bconv.cpp -cflags $my_cflags

# 添加测试文件
add_files ./testbench/bconv_tb.cpp -cflags $my_cflags -tb

open_solution "solution1"
set_part xcu250-figd2104-2l-e
create_clock -period 4ns

# 运行 C 仿真
csim_design

exit