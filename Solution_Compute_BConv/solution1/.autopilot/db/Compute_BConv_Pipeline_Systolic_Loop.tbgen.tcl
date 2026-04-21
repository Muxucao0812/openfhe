set moduleName Compute_BConv_Pipeline_Systolic_Loop
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set C_modelName {Compute_BConv_Pipeline_Systolic_Loop}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict local_out_x_8 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_7 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_6 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_5 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_in_x { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_in_x_3 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_in_x_4 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ cmp261_i_4 int 1 regular  }
	{ icmp42 int 1 regular  }
	{ cmp261_i_2 int 1 regular  }
	{ icmp int 1 regular  }
	{ local_out_x_8 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_7 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_6 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_5 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_in_x int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ local_in_x_3 int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ local_in_x_4 int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ p_reload116 int 64 regular  }
	{ mod_p_reload int 64 regular  }
	{ p_reload139 int 64 regular  }
	{ p_reload140 int 64 regular  }
	{ p_reload117 int 64 regular  }
	{ mod_p_1_reload int 64 regular  }
	{ p_reload141 int 64 regular  }
	{ p_reload142 int 64 regular  }
	{ p_reload118 int 64 regular  }
	{ mod_p_2_reload int 64 regular  }
	{ p_reload143 int 64 regular  }
	{ p_reload144 int 64 regular  }
	{ p_reload119 int 64 regular  }
	{ p_reload147 int 64 regular  }
	{ p_reload145 int 64 regular  }
	{ p_reload146 int 64 regular  }
	{ p_reload115 int 64 regular  }
	{ mod_p_4_reload int 64 regular  }
	{ p_reload137 int 64 regular  }
	{ p_reload138 int 64 regular  }
	{ p_reload121 int 64 regular  }
	{ p_reload122 int 64 regular  }
	{ p_reload123 int 64 regular  }
	{ p_reload124 int 64 regular  }
	{ p_reload120 int 64 regular  }
	{ p_reload111 int 64 regular  }
	{ p_reload112 int 64 regular  }
	{ p_reload113 int 64 regular  }
	{ p_reload114 int 64 regular  }
	{ p_reload int 64 regular  }
	{ icmp_ln166 int 1 regular  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "cmp261_i_4", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "icmp42", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "cmp261_i_2", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "icmp", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "local_out_x_8", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_7", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_6", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_5", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x_3", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x_4", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload116", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "mod_p_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload139", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload140", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload117", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "mod_p_1_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload141", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload142", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload118", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "mod_p_2_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload143", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload144", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload119", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload147", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload145", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload146", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload115", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "mod_p_4_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload137", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload138", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload121", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload122", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload123", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload124", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload120", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload111", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload112", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload113", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload114", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "p_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "icmp_ln166", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 70
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ cmp261_i_4 sc_in sc_lv 1 signal 0 } 
	{ icmp42 sc_in sc_lv 1 signal 1 } 
	{ cmp261_i_2 sc_in sc_lv 1 signal 2 } 
	{ icmp sc_in sc_lv 1 signal 3 } 
	{ local_out_x_8_address1 sc_out sc_lv 12 signal 4 } 
	{ local_out_x_8_ce1 sc_out sc_logic 1 signal 4 } 
	{ local_out_x_8_we1 sc_out sc_logic 1 signal 4 } 
	{ local_out_x_8_d1 sc_out sc_lv 64 signal 4 } 
	{ local_out_x_7_address1 sc_out sc_lv 12 signal 5 } 
	{ local_out_x_7_ce1 sc_out sc_logic 1 signal 5 } 
	{ local_out_x_7_we1 sc_out sc_logic 1 signal 5 } 
	{ local_out_x_7_d1 sc_out sc_lv 64 signal 5 } 
	{ local_out_x_6_address1 sc_out sc_lv 12 signal 6 } 
	{ local_out_x_6_ce1 sc_out sc_logic 1 signal 6 } 
	{ local_out_x_6_we1 sc_out sc_logic 1 signal 6 } 
	{ local_out_x_6_d1 sc_out sc_lv 64 signal 6 } 
	{ local_out_x_5_address1 sc_out sc_lv 12 signal 7 } 
	{ local_out_x_5_ce1 sc_out sc_logic 1 signal 7 } 
	{ local_out_x_5_we1 sc_out sc_logic 1 signal 7 } 
	{ local_out_x_5_d1 sc_out sc_lv 64 signal 7 } 
	{ local_out_x_address1 sc_out sc_lv 12 signal 8 } 
	{ local_out_x_ce1 sc_out sc_logic 1 signal 8 } 
	{ local_out_x_we1 sc_out sc_logic 1 signal 8 } 
	{ local_out_x_d1 sc_out sc_lv 64 signal 8 } 
	{ local_in_x_address0 sc_out sc_lv 12 signal 9 } 
	{ local_in_x_ce0 sc_out sc_logic 1 signal 9 } 
	{ local_in_x_q0 sc_in sc_lv 64 signal 9 } 
	{ local_in_x_3_address0 sc_out sc_lv 12 signal 10 } 
	{ local_in_x_3_ce0 sc_out sc_logic 1 signal 10 } 
	{ local_in_x_3_q0 sc_in sc_lv 64 signal 10 } 
	{ local_in_x_4_address0 sc_out sc_lv 12 signal 11 } 
	{ local_in_x_4_ce0 sc_out sc_logic 1 signal 11 } 
	{ local_in_x_4_q0 sc_in sc_lv 64 signal 11 } 
	{ p_reload116 sc_in sc_lv 64 signal 12 } 
	{ mod_p_reload sc_in sc_lv 64 signal 13 } 
	{ p_reload139 sc_in sc_lv 64 signal 14 } 
	{ p_reload140 sc_in sc_lv 64 signal 15 } 
	{ p_reload117 sc_in sc_lv 64 signal 16 } 
	{ mod_p_1_reload sc_in sc_lv 64 signal 17 } 
	{ p_reload141 sc_in sc_lv 64 signal 18 } 
	{ p_reload142 sc_in sc_lv 64 signal 19 } 
	{ p_reload118 sc_in sc_lv 64 signal 20 } 
	{ mod_p_2_reload sc_in sc_lv 64 signal 21 } 
	{ p_reload143 sc_in sc_lv 64 signal 22 } 
	{ p_reload144 sc_in sc_lv 64 signal 23 } 
	{ p_reload119 sc_in sc_lv 64 signal 24 } 
	{ p_reload147 sc_in sc_lv 64 signal 25 } 
	{ p_reload145 sc_in sc_lv 64 signal 26 } 
	{ p_reload146 sc_in sc_lv 64 signal 27 } 
	{ p_reload115 sc_in sc_lv 64 signal 28 } 
	{ mod_p_4_reload sc_in sc_lv 64 signal 29 } 
	{ p_reload137 sc_in sc_lv 64 signal 30 } 
	{ p_reload138 sc_in sc_lv 64 signal 31 } 
	{ p_reload121 sc_in sc_lv 64 signal 32 } 
	{ p_reload122 sc_in sc_lv 64 signal 33 } 
	{ p_reload123 sc_in sc_lv 64 signal 34 } 
	{ p_reload124 sc_in sc_lv 64 signal 35 } 
	{ p_reload120 sc_in sc_lv 64 signal 36 } 
	{ p_reload111 sc_in sc_lv 64 signal 37 } 
	{ p_reload112 sc_in sc_lv 64 signal 38 } 
	{ p_reload113 sc_in sc_lv 64 signal 39 } 
	{ p_reload114 sc_in sc_lv 64 signal 40 } 
	{ p_reload sc_in sc_lv 64 signal 41 } 
	{ icmp_ln166 sc_in sc_lv 1 signal 42 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "cmp261_i_4", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "cmp261_i_4", "role": "default" }} , 
 	{ "name": "icmp42", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "icmp42", "role": "default" }} , 
 	{ "name": "cmp261_i_2", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "cmp261_i_2", "role": "default" }} , 
 	{ "name": "icmp", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "icmp", "role": "default" }} , 
 	{ "name": "local_out_x_8_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_8", "role": "address1" }} , 
 	{ "name": "local_out_x_8_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_8", "role": "ce1" }} , 
 	{ "name": "local_out_x_8_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_8", "role": "we1" }} , 
 	{ "name": "local_out_x_8_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_8", "role": "d1" }} , 
 	{ "name": "local_out_x_7_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_7", "role": "address1" }} , 
 	{ "name": "local_out_x_7_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_7", "role": "ce1" }} , 
 	{ "name": "local_out_x_7_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_7", "role": "we1" }} , 
 	{ "name": "local_out_x_7_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_7", "role": "d1" }} , 
 	{ "name": "local_out_x_6_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_6", "role": "address1" }} , 
 	{ "name": "local_out_x_6_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_6", "role": "ce1" }} , 
 	{ "name": "local_out_x_6_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_6", "role": "we1" }} , 
 	{ "name": "local_out_x_6_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_6", "role": "d1" }} , 
 	{ "name": "local_out_x_5_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_5", "role": "address1" }} , 
 	{ "name": "local_out_x_5_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_5", "role": "ce1" }} , 
 	{ "name": "local_out_x_5_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_5", "role": "we1" }} , 
 	{ "name": "local_out_x_5_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_5", "role": "d1" }} , 
 	{ "name": "local_out_x_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x", "role": "address1" }} , 
 	{ "name": "local_out_x_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x", "role": "ce1" }} , 
 	{ "name": "local_out_x_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x", "role": "we1" }} , 
 	{ "name": "local_out_x_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x", "role": "d1" }} , 
 	{ "name": "local_in_x_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x", "role": "address0" }} , 
 	{ "name": "local_in_x_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x", "role": "ce0" }} , 
 	{ "name": "local_in_x_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x", "role": "q0" }} , 
 	{ "name": "local_in_x_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_3", "role": "address0" }} , 
 	{ "name": "local_in_x_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_3", "role": "ce0" }} , 
 	{ "name": "local_in_x_3_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_3", "role": "q0" }} , 
 	{ "name": "local_in_x_4_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_4", "role": "address0" }} , 
 	{ "name": "local_in_x_4_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_4", "role": "ce0" }} , 
 	{ "name": "local_in_x_4_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_4", "role": "q0" }} , 
 	{ "name": "p_reload116", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload116", "role": "default" }} , 
 	{ "name": "mod_p_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "mod_p_reload", "role": "default" }} , 
 	{ "name": "p_reload139", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload139", "role": "default" }} , 
 	{ "name": "p_reload140", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload140", "role": "default" }} , 
 	{ "name": "p_reload117", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload117", "role": "default" }} , 
 	{ "name": "mod_p_1_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "mod_p_1_reload", "role": "default" }} , 
 	{ "name": "p_reload141", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload141", "role": "default" }} , 
 	{ "name": "p_reload142", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload142", "role": "default" }} , 
 	{ "name": "p_reload118", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload118", "role": "default" }} , 
 	{ "name": "mod_p_2_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "mod_p_2_reload", "role": "default" }} , 
 	{ "name": "p_reload143", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload143", "role": "default" }} , 
 	{ "name": "p_reload144", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload144", "role": "default" }} , 
 	{ "name": "p_reload119", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload119", "role": "default" }} , 
 	{ "name": "p_reload147", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload147", "role": "default" }} , 
 	{ "name": "p_reload145", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload145", "role": "default" }} , 
 	{ "name": "p_reload146", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload146", "role": "default" }} , 
 	{ "name": "p_reload115", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload115", "role": "default" }} , 
 	{ "name": "mod_p_4_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "mod_p_4_reload", "role": "default" }} , 
 	{ "name": "p_reload137", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload137", "role": "default" }} , 
 	{ "name": "p_reload138", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload138", "role": "default" }} , 
 	{ "name": "p_reload121", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload121", "role": "default" }} , 
 	{ "name": "p_reload122", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload122", "role": "default" }} , 
 	{ "name": "p_reload123", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload123", "role": "default" }} , 
 	{ "name": "p_reload124", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload124", "role": "default" }} , 
 	{ "name": "p_reload120", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload120", "role": "default" }} , 
 	{ "name": "p_reload111", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload111", "role": "default" }} , 
 	{ "name": "p_reload112", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload112", "role": "default" }} , 
 	{ "name": "p_reload113", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload113", "role": "default" }} , 
 	{ "name": "p_reload114", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload114", "role": "default" }} , 
 	{ "name": "p_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "p_reload", "role": "default" }} , 
 	{ "name": "icmp_ln166", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "icmp_ln166", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "5", "9", "13", "17", "21", "25", "29", "33", "37", "41", "45", "49", "53", "57", "61"],
		"CDFG" : "Compute_BConv_Pipeline_Systolic_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "4126", "EstimateLatencyMax" : "4126",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "cmp261_i_4", "Type" : "None", "Direction" : "I"},
			{"Name" : "icmp42", "Type" : "None", "Direction" : "I"},
			{"Name" : "cmp261_i_2", "Type" : "None", "Direction" : "I"},
			{"Name" : "icmp", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_out_x_8", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_7", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_6", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_5", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_in_x", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_in_x_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_in_x_4", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "p_reload116", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_p_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload139", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload140", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload117", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_p_1_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload141", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload142", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload118", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_p_2_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload143", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload144", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload119", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload147", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload145", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload146", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload115", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_p_4_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload137", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload138", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload121", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload122", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload123", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload124", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload120", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload111", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload112", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload113", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload114", "Type" : "None", "Direction" : "I"},
			{"Name" : "p_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "icmp_ln166", "Type" : "None", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Systolic_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter18", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter18", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_858", "Parent" : "0", "Child" : ["2", "3", "4"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "2", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_858.mul_64ns_64ns_128_5_1_U39", "Parent" : "1"},
	{"ID" : "3", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_858.mul_64ns_64ns_128_5_1_U40", "Parent" : "1"},
	{"ID" : "4", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_858.mul_64s_64s_64_5_1_U41", "Parent" : "1"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_867", "Parent" : "0", "Child" : ["6", "7", "8"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_867.mul_64ns_64ns_128_5_1_U39", "Parent" : "5"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_867.mul_64ns_64ns_128_5_1_U40", "Parent" : "5"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_867.mul_64s_64s_64_5_1_U41", "Parent" : "5"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_876", "Parent" : "0", "Child" : ["10", "11", "12"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_876.mul_64ns_64ns_128_5_1_U39", "Parent" : "9"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_876.mul_64ns_64ns_128_5_1_U40", "Parent" : "9"},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_876.mul_64s_64s_64_5_1_U41", "Parent" : "9"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_885", "Parent" : "0", "Child" : ["14", "15", "16"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_885.mul_64ns_64ns_128_5_1_U39", "Parent" : "13"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_885.mul_64ns_64ns_128_5_1_U40", "Parent" : "13"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_885.mul_64s_64s_64_5_1_U41", "Parent" : "13"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_894", "Parent" : "0", "Child" : ["18", "19", "20"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_894.mul_64ns_64ns_128_5_1_U39", "Parent" : "17"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_894.mul_64ns_64ns_128_5_1_U40", "Parent" : "17"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_894.mul_64s_64s_64_5_1_U41", "Parent" : "17"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_903", "Parent" : "0", "Child" : ["22", "23", "24"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_903.mul_64ns_64ns_128_5_1_U39", "Parent" : "21"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_903.mul_64ns_64ns_128_5_1_U40", "Parent" : "21"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_903.mul_64s_64s_64_5_1_U41", "Parent" : "21"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_912", "Parent" : "0", "Child" : ["26", "27", "28"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_912.mul_64ns_64ns_128_5_1_U39", "Parent" : "25"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_912.mul_64ns_64ns_128_5_1_U40", "Parent" : "25"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_912.mul_64s_64s_64_5_1_U41", "Parent" : "25"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_921", "Parent" : "0", "Child" : ["30", "31", "32"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_921.mul_64ns_64ns_128_5_1_U39", "Parent" : "29"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_921.mul_64ns_64ns_128_5_1_U40", "Parent" : "29"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_921.mul_64s_64s_64_5_1_U41", "Parent" : "29"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_930", "Parent" : "0", "Child" : ["34", "35", "36"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "34", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_930.mul_64ns_64ns_128_5_1_U39", "Parent" : "33"},
	{"ID" : "35", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_930.mul_64ns_64ns_128_5_1_U40", "Parent" : "33"},
	{"ID" : "36", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_930.mul_64s_64s_64_5_1_U41", "Parent" : "33"},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_939", "Parent" : "0", "Child" : ["38", "39", "40"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "38", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_939.mul_64ns_64ns_128_5_1_U39", "Parent" : "37"},
	{"ID" : "39", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_939.mul_64ns_64ns_128_5_1_U40", "Parent" : "37"},
	{"ID" : "40", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_939.mul_64s_64s_64_5_1_U41", "Parent" : "37"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_948", "Parent" : "0", "Child" : ["42", "43", "44"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "42", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_948.mul_64ns_64ns_128_5_1_U39", "Parent" : "41"},
	{"ID" : "43", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_948.mul_64ns_64ns_128_5_1_U40", "Parent" : "41"},
	{"ID" : "44", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_948.mul_64s_64s_64_5_1_U41", "Parent" : "41"},
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_957", "Parent" : "0", "Child" : ["46", "47", "48"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "46", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_957.mul_64ns_64ns_128_5_1_U39", "Parent" : "45"},
	{"ID" : "47", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_957.mul_64ns_64ns_128_5_1_U40", "Parent" : "45"},
	{"ID" : "48", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_957.mul_64s_64s_64_5_1_U41", "Parent" : "45"},
	{"ID" : "49", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_966", "Parent" : "0", "Child" : ["50", "51", "52"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "50", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_966.mul_64ns_64ns_128_5_1_U39", "Parent" : "49"},
	{"ID" : "51", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_966.mul_64ns_64ns_128_5_1_U40", "Parent" : "49"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_966.mul_64s_64s_64_5_1_U41", "Parent" : "49"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_975", "Parent" : "0", "Child" : ["54", "55", "56"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "54", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_975.mul_64ns_64ns_128_5_1_U39", "Parent" : "53"},
	{"ID" : "55", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_975.mul_64ns_64ns_128_5_1_U40", "Parent" : "53"},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_975.mul_64s_64s_64_5_1_U41", "Parent" : "53"},
	{"ID" : "57", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_984", "Parent" : "0", "Child" : ["58", "59", "60"],
		"CDFG" : "MultMod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "0", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "15", "EstimateLatencyMin" : "15", "EstimateLatencyMax" : "15",
		"Combinational" : "0",
		"Datapath" : "1",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "a_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "b_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "mod_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "m_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "k_half_val", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "58", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_984.mul_64ns_64ns_128_5_1_U39", "Parent" : "57"},
	{"ID" : "59", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_984.mul_64ns_64ns_128_5_1_U40", "Parent" : "57"},
	{"ID" : "60", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_MultMod_fu_984.mul_64s_64s_64_5_1_U41", "Parent" : "57"},
	{"ID" : "61", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Pipeline_Systolic_Loop {
		cmp261_i_4 {Type I LastRead 0 FirstWrite -1}
		icmp42 {Type I LastRead 0 FirstWrite -1}
		cmp261_i_2 {Type I LastRead 0 FirstWrite -1}
		icmp {Type I LastRead 0 FirstWrite -1}
		local_out_x_8 {Type O LastRead -1 FirstWrite 18}
		local_out_x_7 {Type O LastRead -1 FirstWrite 18}
		local_out_x_6 {Type O LastRead -1 FirstWrite 18}
		local_out_x_5 {Type O LastRead -1 FirstWrite 18}
		local_out_x {Type O LastRead -1 FirstWrite 18}
		local_in_x {Type I LastRead 0 FirstWrite -1}
		local_in_x_3 {Type I LastRead 0 FirstWrite -1}
		local_in_x_4 {Type I LastRead 0 FirstWrite -1}
		p_reload116 {Type I LastRead 0 FirstWrite -1}
		mod_p_reload {Type I LastRead 0 FirstWrite -1}
		p_reload139 {Type I LastRead 0 FirstWrite -1}
		p_reload140 {Type I LastRead 0 FirstWrite -1}
		p_reload117 {Type I LastRead 0 FirstWrite -1}
		mod_p_1_reload {Type I LastRead 0 FirstWrite -1}
		p_reload141 {Type I LastRead 0 FirstWrite -1}
		p_reload142 {Type I LastRead 0 FirstWrite -1}
		p_reload118 {Type I LastRead 0 FirstWrite -1}
		mod_p_2_reload {Type I LastRead 0 FirstWrite -1}
		p_reload143 {Type I LastRead 0 FirstWrite -1}
		p_reload144 {Type I LastRead 0 FirstWrite -1}
		p_reload119 {Type I LastRead 0 FirstWrite -1}
		p_reload147 {Type I LastRead 0 FirstWrite -1}
		p_reload145 {Type I LastRead 0 FirstWrite -1}
		p_reload146 {Type I LastRead 0 FirstWrite -1}
		p_reload115 {Type I LastRead 0 FirstWrite -1}
		mod_p_4_reload {Type I LastRead 0 FirstWrite -1}
		p_reload137 {Type I LastRead 0 FirstWrite -1}
		p_reload138 {Type I LastRead 0 FirstWrite -1}
		p_reload121 {Type I LastRead 0 FirstWrite -1}
		p_reload122 {Type I LastRead 0 FirstWrite -1}
		p_reload123 {Type I LastRead 0 FirstWrite -1}
		p_reload124 {Type I LastRead 0 FirstWrite -1}
		p_reload120 {Type I LastRead 0 FirstWrite -1}
		p_reload111 {Type I LastRead 0 FirstWrite -1}
		p_reload112 {Type I LastRead 0 FirstWrite -1}
		p_reload113 {Type I LastRead 0 FirstWrite -1}
		p_reload114 {Type I LastRead 0 FirstWrite -1}
		p_reload {Type I LastRead 0 FirstWrite -1}
		icmp_ln166 {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	MultMod {
		a_val {Type I LastRead 0 FirstWrite -1}
		b_val {Type I LastRead 0 FirstWrite -1}
		mod_val {Type I LastRead 0 FirstWrite -1}
		m_val {Type I LastRead 0 FirstWrite -1}
		k_half_val {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "4126", "Max" : "4126"}
	, {"Name" : "Interval", "Min" : "4126", "Max" : "4126"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	cmp261_i_4 { ap_none {  { cmp261_i_4 in_data 0 1 } } }
	icmp42 { ap_none {  { icmp42 in_data 0 1 } } }
	cmp261_i_2 { ap_none {  { cmp261_i_2 in_data 0 1 } } }
	icmp { ap_none {  { icmp in_data 0 1 } } }
	local_out_x_8 { ap_memory {  { local_out_x_8_address1 MemPortADDR2 1 12 }  { local_out_x_8_ce1 MemPortCE2 1 1 }  { local_out_x_8_we1 MemPortWE2 1 1 }  { local_out_x_8_d1 MemPortDIN2 1 64 } } }
	local_out_x_7 { ap_memory {  { local_out_x_7_address1 MemPortADDR2 1 12 }  { local_out_x_7_ce1 MemPortCE2 1 1 }  { local_out_x_7_we1 MemPortWE2 1 1 }  { local_out_x_7_d1 MemPortDIN2 1 64 } } }
	local_out_x_6 { ap_memory {  { local_out_x_6_address1 MemPortADDR2 1 12 }  { local_out_x_6_ce1 MemPortCE2 1 1 }  { local_out_x_6_we1 MemPortWE2 1 1 }  { local_out_x_6_d1 MemPortDIN2 1 64 } } }
	local_out_x_5 { ap_memory {  { local_out_x_5_address1 MemPortADDR2 1 12 }  { local_out_x_5_ce1 MemPortCE2 1 1 }  { local_out_x_5_we1 MemPortWE2 1 1 }  { local_out_x_5_d1 MemPortDIN2 1 64 } } }
	local_out_x { ap_memory {  { local_out_x_address1 MemPortADDR2 1 12 }  { local_out_x_ce1 MemPortCE2 1 1 }  { local_out_x_we1 MemPortWE2 1 1 }  { local_out_x_d1 MemPortDIN2 1 64 } } }
	local_in_x { ap_memory {  { local_in_x_address0 mem_address 1 12 }  { local_in_x_ce0 mem_ce 1 1 }  { local_in_x_q0 mem_dout 0 64 } } }
	local_in_x_3 { ap_memory {  { local_in_x_3_address0 mem_address 1 12 }  { local_in_x_3_ce0 mem_ce 1 1 }  { local_in_x_3_q0 mem_dout 0 64 } } }
	local_in_x_4 { ap_memory {  { local_in_x_4_address0 mem_address 1 12 }  { local_in_x_4_ce0 mem_ce 1 1 }  { local_in_x_4_q0 mem_dout 0 64 } } }
	p_reload116 { ap_none {  { p_reload116 in_data 0 64 } } }
	mod_p_reload { ap_none {  { mod_p_reload in_data 0 64 } } }
	p_reload139 { ap_none {  { p_reload139 in_data 0 64 } } }
	p_reload140 { ap_none {  { p_reload140 in_data 0 64 } } }
	p_reload117 { ap_none {  { p_reload117 in_data 0 64 } } }
	mod_p_1_reload { ap_none {  { mod_p_1_reload in_data 0 64 } } }
	p_reload141 { ap_none {  { p_reload141 in_data 0 64 } } }
	p_reload142 { ap_none {  { p_reload142 in_data 0 64 } } }
	p_reload118 { ap_none {  { p_reload118 in_data 0 64 } } }
	mod_p_2_reload { ap_none {  { mod_p_2_reload in_data 0 64 } } }
	p_reload143 { ap_none {  { p_reload143 in_data 0 64 } } }
	p_reload144 { ap_none {  { p_reload144 in_data 0 64 } } }
	p_reload119 { ap_none {  { p_reload119 in_data 0 64 } } }
	p_reload147 { ap_none {  { p_reload147 in_data 0 64 } } }
	p_reload145 { ap_none {  { p_reload145 in_data 0 64 } } }
	p_reload146 { ap_none {  { p_reload146 in_data 0 64 } } }
	p_reload115 { ap_none {  { p_reload115 in_data 0 64 } } }
	mod_p_4_reload { ap_none {  { mod_p_4_reload in_data 0 64 } } }
	p_reload137 { ap_none {  { p_reload137 in_data 0 64 } } }
	p_reload138 { ap_none {  { p_reload138 in_data 0 64 } } }
	p_reload121 { ap_none {  { p_reload121 in_data 0 64 } } }
	p_reload122 { ap_none {  { p_reload122 in_data 0 64 } } }
	p_reload123 { ap_none {  { p_reload123 in_data 0 64 } } }
	p_reload124 { ap_none {  { p_reload124 in_data 0 64 } } }
	p_reload120 { ap_none {  { p_reload120 in_data 0 64 } } }
	p_reload111 { ap_none {  { p_reload111 in_data 0 64 } } }
	p_reload112 { ap_none {  { p_reload112 in_data 0 64 } } }
	p_reload113 { ap_none {  { p_reload113 in_data 0 64 } } }
	p_reload114 { ap_none {  { p_reload114 in_data 0 64 } } }
	p_reload { ap_none {  { p_reload in_data 0 64 } } }
	icmp_ln166 { ap_none {  { icmp_ln166 in_data 0 1 } } }
}
