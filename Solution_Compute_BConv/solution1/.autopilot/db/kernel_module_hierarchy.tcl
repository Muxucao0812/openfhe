set ModuleHierarchy {[{
"Name" : "Compute_BConv","ID" : "0","Type" : "sequential",
"SubInsts" : [
	{"Name" : "grp_Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1_fu_230","ID" : "1","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_W_VITIS_LOOP_214_1","ID" : "2","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Pipeline_Load_Mod_fu_251","ID" : "3","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_Mod","ID" : "4","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3_fu_276","ID" : "5","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3","ID" : "6","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Pipeline_Systolic_Loop_fu_288","ID" : "7","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Systolic_Loop","ID" : "8","Type" : "pipeline",
		"SubInsts" : [
		{"Name" : "grp_MultMod_fu_858","ID" : "9","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_867","ID" : "10","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_876","ID" : "11","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_885","ID" : "12","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_894","ID" : "13","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_903","ID" : "14","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_912","ID" : "15","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_921","ID" : "16","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_930","ID" : "17","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_939","ID" : "18","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_948","ID" : "19","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_957","ID" : "20","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_966","ID" : "21","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_975","ID" : "22","Type" : "pipeline"},
		{"Name" : "grp_MultMod_fu_984","ID" : "23","Type" : "pipeline"},]},]},
	{"Name" : "grp_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5_fu_335","ID" : "24","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5","ID" : "25","Type" : "pipeline"},]},]
}]}