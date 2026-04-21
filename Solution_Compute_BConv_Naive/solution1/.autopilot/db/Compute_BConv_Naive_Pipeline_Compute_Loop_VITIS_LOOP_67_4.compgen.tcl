# This script segment is generated automatically by AutoPilot

set name Compute_BConv_Naive_mul_64ns_64ns_128_2_1
if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler $name BINDTYPE {op} TYPE {mul} IMPL {auto} LATENCY 1 ALLOW_PRAGMA 1
}


if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler Compute_BConv_Naive_sparsemux_11_3_64_1_1 BINDTYPE {op} TYPE {sparsemux} IMPL {auto}
}


set name Compute_BConv_Naive_urem_128ns_64ns_64_132_1
if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler $name BINDTYPE {op} TYPE {urem} IMPL {auto} LATENCY 131 ALLOW_PRAGMA 1
}


set name Compute_BConv_Naive_urem_64ns_64ns_64_68_1
if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler $name BINDTYPE {op} TYPE {urem} IMPL {auto} LATENCY 67 ALLOW_PRAGMA 1
}


# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 46 \
    name local_in_x \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename local_in_x \
    op interface \
    ports { local_in_x_address0 { O 12 vector } local_in_x_ce0 { O 1 bit } local_in_x_q0 { I 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_in_x'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 47 \
    name local_in_x_1 \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename local_in_x_1 \
    op interface \
    ports { local_in_x_1_address0 { O 12 vector } local_in_x_1_ce0 { O 1 bit } local_in_x_1_q0 { I 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_in_x_1'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 48 \
    name local_in_x_2 \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename local_in_x_2 \
    op interface \
    ports { local_in_x_2_address0 { O 12 vector } local_in_x_2_ce0 { O 1 bit } local_in_x_2_q0 { I 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_in_x_2'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 49 \
    name local_out_x \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x \
    op interface \
    ports { local_out_x_address1 { O 12 vector } local_out_x_ce1 { O 1 bit } local_out_x_we1 { O 1 bit } local_out_x_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 50 \
    name local_out_x_1 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_1 \
    op interface \
    ports { local_out_x_1_address1 { O 12 vector } local_out_x_1_ce1 { O 1 bit } local_out_x_1_we1 { O 1 bit } local_out_x_1_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_1'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 51 \
    name local_out_x_2 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_2 \
    op interface \
    ports { local_out_x_2_address1 { O 12 vector } local_out_x_2_ce1 { O 1 bit } local_out_x_2_we1 { O 1 bit } local_out_x_2_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_2'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 52 \
    name local_out_x_3 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_3 \
    op interface \
    ports { local_out_x_3_address1 { O 12 vector } local_out_x_3_ce1 { O 1 bit } local_out_x_3_we1 { O 1 bit } local_out_x_3_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_3'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 53 \
    name local_out_x_4 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_4 \
    op interface \
    ports { local_out_x_4_address1 { O 12 vector } local_out_x_4_ce1 { O 1 bit } local_out_x_4_we1 { O 1 bit } local_out_x_4_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_4'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 44 \
    name sizeP \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_sizeP \
    op interface \
    ports { sizeP { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 45 \
    name zext_ln66_1 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_zext_ln66_1 \
    op interface \
    ports { zext_ln66_1 { I 44 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 54 \
    name local_mod_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_mod_reload \
    op interface \
    ports { local_mod_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 55 \
    name local_mod_1_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_mod_1_reload \
    op interface \
    ports { local_mod_1_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 56 \
    name local_mod_2_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_mod_2_reload \
    op interface \
    ports { local_mod_2_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 57 \
    name local_mod_3_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_mod_3_reload \
    op interface \
    ports { local_mod_3_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 58 \
    name local_mod_4_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_mod_4_reload \
    op interface \
    ports { local_mod_4_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 59 \
    name local_w_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_reload \
    op interface \
    ports { local_w_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 60 \
    name local_w_1_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_1_reload \
    op interface \
    ports { local_w_1_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 61 \
    name local_w_2_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_2_reload \
    op interface \
    ports { local_w_2_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 62 \
    name local_w_3_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_3_reload \
    op interface \
    ports { local_w_3_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 63 \
    name local_w_4_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_4_reload \
    op interface \
    ports { local_w_4_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 64 \
    name local_w_5_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_5_reload \
    op interface \
    ports { local_w_5_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 65 \
    name local_w_6_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_6_reload \
    op interface \
    ports { local_w_6_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 66 \
    name local_w_7_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_7_reload \
    op interface \
    ports { local_w_7_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 67 \
    name local_w_8_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_8_reload \
    op interface \
    ports { local_w_8_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 68 \
    name local_w_9_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_9_reload \
    op interface \
    ports { local_w_9_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 69 \
    name local_w_10_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_10_reload \
    op interface \
    ports { local_w_10_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 70 \
    name local_w_11_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_11_reload \
    op interface \
    ports { local_w_11_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 71 \
    name local_w_12_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_12_reload \
    op interface \
    ports { local_w_12_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 72 \
    name local_w_13_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_13_reload \
    op interface \
    ports { local_w_13_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 73 \
    name local_w_14_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_local_w_14_reload \
    op interface \
    ports { local_w_14_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id -1 \
    name ap_ctrl \
    type ap_ctrl \
    reset_level 1 \
    sync_rst true \
    corename ap_ctrl \
    op interface \
    ports { ap_start { I 1 bit } ap_ready { O 1 bit } ap_done { O 1 bit } ap_idle { O 1 bit } } \
} "
}


# Adapter definition:
set PortName ap_clk
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_clock] == "cg_default_interface_gen_clock"} {
eval "cg_default_interface_gen_clock { \
    id -2 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_clk \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-113\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}


# Adapter definition:
set PortName ap_rst
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_reset] == "cg_default_interface_gen_reset"} {
eval "cg_default_interface_gen_reset { \
    id -3 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_rst \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-114\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}



# merge
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_end
    cg_default_interface_gen_bundle_end
    AESL_LIB_XILADAPTER::native_axis_end
}


# flow_control definition:
set InstName Compute_BConv_Naive_flow_control_loop_pipe_sequential_init_U
set CompName Compute_BConv_Naive_flow_control_loop_pipe_sequential_init
set name flow_control_loop_pipe_sequential_init
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control] == "::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control"} {
eval "::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control { \
    name ${name} \
    prefix Compute_BConv_Naive_ \
}"
} else {
puts "@W \[IMPL-107\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler $CompName BINDTYPE interface TYPE internal_upc_flow_control INSTNAME $InstName
}


