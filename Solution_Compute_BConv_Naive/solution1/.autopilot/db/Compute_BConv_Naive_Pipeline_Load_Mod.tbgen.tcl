set moduleName Compute_BConv_Naive_Pipeline_Load_Mod
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
set C_modelName {Compute_BConv_Naive_Pipeline_Load_Mod}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict out_mod { MEM_WIDTH 64 MEM_SIZE 40 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ out_mod int 64 regular {array 5 { 1 3 } 1 1 }  }
	{ local_mod_4_out int 64 regular {pointer 1}  }
	{ local_mod_3_out int 64 regular {pointer 1}  }
	{ local_mod_2_out int 64 regular {pointer 1}  }
	{ local_mod_1_out int 64 regular {pointer 1}  }
	{ local_mod_out int 64 regular {pointer 1}  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "out_mod", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_mod_4_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_mod_3_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_mod_2_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_mod_1_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_mod_out", "interface" : "wire", "bitwidth" : 64, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 19
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ out_mod_address0 sc_out sc_lv 3 signal 0 } 
	{ out_mod_ce0 sc_out sc_logic 1 signal 0 } 
	{ out_mod_q0 sc_in sc_lv 64 signal 0 } 
	{ local_mod_4_out sc_out sc_lv 64 signal 1 } 
	{ local_mod_4_out_ap_vld sc_out sc_logic 1 outvld 1 } 
	{ local_mod_3_out sc_out sc_lv 64 signal 2 } 
	{ local_mod_3_out_ap_vld sc_out sc_logic 1 outvld 2 } 
	{ local_mod_2_out sc_out sc_lv 64 signal 3 } 
	{ local_mod_2_out_ap_vld sc_out sc_logic 1 outvld 3 } 
	{ local_mod_1_out sc_out sc_lv 64 signal 4 } 
	{ local_mod_1_out_ap_vld sc_out sc_logic 1 outvld 4 } 
	{ local_mod_out sc_out sc_lv 64 signal 5 } 
	{ local_mod_out_ap_vld sc_out sc_logic 1 outvld 5 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "out_mod_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_mod", "role": "address0" }} , 
 	{ "name": "out_mod_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_mod", "role": "ce0" }} , 
 	{ "name": "out_mod_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_mod", "role": "q0" }} , 
 	{ "name": "local_mod_4_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_4_out", "role": "default" }} , 
 	{ "name": "local_mod_4_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_mod_4_out", "role": "ap_vld" }} , 
 	{ "name": "local_mod_3_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_3_out", "role": "default" }} , 
 	{ "name": "local_mod_3_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_mod_3_out", "role": "ap_vld" }} , 
 	{ "name": "local_mod_2_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_2_out", "role": "default" }} , 
 	{ "name": "local_mod_2_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_mod_2_out", "role": "ap_vld" }} , 
 	{ "name": "local_mod_1_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_1_out", "role": "default" }} , 
 	{ "name": "local_mod_1_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_mod_1_out", "role": "ap_vld" }} , 
 	{ "name": "local_mod_out", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_mod_out", "role": "default" }} , 
 	{ "name": "local_mod_out_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "local_mod_out", "role": "ap_vld" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "Compute_BConv_Naive_Pipeline_Load_Mod",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "7", "EstimateLatencyMax" : "7",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "out_mod", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_mod_4_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_mod_3_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_mod_2_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_mod_1_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "local_mod_out", "Type" : "Vld", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_Mod", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter1", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter1", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Naive_Pipeline_Load_Mod {
		out_mod {Type I LastRead 0 FirstWrite -1}
		local_mod_4_out {Type O LastRead -1 FirstWrite 0}
		local_mod_3_out {Type O LastRead -1 FirstWrite 0}
		local_mod_2_out {Type O LastRead -1 FirstWrite 0}
		local_mod_1_out {Type O LastRead -1 FirstWrite 0}
		local_mod_out {Type O LastRead -1 FirstWrite 0}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "7", "Max" : "7"}
	, {"Name" : "Interval", "Min" : "7", "Max" : "7"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	out_mod { ap_memory {  { out_mod_address0 mem_address 1 3 }  { out_mod_ce0 mem_ce 1 1 }  { out_mod_q0 mem_dout 0 64 } } }
	local_mod_4_out { ap_vld {  { local_mod_4_out out_data 1 64 }  { local_mod_4_out_ap_vld out_vld 1 1 } } }
	local_mod_3_out { ap_vld {  { local_mod_3_out out_data 1 64 }  { local_mod_3_out_ap_vld out_vld 1 1 } } }
	local_mod_2_out { ap_vld {  { local_mod_2_out out_data 1 64 }  { local_mod_2_out_ap_vld out_vld 1 1 } } }
	local_mod_1_out { ap_vld {  { local_mod_1_out out_data 1 64 }  { local_mod_1_out_ap_vld out_vld 1 1 } } }
	local_mod_out { ap_vld {  { local_mod_out out_data 1 64 }  { local_mod_out_ap_vld out_vld 1 1 } } }
}
