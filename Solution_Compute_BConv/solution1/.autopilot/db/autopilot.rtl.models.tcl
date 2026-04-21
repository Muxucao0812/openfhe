set SynModuleInfo {
  {SRCNAME Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1 MODELNAME Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1 RTLNAME Compute_BConv_Compute_BConv_Pipeline_Load_W_VITIS_LOOP_214_1
    SUBMODULES {
      {MODELNAME Compute_BConv_flow_control_loop_pipe_sequential_init RTLNAME Compute_BConv_flow_control_loop_pipe_sequential_init BINDTYPE interface TYPE internal_upc_flow_control INSTNAME Compute_BConv_flow_control_loop_pipe_sequential_init_U}
    }
  }
  {SRCNAME Compute_BConv_Pipeline_Load_Mod MODELNAME Compute_BConv_Pipeline_Load_Mod RTLNAME Compute_BConv_Compute_BConv_Pipeline_Load_Mod}
  {SRCNAME Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3 MODELNAME Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3 RTLNAME Compute_BConv_Compute_BConv_Pipeline_Load_X_VITIS_LOOP_229_2_VITIS_LOOP_230_3}
  {SRCNAME MultMod MODELNAME MultMod RTLNAME Compute_BConv_MultMod
    SUBMODULES {
      {MODELNAME Compute_BConv_mul_64ns_64ns_128_5_1 RTLNAME Compute_BConv_mul_64ns_64ns_128_5_1 BINDTYPE op TYPE mul IMPL dsp LATENCY 4 ALLOW_PRAGMA 1}
      {MODELNAME Compute_BConv_mul_64s_64s_64_5_1 RTLNAME Compute_BConv_mul_64s_64s_64_5_1 BINDTYPE op TYPE mul IMPL dsp LATENCY 4 ALLOW_PRAGMA 1}
    }
  }
  {SRCNAME Compute_BConv_Pipeline_Systolic_Loop MODELNAME Compute_BConv_Pipeline_Systolic_Loop RTLNAME Compute_BConv_Compute_BConv_Pipeline_Systolic_Loop}
  {SRCNAME Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5 MODELNAME Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5 RTLNAME Compute_BConv_Compute_BConv_Pipeline_Store_X_VITIS_LOOP_244_4_VITIS_LOOP_245_5
    SUBMODULES {
      {MODELNAME Compute_BConv_sparsemux_11_3_64_1_1 RTLNAME Compute_BConv_sparsemux_11_3_64_1_1 BINDTYPE op TYPE sparsemux IMPL auto}
    }
  }
  {SRCNAME Compute_BConv MODELNAME Compute_BConv RTLNAME Compute_BConv IS_TOP 1
    SUBMODULES {
      {MODELNAME Compute_BConv_local_in_x_RAM_2P_BRAM_1R1W RTLNAME Compute_BConv_local_in_x_RAM_2P_BRAM_1R1W BINDTYPE storage TYPE ram_2p IMPL bram LATENCY 2 ALLOW_PRAGMA 1}
    }
  }
}
