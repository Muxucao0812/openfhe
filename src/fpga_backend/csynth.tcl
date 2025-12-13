open_project Solution
set_top Top;


set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# 1. 添加设计文件 (src)
foreach source_file [glob -nocomplain ./src/*.cpp] {
    add_files $source_file -cflags $my_cflags
}
# add_files ./src/top.cpp -cflags $my_cflags


open_solution "solution1"
set_part xcu250-figd2104-2l-e
create_clock -period 4ns

csynth_design
exit
