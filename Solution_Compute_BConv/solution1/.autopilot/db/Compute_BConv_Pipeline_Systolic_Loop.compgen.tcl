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
    id 53 \
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
    id 54 \
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
    id 55 \
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
    id 56 \
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
    id 57 \
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
    id 58 \
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
    id 59 \
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
    id 60 \
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
    name cmp261_i_4 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_cmp261_i_4 \
    op interface \
    ports { cmp261_i_4 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 50 \
    name icmp42 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_icmp42 \
    op interface \
    ports { icmp42 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 51 \
    name cmp261_i_2 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_cmp261_i_2 \
    op interface \
    ports { cmp261_i_2 { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 52 \
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
    id 61 \
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
    id 62 \
    name mod_p_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_mod_p_reload \
    op interface \
    ports { mod_p_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 63 \
    name p_reload139 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload139 \
    op interface \
    ports { p_reload139 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 64 \
    name p_reload140 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload140 \
    op interface \
    ports { p_reload140 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 65 \
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
    id 66 \
    name mod_p_1_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_mod_p_1_reload \
    op interface \
    ports { mod_p_1_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 67 \
    name p_reload141 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload141 \
    op interface \
    ports { p_reload141 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 68 \
    name p_reload142 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload142 \
    op interface \
    ports { p_reload142 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 69 \
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
    id 70 \
    name mod_p_2_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_mod_p_2_reload \
    op interface \
    ports { mod_p_2_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 71 \
    name p_reload143 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload143 \
    op interface \
    ports { p_reload143 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 72 \
    name p_reload144 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload144 \
    op interface \
    ports { p_reload144 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 73 \
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
    id 74 \
    name p_reload147 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload147 \
    op interface \
    ports { p_reload147 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 75 \
    name p_reload145 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload145 \
    op interface \
    ports { p_reload145 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 76 \
    name p_reload146 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload146 \
    op interface \
    ports { p_reload146 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 77 \
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
    id 78 \
    name mod_p_4_reload \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_mod_p_4_reload \
    op interface \
    ports { mod_p_4_reload { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 79 \
    name p_reload137 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload137 \
    op interface \
    ports { p_reload137 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 80 \
    name p_reload138 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload138 \
    op interface \
    ports { p_reload138 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 81 \
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
    id 82 \
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
    id 83 \
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
    id 84 \
    name p_reload124 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_p_reload124 \
    op interface \
    ports { p_reload124 { I 64 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 85 \
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
    id 86 \
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
    id 87 \
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
    id 88 \
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
    id 89 \
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
    id 90 \
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
    id 91 \
    name icmp_ln166 \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_icmp_ln166 \
    op interface \
    ports { icmp_ln166 { I 1 vector } } \
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


