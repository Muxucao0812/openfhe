set moduleName Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1
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
set C_modelName {Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict in_w { MEM_WIDTH 64 MEM_SIZE 120 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ in_w int 64 regular {array 15 { 1 3 } 1 1 }  }
	{ local_w_14_out int 64 regular {pointer 1}  }
	{ local_w_13_out int 64 regular {pointer 1}  }
	{ local_w_12_out int 64 regular {pointer 1}  }
	{ local_w_11_out int 64 regular {pointer 1}  }
	{ local_w_10_out int 64 regular {pointer 1}  }
	{ local_w_9_out int 64 regular {pointer 1}  }
	{ local_w_8_out int 64 regular {pointer 1}  }
	{ local_w_7_out int 64 regular {pointer 1}  }
	{ local_w_6_out int 64 regular {pointer 1}  }
	{ local_w_5_out int 64 regular {pointer 1}  }
	{ local_w_4_out int 64 regular {pointer 1}  }
	{ local_w_3_out int 64 regular {pointer 1}  }
	{ local_w_2_out int 64 regular {pointer 1}  }
	{ local_w_1_out int 64 regular {pointer 1}  }
	{ local_w_out int 64 regular {pointer 1}  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "in_w", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_w_14_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_13_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_12_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_11_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_10_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_9_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_8_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_7_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_6_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_5_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_4_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_3_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_2_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_1_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_w_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 39
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ in_w_address0 sc_out sc_lv 4 signal 0 } 
	{ in_w_ce0 sc_out sc_logic 1 signal 0 } 
	{ in_w_q0 sc_in sc_lv 64 signal 0 } 
	{ local_w_14_out sc_out sc_lv 64 signal 1 } 
	{ local_w_14_out_ap_vld sc_out sc_logic 1 outvld 1 } 
	{ local_w_13_out sc_out sc_lv 64 signal 2 } 
	{ local_w_13_out_ap_vld sc_out sc_logic 1 outvld 2 } 
	{ local_w_12_out sc_out sc_lv 64 signal 3 } 
	{ local_w_12_out_ap_vld sc_out sc_logic 1 outvld 3 } 
	{ local_w_11_out sc_out sc_lv 64 signal 4 } 
	{ local_w_11_out_ap_vld sc_out sc_logic 1 outvld 4 } 
	{ local_w_10_out sc_out sc_lv 64 signal 5 } 
	{ local_w_10_out_ap_vld sc_out sc_logic 1 outvld 5 } 
	{ local_w_9_out sc_out sc_lv 64 signal 6 } 
	{ local_w_9_out_ap_vld sc_out sc_logic 1 outvld 6 } 
	{ local_w_8_out sc_out sc_lv 64 signal 7 } 
	{ local_w_8_out_ap_vld sc_out sc_logic 1 outvld 7 } 
	{ local_w_7_out sc_out sc_lv 64 signal 8 } 
	{ local_w_7_out_ap_vld sc_out sc_logic 1 outvld 8 } 
	{ local_w_6_out sc_out sc_lv 64 signal 9 } 
	{ local_w_6_out_ap_vld sc_out sc_logic 1 outvld 9 } 
	{ local_w_5_out sc_out sc_lv 64 signal 10 } 
	{ local_w_5_out_ap_vld sc_out sc_logic 1 outvld 10 } 
	{ local_w_4_out sc_out sc_lv 64 signal 11 } 
	{ local_w_4_out_ap_vld sc_out sc_logic 1 outvld 11 } 
	{ local_w_3_out sc_out sc_lv 64 signal 12 } 
	{ local_w_3_out_ap_vld sc_out sc_logic 1 outvld 12 } 
	{ local_w_2_out sc_out sc_lv 64 signal 13 } 
	{ local_w_2_out_ap_vld sc_out sc_logic 1 outvld 13 } 
	{ local_w_1_out sc_out sc_lv 64 signal 14 } 
	{ local_w_1_out_ap_vld sc_out sc_logic 1 outvld 14 } 
	{ local_w_out sc_out sc_lv 64 signal 15 } 
	{ local_w_out_ap_vld sc_out sc_logic 1 outvld 15 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "in_w_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "in_w", "role": "address0" }} , 
 	{ "name": "in_w_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "in_w", "role": "ce0" }} , 
 	{ "name": "in_w_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "in_w", "role": "q0" }} , 
 	{ "name": "local_w_14_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_14_out", "role": "default" }} , 
 	{ "name": "local_w_14_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_14_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_13_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_13_out", "role": "default" }} , 
 	{ "name": "local_w_13_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_13_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_12_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_12_out", "role": "default" }} , 
 	{ "name": "local_w_12_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_12_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_11_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_11_out", "role": "default" }} , 
 	{ "name": "local_w_11_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_11_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_10_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_10_out", "role": "default" }} , 
 	{ "name": "local_w_10_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_10_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_9_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_9_out", "role": "default" }} , 
 	{ "name": "local_w_9_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_9_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_8_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_8_out", "role": "default" }} , 
 	{ "name": "local_w_8_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_8_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_7_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_7_out", "role": "default" }} , 
 	{ "name": "local_w_7_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_7_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_6_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_6_out", "role": "default" }} , 
 	{ "name": "local_w_6_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_6_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_5_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_5_out", "role": "default" }} , 
 	{ "name": "local_w_5_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_5_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_4_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_4_out", "role": "default" }} , 
 	{ "name": "local_w_4_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_4_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_3_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_3_out", "role": "default" }} , 
 	{ "name": "local_w_3_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_3_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_2_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_2_out", "role": "default" }} , 
 	{ "name": "local_w_2_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_2_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_1_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_1_out", "role": "default" }} , 
 	{ "name": "local_w_1_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_1_out", "role": "ap_vld" }} , 
 	{ "name": "local_w_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_w_out", "role": "default" }} , 
 	{ "name": "local_w_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_w_out", "role": "ap_vld" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "17", "EstimateLatencyMax" : "17",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "in_w", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_w_14_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_13_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_12_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_11_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_10_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_9_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_8_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_7_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_6_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_5_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_4_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_3_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_2_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_1_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_w_out", "Type" : "Vld", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_W_VITIS_LOOP_40_1", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter1", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter1", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1 {
		in_w {Type I LastRead 0 FirstWrite -1}
		local_w_14_out {Type O LastRead -1 FirstWrite 0}
		local_w_13_out {Type O LastRead -1 FirstWrite 0}
		local_w_12_out {Type O LastRead -1 FirstWrite 0}
		local_w_11_out {Type O LastRead -1 FirstWrite 0}
		local_w_10_out {Type O LastRead -1 FirstWrite 0}
		local_w_9_out {Type O LastRead -1 FirstWrite 0}
		local_w_8_out {Type O LastRead -1 FirstWrite 0}
		local_w_7_out {Type O LastRead -1 FirstWrite 0}
		local_w_6_out {Type O LastRead -1 FirstWrite 0}
		local_w_5_out {Type O LastRead -1 FirstWrite 0}
		local_w_4_out {Type O LastRead -1 FirstWrite 0}
		local_w_3_out {Type O LastRead -1 FirstWrite 0}
		local_w_2_out {Type O LastRead -1 FirstWrite 0}
		local_w_1_out {Type O LastRead -1 FirstWrite 0}
		local_w_out {Type O LastRead -1 FirstWrite 0}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "17", "Max" : "17"}
	, {"Name" : "Interval", "Min" : "17", "Max" : "17"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	in_w { ap_memory {  { in_w_address0 mem_address 1 4 }  { in_w_ce0 mem_ce 1 1 }  { in_w_q0 mem_dout 0 64 } } }
	local_w_14_out { ap_vld {  { local_w_14_out out_data 1 64 }  { local_w_14_out_ap_vld out_vld 1 1 } } }
	local_w_13_out { ap_vld {  { local_w_13_out out_data 1 64 }  { local_w_13_out_ap_vld out_vld 1 1 } } }
	local_w_12_out { ap_vld {  { local_w_12_out out_data 1 64 }  { local_w_12_out_ap_vld out_vld 1 1 } } }
	local_w_11_out { ap_vld {  { local_w_11_out out_data 1 64 }  { local_w_11_out_ap_vld out_vld 1 1 } } }
	local_w_10_out { ap_vld {  { local_w_10_out out_data 1 64 }  { local_w_10_out_ap_vld out_vld 1 1 } } }
	local_w_9_out { ap_vld {  { local_w_9_out out_data 1 64 }  { local_w_9_out_ap_vld out_vld 1 1 } } }
	local_w_8_out { ap_vld {  { local_w_8_out out_data 1 64 }  { local_w_8_out_ap_vld out_vld 1 1 } } }
	local_w_7_out { ap_vld {  { local_w_7_out out_data 1 64 }  { local_w_7_out_ap_vld out_vld 1 1 } } }
	local_w_6_out { ap_vld {  { local_w_6_out out_data 1 64 }  { local_w_6_out_ap_vld out_vld 1 1 } } }
	local_w_5_out { ap_vld {  { local_w_5_out out_data 1 64 }  { local_w_5_out_ap_vld out_vld 1 1 } } }
	local_w_4_out { ap_vld {  { local_w_4_out out_data 1 64 }  { local_w_4_out_ap_vld out_vld 1 1 } } }
	local_w_3_out { ap_vld {  { local_w_3_out out_data 1 64 }  { local_w_3_out_ap_vld out_vld 1 1 } } }
	local_w_2_out { ap_vld {  { local_w_2_out out_data 1 64 }  { local_w_2_out_ap_vld out_vld 1 1 } } }
	local_w_1_out { ap_vld {  { local_w_1_out out_data 1 64 }  { local_w_1_out_ap_vld out_vld 1 1 } } }
	local_w_out { ap_vld {  { local_w_out out_data 1 64 }  { local_w_out_ap_vld out_vld 1 1 } } }
}
