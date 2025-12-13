open_project Solution
set_top AddMod   ;# 这里设为 AddMod 或者你的顶层函数名都可以

set my_cflags "-I./include -I/opt/xilinx/xrt/include"

# 1. 添加设计文件 (src)
foreach source_file [glob -nocomplain ./src/*.cpp] {
    add_files $source_file -cflags $my_cflags
}

foreach test_file [glob -nocomplain ./test/*.cpp] {
    add_files $test_file -cflags $my_cflags -tb
}
open_solution "solution1"
set_part xcu250-figd2104-2l-e
create_clock -period 4ns

csim_design
exit