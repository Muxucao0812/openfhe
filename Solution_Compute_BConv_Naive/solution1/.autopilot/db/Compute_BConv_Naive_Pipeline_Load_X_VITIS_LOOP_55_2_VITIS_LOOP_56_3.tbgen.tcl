set moduleName Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3
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
set C_modelName {Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict in_x { MEM_WIDTH 64 MEM_SIZE 262144 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict local_in_x_2 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_in_x_1 { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
dict set ap_memory_interface_dict local_in_x { MEM_WIDTH 64 MEM_SIZE 32768 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
set C_modelArgList {
	{ in_x int 64 regular {array 32768 { 1 3 } 1 1 }  }
	{ local_in_x_2 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_in_x_1 int 64 regular {array 4096 { 3 0 } 0 1 }  }
	{ local_in_x int 64 regular {array 4096 { 3 0 } 0 1 }  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "local_in_x_2", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_in_x_1", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "local_in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 21
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ in_x_address0 sc_out sc_lv 15 signal 0 } 
	{ in_x_ce0 sc_out sc_logic 1 signal 0 } 
	{ in_x_q0 sc_in sc_lv 64 signal 0 } 
	{ local_in_x_2_address1 sc_out sc_lv 12 signal 1 } 
	{ local_in_x_2_ce1 sc_out sc_logic 1 signal 1 } 
	{ local_in_x_2_we1 sc_out sc_logic 1 signal 1 } 
	{ local_in_x_2_d1 sc_out sc_lv 64 signal 1 } 
	{ local_in_x_1_address1 sc_out sc_lv 12 signal 2 } 
	{ local_in_x_1_ce1 sc_out sc_logic 1 signal 2 } 
	{ local_in_x_1_we1 sc_out sc_logic 1 signal 2 } 
	{ local_in_x_1_d1 sc_out sc_lv 64 signal 2 } 
	{ local_in_x_address1 sc_out sc_lv 12 signal 3 } 
	{ local_in_x_ce1 sc_out sc_logic 1 signal 3 } 
	{ local_in_x_we1 sc_out sc_logic 1 signal 3 } 
	{ local_in_x_d1 sc_out sc_lv 64 signal 3 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "in_x_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":15, "type": "signal", "bundle":{"name": "in_x", "role": "address0" }} , 
 	{ "name": "in_x_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "in_x", "role": "ce0" }} , 
 	{ "name": "in_x_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "in_x", "role": "q0" }} , 
 	{ "name": "local_in_x_2_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "address1" }} , 
 	{ "name": "local_in_x_2_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "ce1" }} , 
 	{ "name": "local_in_x_2_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "we1" }} , 
 	{ "name": "local_in_x_2_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_2", "role": "d1" }} , 
 	{ "name": "local_in_x_1_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "address1" }} , 
 	{ "name": "local_in_x_1_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "ce1" }} , 
 	{ "name": "local_in_x_1_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "we1" }} , 
 	{ "name": "local_in_x_1_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x_1", "role": "d1" }} , 
 	{ "name": "local_in_x_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "local_in_x", "role": "address1" }} , 
 	{ "name": "local_in_x_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x", "role": "ce1" }} , 
 	{ "name": "local_in_x_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "local_in_x", "role": "we1" }} , 
 	{ "name": "local_in_x_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "local_in_x", "role": "d1" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "12293", "EstimateLatencyMax" : "12293",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "in_x", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_in_x_2", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_in_x_1", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_in_x", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3 {
		in_x {Type I LastRead 2 FirstWrite -1}
		local_in_x_2 {Type O LastRead -1 FirstWrite 4}
		local_in_x_1 {Type O LastRead -1 FirstWrite 4}
		local_in_x {Type O LastRead -1 FirstWrite 4}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "12293", "Max" : "12293"}
	, {"Name" : "Interval", "Min" : "12293", "Max" : "12293"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	in_x { ap_memory {  { in_x_address0 mem_address 1 15 }  { in_x_ce0 mem_ce 1 1 }  { in_x_q0 mem_dout 0 64 } } }
	local_in_x_2 { ap_memory {  { local_in_x_2_address1 MemPortADDR2 1 12 }  { local_in_x_2_ce1 MemPortCE2 1 1 }  { local_in_x_2_we1 MemPortWE2 1 1 }  { local_in_x_2_d1 MemPortDIN2 1 64 } } }
	local_in_x_1 { ap_memory {  { local_in_x_1_address1 MemPortADDR2 1 12 }  { local_in_x_1_ce1 MemPortCE2 1 1 }  { local_in_x_1_we1 MemPortWE2 1 1 }  { local_in_x_1_d1 MemPortDIN2 1 64 } } }
	local_in_x { ap_memory {  { local_in_x_address1 MemPortADDR2 1 12 }  { local_in_x_ce1 MemPortCE2 1 1 }  { local_in_x_we1 MemPortWE2 1 1 }  { local_in_x_d1 MemPortDIN2 1 64 } } }
}
