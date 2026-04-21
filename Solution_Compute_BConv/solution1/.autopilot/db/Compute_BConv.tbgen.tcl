set moduleName Compute_BConv
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
set C_modelName {Compute_BConv}
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
	{ out_k_half int 64 regular {array 5 { 1 3 } 1 1 }  }
	{ out_m_barrett int 64 regular {array 5 { 1 3 } 1 1 }  }
	{ sizeP int 32 regular  }
}
set hasAXIMCache 0
set hasAXIML2Cache 0
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "in_x", "interface" : "memory", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "in_w", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "out_mod", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "out_k_half", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "out_m_barrett", "interface" : "memory", "bitwidth" : 64, "direction" : "READONLY"} , 
 	{ "Name" : "sizeP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 24
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
	{ out_k_half_q0 sc_in sc_lv 64 signal 3 } 
	{ out_m_barrett_address0 sc_out sc_lv 3 signal 4 } 
	{ out_m_barrett_ce0 sc_out sc_logic 1 signal 4 } 
	{ out_m_barrett_q0 sc_in sc_lv 64 signal 4 } 
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
 	{ "name": "out_k_half_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_k_half", "role": "q0" }} , 
 	{ "name": "out_m_barrett_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "address0" }} , 
 	{ "name": "out_m_barrett_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "ce0" }} , 
 	{ "name": "out_m_barrett_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "out_m_barrett", "role": "q0" }} , 
 	{ "name": "sizeP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sizeP", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "11", "13", "15", "77"],
		"CDFG" : "Compute_BConv",
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
					{"ID" : "13", "SubInstance" : "grp_Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3_fu_276", "Port" : "in_x", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "77", "SubInstance" : "grp_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5_fu_335", "Port" : "in_x", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "in_w", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "9", "SubInstance" : "grp_Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1_fu_230", "Port" : "in_w", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_mod", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "11", "SubInstance" : "grp_Compute_BConv_Pipeline_Load_Mod_fu_251", "Port" : "out_mod", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_k_half", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "11", "SubInstance" : "grp_Compute_BConv_Pipeline_Load_Mod_fu_251", "Port" : "out_k_half", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_m_barrett", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "11", "SubInstance" : "grp_Compute_BConv_Pipeline_Load_Mod_fu_251", "Port" : "out_m_barrett", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "sizeP", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_3_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_in_x_4_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_5_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_6_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_7_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.local_out_x_8_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1_fu_230", "Parent" : "0", "Child" : ["10"],
		"CDFG" : "Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1",
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
			{"Name" : "p_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out1", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out2", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out3", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out4", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out5", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out6", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out7", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out8", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out9", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out10", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out11", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out12", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out13", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out14", "Type" : "Vld", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_W_VITIS_LOOP_214_1", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter1", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter1", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1_fu_230.flow_control_loop_pipe_sequential_init_U", "Parent" : "9"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_Mod_fu_251", "Parent" : "0", "Child" : ["12"],
		"CDFG" : "Compute_BConv_Pipeline_Load_Mod",
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
			{"Name" : "out_k_half", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "out_m_barrett", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "p_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out1", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out2", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out3", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out4", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out5", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out6", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out7", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out8", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out9", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "mod_p_4_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "p_out10", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "mod_p_2_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "mod_p_1_out", "Type" : "Vld", "Direction" : "O"},
			{"Name" : "mod_p_out", "Type" : "Vld", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_Mod", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter1", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter1", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_Mod_fu_251.flow_control_loop_pipe_sequential_init_U", "Parent" : "11"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3_fu_276", "Parent" : "0", "Child" : ["14"],
		"CDFG" : "Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3",
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
			{"Name" : "local_in_x_4", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_in_x_3", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_in_x", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3_fu_276.flow_control_loop_pipe_sequential_init_U", "Parent" : "13"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288", "Parent" : "0", "Child" : ["16", "20", "24", "28", "32", "36", "40", "44", "48", "52", "56", "60", "64", "68", "72", "76"],
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
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_858", "Parent" : "15", "Child" : ["17", "18", "19"],
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
	{"ID" : "17", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_858.mul_64ns_64ns_128_5_1_U39", "Parent" : "16"},
	{"ID" : "18", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_858.mul_64ns_64ns_128_5_1_U40", "Parent" : "16"},
	{"ID" : "19", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_858.mul_64s_64s_64_5_1_U41", "Parent" : "16"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_867", "Parent" : "15", "Child" : ["21", "22", "23"],
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
	{"ID" : "21", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_867.mul_64ns_64ns_128_5_1_U39", "Parent" : "20"},
	{"ID" : "22", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_867.mul_64ns_64ns_128_5_1_U40", "Parent" : "20"},
	{"ID" : "23", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_867.mul_64s_64s_64_5_1_U41", "Parent" : "20"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_876", "Parent" : "15", "Child" : ["25", "26", "27"],
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
	{"ID" : "25", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_876.mul_64ns_64ns_128_5_1_U39", "Parent" : "24"},
	{"ID" : "26", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_876.mul_64ns_64ns_128_5_1_U40", "Parent" : "24"},
	{"ID" : "27", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_876.mul_64s_64s_64_5_1_U41", "Parent" : "24"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_885", "Parent" : "15", "Child" : ["29", "30", "31"],
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
	{"ID" : "29", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_885.mul_64ns_64ns_128_5_1_U39", "Parent" : "28"},
	{"ID" : "30", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_885.mul_64ns_64ns_128_5_1_U40", "Parent" : "28"},
	{"ID" : "31", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_885.mul_64s_64s_64_5_1_U41", "Parent" : "28"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_894", "Parent" : "15", "Child" : ["33", "34", "35"],
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
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_894.mul_64ns_64ns_128_5_1_U39", "Parent" : "32"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_894.mul_64ns_64ns_128_5_1_U40", "Parent" : "32"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_894.mul_64s_64s_64_5_1_U41", "Parent" : "32"},
	{"ID" : "36", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_903", "Parent" : "15", "Child" : ["37", "38", "39"],
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
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_903.mul_64ns_64ns_128_5_1_U39", "Parent" : "36"},
	{"ID" : "38", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_903.mul_64ns_64ns_128_5_1_U40", "Parent" : "36"},
	{"ID" : "39", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_903.mul_64s_64s_64_5_1_U41", "Parent" : "36"},
	{"ID" : "40", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_912", "Parent" : "15", "Child" : ["41", "42", "43"],
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
	{"ID" : "41", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_912.mul_64ns_64ns_128_5_1_U39", "Parent" : "40"},
	{"ID" : "42", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_912.mul_64ns_64ns_128_5_1_U40", "Parent" : "40"},
	{"ID" : "43", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_912.mul_64s_64s_64_5_1_U41", "Parent" : "40"},
	{"ID" : "44", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_921", "Parent" : "15", "Child" : ["45", "46", "47"],
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
	{"ID" : "45", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_921.mul_64ns_64ns_128_5_1_U39", "Parent" : "44"},
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_921.mul_64ns_64ns_128_5_1_U40", "Parent" : "44"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_921.mul_64s_64s_64_5_1_U41", "Parent" : "44"},
	{"ID" : "48", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_930", "Parent" : "15", "Child" : ["49", "50", "51"],
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
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_930.mul_64ns_64ns_128_5_1_U39", "Parent" : "48"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_930.mul_64ns_64ns_128_5_1_U40", "Parent" : "48"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_930.mul_64s_64s_64_5_1_U41", "Parent" : "48"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_939", "Parent" : "15", "Child" : ["53", "54", "55"],
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
	{"ID" : "53", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_939.mul_64ns_64ns_128_5_1_U39", "Parent" : "52"},
	{"ID" : "54", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_939.mul_64ns_64ns_128_5_1_U40", "Parent" : "52"},
	{"ID" : "55", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_939.mul_64s_64s_64_5_1_U41", "Parent" : "52"},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_948", "Parent" : "15", "Child" : ["57", "58", "59"],
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
	{"ID" : "57", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_948.mul_64ns_64ns_128_5_1_U39", "Parent" : "56"},
	{"ID" : "58", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_948.mul_64ns_64ns_128_5_1_U40", "Parent" : "56"},
	{"ID" : "59", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_948.mul_64s_64s_64_5_1_U41", "Parent" : "56"},
	{"ID" : "60", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_957", "Parent" : "15", "Child" : ["61", "62", "63"],
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
	{"ID" : "61", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_957.mul_64ns_64ns_128_5_1_U39", "Parent" : "60"},
	{"ID" : "62", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_957.mul_64ns_64ns_128_5_1_U40", "Parent" : "60"},
	{"ID" : "63", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_957.mul_64s_64s_64_5_1_U41", "Parent" : "60"},
	{"ID" : "64", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_966", "Parent" : "15", "Child" : ["65", "66", "67"],
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
	{"ID" : "65", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_966.mul_64ns_64ns_128_5_1_U39", "Parent" : "64"},
	{"ID" : "66", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_966.mul_64ns_64ns_128_5_1_U40", "Parent" : "64"},
	{"ID" : "67", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_966.mul_64s_64s_64_5_1_U41", "Parent" : "64"},
	{"ID" : "68", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_975", "Parent" : "15", "Child" : ["69", "70", "71"],
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
	{"ID" : "69", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_975.mul_64ns_64ns_128_5_1_U39", "Parent" : "68"},
	{"ID" : "70", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_975.mul_64ns_64ns_128_5_1_U40", "Parent" : "68"},
	{"ID" : "71", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_975.mul_64s_64s_64_5_1_U41", "Parent" : "68"},
	{"ID" : "72", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_984", "Parent" : "15", "Child" : ["73", "74", "75"],
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
	{"ID" : "73", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_984.mul_64ns_64ns_128_5_1_U39", "Parent" : "72"},
	{"ID" : "74", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_984.mul_64ns_64ns_128_5_1_U40", "Parent" : "72"},
	{"ID" : "75", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.grp_MultMod_fu_984.mul_64s_64s_64_5_1_U41", "Parent" : "72"},
	{"ID" : "76", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288.flow_control_loop_pipe_sequential_init_U", "Parent" : "15"},
	{"ID" : "77", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5_fu_335", "Parent" : "0", "Child" : ["78", "79"],
		"CDFG" : "Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5",
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
			{"Name" : "zext_ln243", "Type" : "None", "Direction" : "I"},
			{"Name" : "in_x", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "local_out_x", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_5", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_6", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_7", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "local_out_x_8", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "78", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5_fu_335.sparsemux_11_3_64_1_1_U92", "Parent" : "77"},
	{"ID" : "79", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5_fu_335.flow_control_loop_pipe_sequential_init_U", "Parent" : "77"}]}


set ArgLastReadFirstWriteLatency {
	Compute_BConv {
		in_x {Type IO LastRead 2 FirstWrite -1}
		in_w {Type I LastRead 0 FirstWrite -1}
		out_mod {Type I LastRead 0 FirstWrite -1}
		out_k_half {Type I LastRead 0 FirstWrite -1}
		out_m_barrett {Type I LastRead 0 FirstWrite -1}
		sizeP {Type I LastRead 2 FirstWrite -1}}
	Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1 {
		in_w {Type I LastRead 0 FirstWrite -1}
		p_out {Type O LastRead -1 FirstWrite 0}
		p_out1 {Type O LastRead -1 FirstWrite 0}
		p_out2 {Type O LastRead -1 FirstWrite 0}
		p_out3 {Type O LastRead -1 FirstWrite 0}
		p_out4 {Type O LastRead -1 FirstWrite 0}
		p_out5 {Type O LastRead -1 FirstWrite 0}
		p_out6 {Type O LastRead -1 FirstWrite 0}
		p_out7 {Type O LastRead -1 FirstWrite 0}
		p_out8 {Type O LastRead -1 FirstWrite 0}
		p_out9 {Type O LastRead -1 FirstWrite 0}
		p_out10 {Type O LastRead -1 FirstWrite 0}
		p_out11 {Type O LastRead -1 FirstWrite 0}
		p_out12 {Type O LastRead -1 FirstWrite 0}
		p_out13 {Type O LastRead -1 FirstWrite 0}
		p_out14 {Type O LastRead -1 FirstWrite 0}}
	Compute_BConv_Pipeline_Load_Mod {
		out_mod {Type I LastRead 0 FirstWrite -1}
		out_k_half {Type I LastRead 0 FirstWrite -1}
		out_m_barrett {Type I LastRead 0 FirstWrite -1}
		p_out {Type O LastRead -1 FirstWrite 0}
		p_out1 {Type O LastRead -1 FirstWrite 0}
		p_out2 {Type O LastRead -1 FirstWrite 0}
		p_out3 {Type O LastRead -1 FirstWrite 0}
		p_out4 {Type O LastRead -1 FirstWrite 0}
		p_out5 {Type O LastRead -1 FirstWrite 0}
		p_out6 {Type O LastRead -1 FirstWrite 0}
		p_out7 {Type O LastRead -1 FirstWrite 0}
		p_out8 {Type O LastRead -1 FirstWrite 0}
		p_out9 {Type O LastRead -1 FirstWrite 0}
		mod_p_4_out {Type O LastRead -1 FirstWrite 0}
		p_out10 {Type O LastRead -1 FirstWrite 0}
		mod_p_2_out {Type O LastRead -1 FirstWrite 0}
		mod_p_1_out {Type O LastRead -1 FirstWrite 0}
		mod_p_out {Type O LastRead -1 FirstWrite 0}}
	Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3 {
		in_x {Type I LastRead 2 FirstWrite -1}
		local_in_x_4 {Type O LastRead -1 FirstWrite 4}
		local_in_x_3 {Type O LastRead -1 FirstWrite 4}
		local_in_x {Type O LastRead -1 FirstWrite 4}}
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
		k_half_val {Type I LastRead 0 FirstWrite -1}}
	Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5 {
		zext_ln243 {Type I LastRead 0 FirstWrite -1}
		in_x {Type O LastRead -1 FirstWrite 4}
		local_out_x {Type I LastRead 2 FirstWrite -1}
		local_out_x_5 {Type I LastRead 2 FirstWrite -1}
		local_out_x_6 {Type I LastRead 2 FirstWrite -1}
		local_out_x_7 {Type I LastRead 2 FirstWrite -1}
		local_out_x_8 {Type I LastRead 2 FirstWrite -1}}}

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
	out_k_half { ap_memory {  { out_k_half_address0 mem_address 1 3 }  { out_k_half_ce0 mem_ce 1 1 }  { out_k_half_q0 mem_dout 0 64 } } }
	out_m_barrett { ap_memory {  { out_m_barrett_address0 mem_address 1 3 }  { out_m_barrett_ce0 mem_ce 1 1 }  { out_m_barrett_q0 mem_dout 0 64 } } }
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
