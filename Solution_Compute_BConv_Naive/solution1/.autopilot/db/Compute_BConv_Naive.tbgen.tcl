set moduleName Compute_BConv_Naive
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set C_modelName {Compute_BConv_Naive}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict in_x { MEM_WIDTH 64 MEM_SIZE 262144 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict in_w { MEM_WIDTH 64 MEM_SIZE 120 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict out_mod { MEM_WIDTH 64 MEM_SIZE 40 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict out_k_half { MEM_WIDTH 64 MEM_SIZE 40 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict out_m_barrett { MEM_WIDTH 64 MEM_SIZE 40 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ in_x int 64 regular {array 32768 { 2 3 } 1 1 }  }
	{ in_w int 64 regular {array 15 { 1 3 } 1 1 }  }
	{ out_mod int 64 regular {array 5 { 1 3 } 1 1 }  }
	{ out_k_half int 64 unused {array 5 { } 0 1 }  }
	{ out_m_barrett int 64 unused {array 5 { } 0 1 }  }
	{ sizeP int 32 regular  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "in_w", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "out_mod", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "out_k_half", "interface" : "memory", "bitwidth" : 64, "direction" : "NONE"} , 
 	{ "Name" : "out_m_barrett", "interface" : "memory", "bitwidth" : 64, "direction" : "NONE"} , 
 	{ "Name" : "sizeP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 38
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ in_x_address0 sc_out sc_lv 15 signal 0 } 
	{ in_x_ce0 sc_out sc_logic 1 signal 0 } 
	{ in_x_we0 sc_out sc_logic 1 signal 0 } 
	{ in_x_d0 sc_out sc_lv 64 signal 0 } 
	{ in_x_q0 sc_in sc_lv 64 signal 0 } 
	{ in_w_address0 sc_out sc_lv 4 signal 1 } 
	{ in_w_ce0 sc_out sc_logic 1 signal 1 } 
	{ in_w_q0 sc_in sc_lv 64 signal 1 } 
	{ out_mod_address0 sc_out sc_lv 3 signal 2 } 
	{ out_mod_ce0 sc_out sc_logic 1 signal 2 } 
	{ out_mod_q0 sc_in sc_lv 64 signal 2 } 
	{ out_k_half_address0 sc_out sc_lv 3 signal 3 } 
	{ out_k_half_ce0 sc_out sc_logic 1 signal 3 } 
	{ out_k_half_we0 sc_out sc_logic 1 signal 3 } 
	{ out_k_half_d0 sc_out sc_lv 64 signal 3 } 
	{ out_k_half_q0 sc_in sc_lv 64 signal 3 } 
	{ out_k_half_address1 sc_out sc_lv 3 signal 3 } 
	{ out_k_half_ce1 sc_out sc_logic 1 signal 3 } 
	{ out_k_half_we1 sc_out sc_logic 1 signal 3 } 
	{ out_k_half_d1 sc_out sc_lv 64 signal 3 } 
	{ out_k_half_q1 sc_in sc_lv 64 signal 3 } 
	{ out_m_barrett_address0 sc_out sc_lv 3 signal 4 } 
	{ out_m_barrett_ce0 sc_out sc_logic 1 signal 4 } 
	{ out_m_barrett_we0 sc_out sc_logic 1 signal 4 } 
	{ out_m_barrett_d0 sc_out sc_lv 64 signal 4 } 
	{ out_m_barrett_q0 sc_in sc_lv 64 signal 4 } 
	{ out_m_barrett_address1 sc_out sc_lv 3 signal 4 } 
	{ out_m_barrett_ce1 sc_out sc_logic 1 signal 4 } 
	{ out_m_barrett_we1 sc_out sc_logic 1 signal 4 } 
	{ out_m_barrett_d1 sc_out sc_lv 64 signal 4 } 
	{ out_m_barrett_q1 sc_in sc_lv 64 signal 4 } 
	{ sizeP sc_in sc_lv 32 signal 5 } 
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
 	{ "name": "in_x_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "in_x", "role": "we0" }} , 
 	{ "name": "in_x_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "in_x", "role": "d0" }} , 
 	{ "name": "in_x_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "in_x", "role": "q0" }} , 
 	{ "name": "in_w_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "in_w", "role": "address0" }} , 
 	{ "name": "in_w_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "in_w", "role": "ce0" }} , 
 	{ "name": "in_w_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "in_w", "role": "q0" }} , 
 	{ "name": "out_mod_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_mod", "role": "address0" }} , 
 	{ "name": "out_mod_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_mod", "role": "ce0" }} , 
 	{ "name": "out_mod_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_mod", "role": "q0" }} , 
 	{ "name": "out_k_half_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_k_half", "role": "address0" }} , 
 	{ "name": "out_k_half_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_k_half", "role": "ce0" }} , 
 	{ "name": "out_k_half_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_k_half", "role": "we0" }} , 
 	{ "name": "out_k_half_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_k_half", "role": "d0" }} , 
 	{ "name": "out_k_half_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_k_half", "role": "q0" }} , 
 	{ "name": "out_k_half_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_k_half", "role": "address1" }} , 
 	{ "name": "out_k_half_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_k_half", "role": "ce1" }} , 
 	{ "name": "out_k_half_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_k_half", "role": "we1" }} , 
 	{ "name": "out_k_half_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_k_half", "role": "d1" }} , 
 	{ "name": "out_k_half_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_k_half", "role": "q1" }} , 
 	{ "name": "out_m_barrett_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "address0" }} , 
 	{ "name": "out_m_barrett_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "ce0" }} , 
 	{ "name": "out_m_barrett_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "we0" }} , 
 	{ "name": "out_m_barrett_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "d0" }} , 
 	{ "name": "out_m_barrett_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "q0" }} , 
 	{ "name": "out_m_barrett_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "address1" }} , 
 	{ "name": "out_m_barrett_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "ce1" }} , 
 	{ "name": "out_m_barrett_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "we1" }} , 
 	{ "name": "out_m_barrett_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "d1" }} , 
 	{ "name": "out_m_barrett_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "q1" }} , 
 	{ "name": "sizeP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sizeP", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "11", "13", "15", "30"],
		"CDFG" : "Compute_BConv_Naive",
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
			{"Name" : "in_x", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "13", "SubInstance" : "grp_Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3_fu_206", "Port" : "in_x", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "30", "SubInstance" : "grp_Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7_fu_253", "Port" : "in_x", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "in_w", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "9", "SubInstance" : "grp_Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1_fu_174", "Port" : "in_w", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_mod", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "11", "SubInstance" : "grp_Compute_BConv_Naive_Pipeline_Load_Mod_fu_195", "Port" : "out_mod", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_k_half", "Type" : "Memory", "Direction" : "X"},
			{"Name" : "out_m_barrett", "Type" : "Memory", "Direction" : "X"},
			{"Name" : "sizeP", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_1_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_2_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_1_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_2_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_3_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_4_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1_fu_174", "Parent" : "0", "Child" : ["10"],
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
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1_fu_174.flow_control_loop_pipe_sequential_init_U", "Parent" : "9"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_Mod_fu_195", "Parent" : "0", "Child" : ["12"],
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
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_Mod_fu_195.flow_control_loop_pipe_sequential_init_U", "Parent" : "11"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3_fu_206", "Parent" : "0", "Child" : ["14"],
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
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3_fu_206.flow_control_loop_pipe_sequential_init_U", "Parent" : "13"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218", "Parent" : "0", "Child" : ["16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29"],
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
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.mul_64ns_64ns_128_2_1_U27", "Parent" : "15"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.mul_64ns_64ns_128_2_1_U28", "Parent" : "15"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.mul_64ns_64ns_128_2_1_U29", "Parent" : "15"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.sparsemux_11_3_64_1_1_U30", "Parent" : "15"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.sparsemux_11_3_64_1_1_U31", "Parent" : "15"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_128ns_64ns_64_132_1_U32", "Parent" : "15"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.sparsemux_11_3_64_1_1_U33", "Parent" : "15"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_128ns_64ns_64_132_1_U34", "Parent" : "15"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_64ns_64ns_64_68_1_U35", "Parent" : "15"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.sparsemux_11_3_64_1_1_U36", "Parent" : "15"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_128ns_64ns_64_132_1_U37", "Parent" : "15"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_64ns_64ns_64_68_1_U38", "Parent" : "15"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.urem_64ns_64ns_64_68_1_U39", "Parent" : "15"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218.flow_control_loop_pipe_sequential_init_U", "Parent" : "15"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7_fu_253", "Parent" : "0", "Child" : ["31", "32"],
		"CDFG" : "Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7",
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
			{"Name" : "zext_ln66_1", "Type" : "None", "Direction" : "I"},
			{"Name" : "in_x", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_4", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7_fu_253.sparsemux_11_3_64_1_1_U74", "Parent" : "30"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7_fu_253.flow_control_loop_pipe_sequential_init_U", "Parent" : "30"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv_Naive {
		in_x {Type IO LastRead 2 FirstWrite -1}
		in_w {Type I LastRead 0 FirstWrite -1}
		out_mod {Type I LastRead 0 FirstWrite -1}
		out_k_half {Type X LastRead -1 FirstWrite -1}
		out_m_barrett {Type X LastRead -1 FirstWrite -1}
		sizeP {Type I LastRead 2 FirstWrite -1}}
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
		local_w_out {Type O LastRead -1 FirstWrite 0}}
	Compute_BConv_Naive_Pipeline_Load_Mod {
		out_mod {Type I LastRead 0 FirstWrite -1}
		local_mod_4_out {Type O LastRead -1 FirstWrite 0}
		local_mod_3_out {Type O LastRead -1 FirstWrite 0}
		local_mod_2_out {Type O LastRead -1 FirstWrite 0}
		local_mod_1_out {Type O LastRead -1 FirstWrite 0}
		local_mod_out {Type O LastRead -1 FirstWrite 0}}
	Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3 {
		in_x {Type I LastRead 2 FirstWrite -1}
		local_in_x_2 {Type O LastRead -1 FirstWrite 4}
		local_in_x_1 {Type O LastRead -1 FirstWrite 4}
		local_in_x {Type O LastRead -1 FirstWrite 4}}
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
		local_w_14_reload {Type I LastRead 0 FirstWrite -1}}
	Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7 {
		zext_ln66_1 {Type I LastRead 0 FirstWrite -1}
		in_x {Type O LastRead -1 FirstWrite 4}
		local_out_x {Type I LastRead 2 FirstWrite -1}
		local_out_x_1 {Type I LastRead 2 FirstWrite -1}
		local_out_x_2 {Type I LastRead 2 FirstWrite -1}
		local_out_x_3 {Type I LastRead 2 FirstWrite -1}
		local_out_x_4 {Type I LastRead 2 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "-1", "Max" : "-1"}
	, {"Name" : "Interval", "Min" : "0", "Max" : "0"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	in_x { ap_memory {  { in_x_address0 mem_address 1 15 }  { in_x_ce0 mem_ce 1 1 }  { in_x_we0 mem_we 1 1 }  { in_x_d0 mem_din 1 64 }  { in_x_q0 mem_dout 0 64 } } }
	in_w { ap_memory {  { in_w_address0 mem_address 1 4 }  { in_w_ce0 mem_ce 1 1 }  { in_w_q0 mem_dout 0 64 } } }
	out_mod { ap_memory {  { out_mod_address0 mem_address 1 3 }  { out_mod_ce0 mem_ce 1 1 }  { out_mod_q0 mem_dout 0 64 } } }
	out_k_half { ap_memory {  { out_k_half_address0 mem_address 1 3 }  { out_k_half_ce0 mem_ce 1 1 }  { out_k_half_we0 mem_we 1 1 }  { out_k_half_d0 mem_din 1 64 }  { out_k_half_q0 mem_dout 0 64 }  { out_k_half_address1 MemPortADDR2 1 3 }  { out_k_half_ce1 MemPortCE2 1 1 }  { out_k_half_we1 MemPortWE2 1 1 }  { out_k_half_d1 MemPortDIN2 1 64 }  { out_k_half_q1 MemPortDOUT2 0 64 } } }
	out_m_barrett { ap_memory {  { out_m_barrett_address0 mem_address 1 3 }  { out_m_barrett_ce0 mem_ce 1 1 }  { out_m_barrett_we0 mem_we 1 1 }  { out_m_barrett_d0 mem_din 1 64 }  { out_m_barrett_q0 mem_dout 0 64 }  { out_m_barrett_address1 MemPortADDR2 1 3 }  { out_m_barrett_ce1 MemPortCE2 1 1 }  { out_m_barrett_we1 MemPortWE2 1 1 }  { out_m_barrett_d1 MemPortDIN2 1 64 }  { out_m_barrett_q1 MemPortDOUT2 0 64 } } }
	sizeP { ap_none {  { sizeP in_data 0 32 } } }
}

set maxi_interface_dict [dict create]

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
