set ModuleHierarchy {[{
"Name" : "Compute_BConv_Naive","ID" : "0","Type" : "sequential",
"SubInsts" : [
	{"Name" : "grp_Compute_BConv_Naive_Pipeline_Load_W_VITIS_LOOP_40_1_fu_174","ID" : "1","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_W_VITIS_LOOP_40_1","ID" : "2","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Naive_Pipeline_Load_Mod_fu_195","ID" : "3","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_Mod","ID" : "4","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Naive_Pipeline_Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3_fu_206","ID" : "5","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Load_X_VITIS_LOOP_55_2_VITIS_LOOP_56_3","ID" : "6","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Naive_Pipeline_Compute_Loop_VITIS_LOOP_67_4_fu_218","ID" : "7","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Compute_Loop_VITIS_LOOP_67_4","ID" : "8","Type" : "pipeline"},]},
	{"Name" : "grp_Compute_BConv_Naive_Pipeline_Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7_fu_253","ID" : "9","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Store_X_VITIS_LOOP_88_6_VITIS_LOOP_89_7","ID" : "10","Type" : "pipeline"},]},]
}]}