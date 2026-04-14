open_project Solution

set_top Compute_BConv

set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# 添加源文件（需包含 arithmetic.cpp，bconv.cpp 依赖 MultMod）
add_files {
    ./src/arithmetic.cpp
    ./src/bconv.cpp
} -cflags $my_cflags

# 添加测试文件（只包含 bconv_tb.cpp，避免多 main 冲突）
add_files ./testbench/bconv_tb.cpp -cflags $my_cflags -tb

open_solution "solution1"
set_part xcu55c-fsvh2892-2L-e
create_clock -period 5ns

# 清理旧的编译缓存，防止遗留 obj 文件引发 multiple definition 错误
file delete -force Solution/solution1/csim

# 运行 C 仿真
csim_design

exit