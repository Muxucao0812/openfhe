# This script segment is generated automatically by AutoPilot

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
    id 77 \
    name local_out_x_8 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_8 \
    op interface \
    ports { local_out_x_8_address1 { O 12 vector } local_out_x_8_ce1 { O 1 bit } local_out_x_8_we1 { O 1 bit } local_out_x_8_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_8'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 78 \
    name local_out_x_7 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_7 \
    op interface \
    ports { local_out_x_7_address1 { O 12 vector } local_out_x_7_ce1 { O 1 bit } local_out_x_7_we1 { O 1 bit } local_out_x_7_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_7'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 79 \
    name local_out_x_6 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_6 \
    op interface \
    ports { local_out_x_6_address1 { O 12 vector } local_out_x_6_ce1 { O 1 bit } local_out_x_6_we1 { O 1 bit } local_out_x_6_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_6'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 80 \
    name local_out_x_5 \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename local_out_x_5 \
    op interface \
    ports { local_out_x_5_address1 { O 12 vector } local_out_x_5_ce1 { O 1 bit } local_out_x_5_we1 { O 1 bit } local_out_x_5_d1 { O 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_out_x_5'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 81 \
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
    id 82 \
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
    id 87 \
    name local_in_x_3 \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename local_in_x_3 \
    op interface \
    ports { local_in_x_3_address0 { O 12 vector } local_in_x_3_ce0 { O 1 bit } local_in_x_3_q0 { I 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_in_x_3'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 89 \
    name local_in_x_4 \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename local_in_x_4 \
    op interface \
    ports { local_in_x_4_address0 { O 12 vector } local_in_x_4_ce0 { O 1 bit } local_in_x_4_q0 { I 64 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'local_in_x_4'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 49 \
    name p_reload83 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload83 \
    op interface \
    ports { p_reload83 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 50 \
    name p_reload111 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload111 \
    op interface \
    ports { p_reload111 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 51 \
    name p_reload109 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload109 \
    op interface \
    ports { p_reload109 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 52 \
    name p_reload110 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload110 \
    op interface \
    ports { p_reload110 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 53 \
    name p_reload88 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload88 \
    op interface \
    ports { p_reload88 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 54 \
    name p_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload \
    op interface \
    ports { p_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 55 \
    name cmp40_i_4 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_cmp40_i_4 \
    op interface \
    ports { cmp40_i_4 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 56 \
    name p_reload87 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload87 \
    op interface \
    ports { p_reload87 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 57 \
    name p_reload123 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload123 \
    op interface \
    ports { p_reload123 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 58 \
    name p_reload121 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload121 \
    op interface \
    ports { p_reload121 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 59 \
    name p_reload122 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload122 \
    op interface \
    ports { p_reload122 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 60 \
    name p_reload92 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload92 \
    op interface \
    ports { p_reload92 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 61 \
    name p_reload82 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload82 \
    op interface \
    ports { p_reload82 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 62 \
    name icmp46 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_icmp46 \
    op interface \
    ports { icmp46 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 63 \
    name p_reload86 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload86 \
    op interface \
    ports { p_reload86 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 64 \
    name p_reload120 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload120 \
    op interface \
    ports { p_reload120 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 65 \
    name p_reload118 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload118 \
    op interface \
    ports { p_reload118 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 66 \
    name p_reload119 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload119 \
    op interface \
    ports { p_reload119 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 67 \
    name p_reload91 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload91 \
    op interface \
    ports { p_reload91 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 68 \
    name p_reload81 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload81 \
    op interface \
    ports { p_reload81 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 69 \
    name cmp40_i_2 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_cmp40_i_2 \
    op interface \
    ports { cmp40_i_2 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 70 \
    name p_reload85 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload85 \
    op interface \
    ports { p_reload85 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 71 \
    name p_reload117 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload117 \
    op interface \
    ports { p_reload117 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 72 \
    name p_reload115 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload115 \
    op interface \
    ports { p_reload115 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 73 \
    name p_reload116 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload116 \
    op interface \
    ports { p_reload116 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 74 \
    name p_reload90 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload90 \
    op interface \
    ports { p_reload90 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 75 \
    name p_reload80 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload80 \
    op interface \
    ports { p_reload80 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 76 \
    name icmp \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_icmp \
    op interface \
    ports { icmp { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 83 \
    name p_reload84 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload84 \
    op interface \
    ports { p_reload84 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 84 \
    name p_reload114 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload114 \
    op interface \
    ports { p_reload114 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 85 \
    name p_reload112 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload112 \
    op interface \
    ports { p_reload112 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 86 \
    name p_reload113 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload113 \
    op interface \
    ports { p_reload113 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 88 \
    name p_reload89 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload89 \
    op interface \
    ports { p_reload89 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 90 \
    name p_reload79 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload79 \
    op interface \
    ports { p_reload79 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 91 \
    name icmp_ln59 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_icmp_ln59 \
    op interface \
    ports { icmp_ln59 { I 1 vector } } \
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
set InstName Compute_BConv_flow_control_loop_pipe_sequential_init_U
set CompName Compute_BConv_flow_control_loop_pipe_sequential_init
set name flow_control_loop_pipe_sequential_init
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control] == "::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control"} {
eval "::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control { \
    name ${name} \
    prefix Compute_BConv_ \
}"
} else {
puts "@W \[IMPL-107\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_UPC_flow_control, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler $CompName BINDTYPE interface TYPE internal_upc_flow_control INSTNAME $InstName
}


