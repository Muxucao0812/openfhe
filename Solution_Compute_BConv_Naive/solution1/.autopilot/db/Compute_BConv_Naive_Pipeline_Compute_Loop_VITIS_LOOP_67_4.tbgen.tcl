set moduleName Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4
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
set C_modelName {Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict local_in_x { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_in_x_1 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_in_x_2 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_out_x { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_1 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_2 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_3 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_out_x_4 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
set C_modelArgList {
	{ sizeP int 32 regular  }
	{ zext_ln66_1 int 44 regular  }
	{ local_in_x int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ local_in_x_1 int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ local_in_x_2 int 64 regular {array 4096 { 1 3 } 1 1 }  }
	{ local_out_x int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_1 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_2 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_3 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_out_x_4 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_mod_reload int 64 regular  }
	{ local_mod_1_reload int 64 regular  }
	{ local_mod_2_reload int 64 regular  }
	{ local_mod_3_reload int 64 regular  }
	{ local_mod_4_reload int 64 regular  }
	{ local_w_reload int 64 regular  }
	{ local_w_1_reload int 64 regular  }
	{ local_w_2_reload int 64 regular  }
	{ local_w_3_reload int 64 regular  }
	{ local_w_4_reload int 64 regular  }
	{ local_w_5_reload int 64 regular  }
	{ local_w_6_reload int 64 regular  }
	{ local_w_7_reload int 64 regular  }
	{ local_w_8_reload int 64 regular  }
	{ local_w_9_reload int 64 regular  }
	{ local_w_10_reload int 64 regular  }
	{ local_w_11_reload int 64 regular  }
	{ local_w_12_reload int 64 regular  }
	{ local_w_13_reload int 64 regular  }
	{ local_w_14_reload int 64 regular  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "sizeP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "zext_ln66_1", "interface" : "wire", "bitwidth" : 44, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x_1", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x_2", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_out_x", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_1", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_2", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_3", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_out_x_4", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_mod_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_mod_1_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_mod_2_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_mod_3_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_mod_4_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_1_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_2_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_3_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_4_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_5_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_6_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_7_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_8_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_9_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_10_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_11_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_12_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_13_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_14_reload", "interface" : "wire", "bitwidth" : 64, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 57
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ sizeP sc_in sc_lv 32 signal 0 } 
	{ zext_ln66_1 sc_in sc_lv 44 signal 1 } 
	{ local_in_x_address0 sc_out sc_lv 12 signal 2 } 
	{ local_in_x_ce0 sc_out sc_logic 1 signal 2 } 
	{ local_in_x_q0 sc_in sc_lv 64 signal 2 } 
	{ local_in_x_1_address0 sc_out sc_lv 12 signal 3 } 
	{ local_in_x_1_ce0 sc_out sc_logic 1 signal 3 } 
	{ local_in_x_1_q0 sc_in sc_lv 64 signal 3 } 
	{ local_in_x_2_address0 sc_out sc_lv 12 signal 4 } 
	{ local_in_x_2_ce0 sc_out sc_logic 1 signal 4 } 
	{ local_in_x_2_q0 sc_in sc_lv 64 signal 4 } 
	{ local_out_x_address1 sc_out sc_lv 12 signal 5 } 
	{ local_out_x_ce1 sc_out sc_logic 1 signal 5 } 
	{ local_out_x_we1 sc_out sc_logic 1 signal 5 } 
	{ local_out_x_d1 sc_out sc_lv 64 signal 5 } 
	{ local_out_x_1_address1 sc_out sc_lv 12 signal 6 } 
	{ local_out_x_1_ce1 sc_out sc_logic 1 signal 6 } 
	{ local_out_x_1_we1 sc_out sc_logic 1 signal 6 } 
	{ local_out_x_1_d1 sc_out sc_lv 64 signal 6 } 
	{ local_out_x_2_address1 sc_out sc_lv 12 signal 7 } 
	{ local_out_x_2_ce1 sc_out sc_logic 1 signal 7 } 
	{ local_out_x_2_we1 sc_out sc_logic 1 signal 7 } 
	{ local_out_x_2_d1 sc_out sc_lv 64 signal 7 } 
	{ local_out_x_3_address1 sc_out sc_lv 12 signal 8 } 
	{ local_out_x_3_ce1 sc_out sc_logic 1 signal 8 } 
	{ local_out_x_3_we1 sc_out sc_logic 1 signal 8 } 
	{ local_out_x_3_d1 sc_out sc_lv 64 signal 8 } 
	{ local_out_x_4_address1 sc_out sc_lv 12 signal 9 } 
	{ local_out_x_4_ce1 sc_out sc_logic 1 signal 9 } 
	{ local_out_x_4_we1 sc_out sc_logic 1 signal 9 } 
	{ local_out_x_4_d1 sc_out sc_lv 64 signal 9 } 
	{ local_mod_reload sc_in sc_lv 64 signal 10 } 
	{ local_mod_1_reload sc_in sc_lv 64 signal 11 } 
	{ local_mod_2_reload sc_in sc_lv 64 signal 12 } 
	{ local_mod_3_reload sc_in sc_lv 64 signal 13 } 
	{ local_mod_4_reload sc_in sc_lv 64 signal 14 } 
	{ local_w_reload sc_in sc_lv 64 signal 15 } 
	{ local_w_1_reload sc_in sc_lv 64 signal 16 } 
	{ local_w_2_reload sc_in sc_lv 64 signal 17 } 
	{ local_w_3_reload sc_in sc_lv 64 signal 18 } 
	{ local_w_4_reload sc_in sc_lv 64 signal 19 } 
	{ local_w_5_reload sc_in sc_lv 64 signal 20 } 
	{ local_w_6_reload sc_in sc_lv 64 signal 21 } 
	{ local_w_7_reload sc_in sc_lv 64 signal 22 } 
	{ local_w_8_reload sc_in sc_lv 64 signal 23 } 
	{ local_w_9_reload sc_in sc_lv 64 signal 24 } 
	{ local_w_10_reload sc_in sc_lv 64 signal 25 } 
	{ local_w_11_reload sc_in sc_lv 64 signal 26 } 
	{ local_w_12_reload sc_in sc_lv 64 signal 27 } 
	{ local_w_13_reload sc_in sc_lv 64 signal 28 } 
	{ local_w_14_reload sc_in sc_lv 64 signal 29 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "sizeP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sizeP", "role": "default" }} , 
 	{ "name": "zext_ln66_1", "direction": "in", "datatype": "sc_lv", "bitwidth":44, "type": "signal", "bundle":{"name": "zext_ln66_1", "role": "default" }} , 
 	{ "name": "local_in_x_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x", "role": "address0" }} , 
 	{ "name": "local_in_x_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x", "role": "ce0" }} , 
 	{ "name": "local_in_x_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x", "role": "q0" }} , 
 	{ "name": "local_in_x_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "address0" }} , 
 	{ "name": "local_in_x_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "ce0" }} , 
 	{ "name": "local_in_x_1_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "q0" }} , 
 	{ "name": "local_in_x_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "address0" }} , 
 	{ "name": "local_in_x_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "ce0" }} , 
 	{ "name": "local_in_x_2_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "q0" }} , 
 	{ "name": "local_out_x_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x", "role": "address1" }} , 
 	{ "name": "local_out_x_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x", "role": "ce1" }} , 
 	{ "name": "local_out_x_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x", "role": "we1" }} , 
 	{ "name": "local_out_x_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x", "role": "d1" }} , 
 	{ "name": "local_out_x_1_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_1", "role": "address1" }} , 
 	{ "name": "local_out_x_1_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_1", "role": "ce1" }} , 
 	{ "name": "local_out_x_1_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_1", "role": "we1" }} , 
 	{ "name": "local_out_x_1_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_1", "role": "d1" }} , 
 	{ "name": "local_out_x_2_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_2", "role": "address1" }} , 
 	{ "name": "local_out_x_2_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_2", "role": "ce1" }} , 
 	{ "name": "local_out_x_2_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_2", "role": "we1" }} , 
 	{ "name": "local_out_x_2_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_2", "role": "d1" }} , 
 	{ "name": "local_out_x_3_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_3", "role": "address1" }} , 
 	{ "name": "local_out_x_3_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_3", "role": "ce1" }} , 
 	{ "name": "local_out_x_3_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_3", "role": "we1" }} , 
 	{ "name": "local_out_x_3_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_3", "role": "d1" }} , 
 	{ "name": "local_out_x_4_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_out_x_4", "role": "address1" }} , 
 	{ "name": "local_out_x_4_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_4", "role": "ce1" }} , 
 	{ "name": "local_out_x_4_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_out_x_4", "role": "we1" }} , 
 	{ "name": "local_out_x_4_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_out_x_4", "role": "d1" }} , 
 	{ "name": "local_mod_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_reload", "role": "default" }} , 
 	{ "name": "local_mod_1_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_1_reload", "role": "default" }} , 
 	{ "name": "local_mod_2_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_2_reload", "role": "default" }} , 
 	{ "name": "local_mod_3_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_3_reload", "role": "default" }} , 
 	{ "name": "local_mod_4_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_4_reload", "role": "default" }} , 
 	{ "name": "local_w_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_reload", "role": "default" }} , 
 	{ "name": "local_w_1_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_1_reload", "role": "default" }} , 
 	{ "name": "local_w_2_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_2_reload", "role": "default" }} , 
 	{ "name": "local_w_3_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_3_reload", "role": "default" }} , 
 	{ "name": "local_w_4_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_4_reload", "role": "default" }} , 
 	{ "name": "local_w_5_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_5_reload", "role": "default" }} , 
 	{ "name": "local_w_6_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_6_reload", "role": "default" }} , 
 	{ "name": "local_w_7_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_7_reload", "role": "default" }} , 
 	{ "name": "local_w_8_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_8_reload", "role": "default" }} , 
 	{ "name": "local_w_9_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_9_reload", "role": "default" }} , 
 	{ "name": "local_w_10_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_10_reload", "role": "default" }} , 
 	{ "name": "local_w_11_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_11_reload", "role": "default" }} , 
 	{ "name": "local_w_12_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_12_reload", "role": "default" }} , 
 	{ "name": "local_w_13_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_13_reload", "role": "default" }} , 
 	{ "name": "local_w_14_reload", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_14_reload", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"],
		"CDFG" : "Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "-1", "EstimateLatencyMax" : "-1",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "sizeP", "Type" : "None", "Direction" : "I"},
			{"Name" : "zext_ln66_1", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_in_x", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_in_x_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_in_x_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_1", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_2", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_3", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x_4", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_mod_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_mod_1_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_mod_2_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_mod_3_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_mod_4_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_1_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_2_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_3_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_4_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_5_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_6_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_7_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_8_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_9_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_10_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_11_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_12_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_13_reload", "Type" : "None", "Direction" : "I"},
			{"Name" : "local_w_14_reload", "Type" : "None", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Compute_Loop_VITIS_LOOP_67_4", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter339", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter339", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_64ns_64ns_128_2_1_U27", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_64ns_64ns_128_2_1_U28", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_64ns_64ns_128_2_1_U29", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sparsemux_11_3_64_1_1_U30", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sparsemux_11_3_64_1_1_U31", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_128ns_64ns_64_132_1_U32", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sparsemux_11_3_64_1_1_U33", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_128ns_64ns_64_132_1_U34", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_64ns_64ns_64_68_1_U35", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sparsemux_11_3_64_1_1_U36", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_128ns_64ns_64_132_1_U37", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_64ns_64ns_64_68_1_U38", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.urem_64ns_64ns_64_68_1_U39", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4 {
		sizeP {Type I LastRead 0 FirstWrite -1}
		zext_ln66_1 {Type I LastRead 0 FirstWrite -1}
		local_in_x {Type I LastRead 1 FirstWrite -1}
		local_in_x_1 {Type I LastRead 68 FirstWrite -1}
		local_in_x_2 {Type I LastRead 136 FirstWrite -1}
		local_out_x {Type O LastRead -1 FirstWrite 339}
		local_out_x_1 {Type O LastRead -1 FirstWrite 339}
		local_out_x_2 {Type O LastRead -1 FirstWrite 339}
		local_out_x_3 {Type O LastRead -1 FirstWrite 339}
		local_out_x_4 {Type O LastRead -1 FirstWrite 339}
		local_mod_reload {Type I LastRead 0 FirstWrite -1}
		local_mod_1_reload {Type I LastRead 0 FirstWrite -1}
		local_mod_2_reload {Type I LastRead 0 FirstWrite -1}
		local_mod_3_reload {Type I LastRead 0 FirstWrite -1}
		local_mod_4_reload {Type I LastRead 0 FirstWrite -1}
		local_w_reload {Type I LastRead 0 FirstWrite -1}
		local_w_1_reload {Type I LastRead 0 FirstWrite -1}
		local_w_2_reload {Type I LastRead 0 FirstWrite -1}
		local_w_3_reload {Type I LastRead 0 FirstWrite -1}
		local_w_4_reload {Type I LastRead 0 FirstWrite -1}
		local_w_5_reload {Type I LastRead 0 FirstWrite -1}
		local_w_6_reload {Type I LastRead 0 FirstWrite -1}
		local_w_7_reload {Type I LastRead 0 FirstWrite -1}
		local_w_8_reload {Type I LastRead 0 FirstWrite -1}
		local_w_9_reload {Type I LastRead 0 FirstWrite -1}
		local_w_10_reload {Type I LastRead 0 FirstWrite -1}
		local_w_11_reload {Type I LastRead 0 FirstWrite -1}
		local_w_12_reload {Type I LastRead 0 FirstWrite -1}
		local_w_13_reload {Type I LastRead 0 FirstWrite -1}
		local_w_14_reload {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "-1", "Max" : "-1"}
	, {"Name" : "Interval", "Min" : "-1", "Max" : "-1"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	sizeP { ap_none {  { sizeP in_data 0 32 } } }
	zext_ln66_1 { ap_none {  { zext_ln66_1 in_data 0 44 } } }
	local_in_x { ap_memory {  { local_in_x_address0 mem_address 1 12 }  { local_in_x_ce0 mem_ce 1 1 }  { local_in_x_q0 mem_dout 0 64 } } }
	local_in_x_1 { ap_memory {  { local_in_x_1_address0 mem_address 1 12 }  { local_in_x_1_ce0 mem_ce 1 1 }  { local_in_x_1_q0 mem_dout 0 64 } } }
	local_in_x_2 { ap_memory {  { local_in_x_2_address0 mem_address 1 12 }  { local_in_x_2_ce0 mem_ce 1 1 }  { local_in_x_2_q0 mem_dout 0 64 } } }
	local_out_x { ap_memory {  { local_out_x_address1 MemPortADDR2 1 12 }  { local_out_x_ce1 MemPortCE2 1 1 }  { local_out_x_we1 MemPortWE2 1 1 }  { local_out_x_d1 MemPortDIN2 1 64 } } }
	local_out_x_1 { ap_memory {  { local_out_x_1_address1 MemPortADDR2 1 12 }  { local_out_x_1_ce1 MemPortCE2 1 1 }  { local_out_x_1_we1 MemPortWE2 1 1 }  { local_out_x_1_d1 MemPortDIN2 1 64 } } }
	local_out_x_2 { ap_memory {  { local_out_x_2_address1 MemPortADDR2 1 12 }  { local_out_x_2_ce1 MemPortCE2 1 1 }  { local_out_x_2_we1 MemPortWE2 1 1 }  { local_out_x_2_d1 MemPortDIN2 1 64 } } }
	local_out_x_3 { ap_memory {  { local_out_x_3_address1 MemPortADDR2 1 12 }  { local_out_x_3_ce1 MemPortCE2 1 1 }  { local_out_x_3_we1 MemPortWE2 1 1 }  { local_out_x_3_d1 MemPortDIN2 1 64 } } }
	local_out_x_4 { ap_memory {  { local_out_x_4_address1 MemPortADDR2 1 12 }  { local_out_x_4_ce1 MemPortCE2 1 1 }  { local_out_x_4_we1 MemPortWE2 1 1 }  { local_out_x_4_d1 MemPortDIN2 1 64 } } }
	local_mod_reload { ap_none {  { local_mod_reload in_data 0 64 } } }
	local_mod_1_reload { ap_none {  { local_mod_1_reload in_data 0 64 } } }
	local_mod_2_reload { ap_none {  { local_mod_2_reload in_data 0 64 } } }
	local_mod_3_reload { ap_none {  { local_mod_3_reload in_data 0 64 } } }
	local_mod_4_reload { ap_none {  { local_mod_4_reload in_data 0 64 } } }
	local_w_reload { ap_none {  { local_w_reload in_data 0 64 } } }
	local_w_1_reload { ap_none {  { local_w_1_reload in_data 0 64 } } }
	local_w_2_reload { ap_none {  { local_w_2_reload in_data 0 64 } } }
	local_w_3_reload { ap_none {  { local_w_3_reload in_data 0 64 } } }
	local_w_4_reload { ap_none {  { local_w_4_reload in_data 0 64 } } }
	local_w_5_reload { ap_none {  { local_w_5_reload in_data 0 64 } } }
	local_w_6_reload { ap_none {  { local_w_6_reload in_data 0 64 } } }
	local_w_7_reload { ap_none {  { local_w_7_reload in_data 0 64 } } }
	local_w_8_reload { ap_none {  { local_w_8_reload in_data 0 64 } } }
	local_w_9_reload { ap_none {  { local_w_9_reload in_data 0 64 } } }
	local_w_10_reload { ap_none {  { local_w_10_reload in_data 0 64 } } }
	local_w_11_reload { ap_none {  { local_w_11_reload in_data 0 64 } } }
	local_w_12_reload { ap_none {  { local_w_12_reload in_data 0 64 } } }
	local_w_13_reload { ap_none {  { local_w_13_reload in_data 0 64 } } }
	local_w_14_reload { ap_none {  { local_w_14_reload in_data 0 64 } } }
}
