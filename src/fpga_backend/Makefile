# ===================================================================
# Unified FPGA Makefile for OpenFHE-CKKS Acceleration
# 
# Features: 
#   1. Vitis Flow: sw_emu, hw_emu, hw (System Integration)
#   2. HLS Flow:   csim, csynth       (Unit Test & Analysis)
# ===================================================================

# # --- 1. 基础配置 ---
# PLATFORM ?= xilinx_u55c_gen3x16_xdma_4_1_202310_1
# HLS_PART := xcu55c-fsvh2892-2L-e 

# PLATFORM ?= xilinx_u55c_gen3x16_xdma_3_202210_1

PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1
# U250 旧平台配置，已改为 U55C

TARGET   ?= hw

# 目录设置
SRC_DIR    := ./src
INC_DIR    := ./include
BUILD_DIR  := ./build/$(TARGET)

# 源文件定义 (主要用于 Vitis Flow 打包，HLS Flow 依赖你的 TCL 脚本配置)
KERNEL_SRC := $(SRC_DIR)/top.cpp 
KERNEL_SRC += $(SRC_DIR)/load.cpp
KERNEL_SRC += $(SRC_DIR)/mod_add_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_mult_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_sub_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/ntt_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/arithmetic.cpp
KERNEL_SRC += $(SRC_DIR)/bconv.cpp
KERNEL_SRC += $(SRC_DIR)/interleave.cpp
KERNEL_SRC += $(SRC_DIR)/auto.cpp
KERNEL_SRC += $(SRC_DIR)/cg_ntt.cpp
KERNEL_SRC += $(SRC_DIR)/bconv_naive.cpp
KERNEL_SRC += $(SRC_DIR)/bconv_systolic.cpp

KERNEL_NAME := Top

# OpenFHE 路径
OPENFHE_ROOT := /home/CONNECT/xmeng027/work/HKS-Accelerator-hanxu/openfhe/
OPENFHE_INC  := -I$(OPENFHE_ROOT)/src/pke/include \
                -I$(OPENFHE_ROOT)/src/core/include \
                -I$(OPENFHE_ROOT)/build/src/core \
                -I$(OPENFHE_ROOT)/third-party/cereal/include

# XRT 路径
XRT_PATH := /opt/xilinx/xrt
XRT_INC  := -I$(XRT_PATH)/include

# 编译器
CXX       := g++
VPP       := v++
VITIS_HLS := vitis_hls

# --- 输出产物 ---
XCLBIN   := $(BUILD_DIR)/fhe_kernels_$(TARGET).xclbin
XO_FILE  := $(BUILD_DIR)/temp.xo
HOST_LIB := $(BUILD_DIR)/libfpga_backend.a

# 编译选项
CXXFLAGS := -std=c++17 -O3 -g -Wall -DOPENFHE_FPGA_ENABLE -fPIC
CXXFLAGS += -I$(INC_DIR) $(OPENFHE_INC) $(XRT_INC)

# Kernel 编译选项 (Vitis Flow)
VPP_FLAGS := -t $(TARGET) --platform $(PLATFORM) --save-temps
VPP_FLAGS += -I$(INC_DIR) -I$(SRC_DIR)
# VPP_FLAGS += --config connectivity.ini  # HBM 连接配置，需根据实际平台调整

# ===================================================================
# 2. 构建规则 (Vitis Flow - 保持不变)
# ===================================================================

REPORTS_OUT := ../../docs/reports/bconv

.PHONY: all clean clean_all emconfig xclbin sw_emu hw_emu hw csim csynth cosim check_module test_auto report report_all

all: $(BUILD_DIR) $(HOST_LIB) $(XCLBIN) emconfig

# --- 2.1 Host Library ---
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(HOST_LIB): $(BUILD_DIR)
	@echo "-> Building Host Library placeholder..."
	touch $@

# --- 2.2 FPGA Kernel (.xclbin) ---
xclbin: $(XCLBIN)

$(XO_FILE): $(KERNEL_SRC) | $(BUILD_DIR)
	@echo "-> Synthesizing Kernel (Vitis HLS)..."
	$(VPP) -c $(VPP_FLAGS) -k $(KERNEL_NAME) -o $@ $^

$(XCLBIN): $(XO_FILE)
	@echo "-> Linking Kernel (Vitis Link)..."
	$(VPP) -l $(VPP_FLAGS) -o $@ $^

emconfig:
	@echo "-> Generating emconfig.json..."
	emconfigutil --platform $(PLATFORM) --nd 1

# --- 2.3 Vitis 快捷方式 ---
# C model sim
sw_emu:
	$(MAKE) all TARGET=sw_emu 

# RTL sim
hw_emu:
	$(MAKE) all TARGET=hw_emu

# bitstream exe
hw:
	$(MAKE) all TARGET=hw

# ===================================================================
# 3. HLS 独立流程 (csim / csynth / cosim)
# ===================================================================

# 指定你的现有 TCL 脚本文件名
CSIM_SCRIPT   := csim.tcl
CSYNTH_SCRIPT := csynth.tcl
COSIM_SCRIPT  := cosim.tcl

# -------------------------------------------------------------------
# HLS 动态模块选择配置 (HLS Dynamic Module Selection)
# -------------------------------------------------------------------
# 使用方式 (MODULE 取值 = HLS 顶层函数名):
#   make csim   MODULE=Compute_CG_NTT
#   make csynth MODULE=Top
#   make cosim  MODULE=Compute_BConv
#   make cosim  MODULE=Compute_BConv_Naive
#   make csim   MODULE=NTT_Kernel
MODULE ?= Compute_CG_NTT

# --- 模块注册表 ---
# 1. Compute_CG_NTT 模块
SRC_Compute_CG_NTT := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/ntt_kernel.cpp $(SRC_DIR)/cg_ntt.cpp
TB_Compute_CG_NTT  := ./testbench/cg_ntt_tb.cpp

# 2. Top 模块 (复用 Vitis 流程的 KERNEL_SRC，并额外加上 cg_ntt / bconv_naive)
SRC_Top := $(KERNEL_SRC) $(SRC_DIR)/cg_ntt.cpp $(SRC_DIR)/bconv_naive.cpp
TB_Top  := ./testbench/top_tb.cpp

# 3. NTT kernel 子模块 (顶层函数: NTT_Kernel)
SRC_NTT_Kernel := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/ntt_kernel.cpp
TB_NTT_Kernel  := ./testbench/ntt_kernel_tb.cpp
CF_NTT_Kernel  := -DNTT_TB_STANDALONE

# 3b. Compute_NTT 子模块 (顶层函数: Compute_NTT，多 limb 包装 NTT_Kernel)
SRC_Compute_NTT := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/ntt_kernel.cpp
TB_Compute_NTT  := ./testbench/ntt_kernel_tb.cpp
CF_Compute_NTT  := -DNTT_TB_STANDALONE

# 4. bconv 子模块 (优化版，顶层函数: Compute_BConv)
SRC_Compute_BConv := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/bconv.cpp
TB_Compute_BConv  := ./testbench/bconv_tb.cpp
CF_Compute_BConv  :=

# 5. bconv_naive 子模块 (顶层函数: Compute_BConv_Naive)
# 通过 -DBCONV_USE_NAIVE 让 bconv_tb.cpp 调用 naive 版本，复用全部测试用例
SRC_Compute_BConv_Naive := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/bconv_naive.cpp
TB_Compute_BConv_Naive  := ./testbench/bconv_tb.cpp
CF_Compute_BConv_Naive  := -DBCONV_USE_NAIVE

# 6. bconv_systolic 子模块 (顶层函数: Compute_BConv_Systolic)
SRC_Compute_BConv_Systolic := $(SRC_DIR)/arithmetic.cpp $(SRC_DIR)/bconv_systolic.cpp
TB_Compute_BConv_Systolic  := ./testbench/bconv_systolic_tb.cpp
CF_Compute_BConv_Systolic  :=

# 按需继续添加:
# SRC_mod_add_kernel := $(SRC_DIR)/mod_add_kernel.cpp
# TB_mod_add_kernel  := ./testbench/mod_add_tb.cpp

# --- 导出给 TCL 使用的环境变量 ---
export HLS_TOP_FUNC     := $(MODULE)
export HLS_SRC_FILES    := $(SRC_$(MODULE))
export HLS_TB_FILES     := $(TB_$(MODULE))
export HLS_EXTRA_CFLAGS := $(CF_$(MODULE))

# 安全检查：防止 MODULE 拼写错误
check_module:
	@if [ -z "$(HLS_SRC_FILES)" ]; then \
		echo "Error: No source files defined for MODULE=$(MODULE)."; \
		echo "       Please define SRC_$(MODULE) in Makefile."; \
		exit 1; \
	fi
	@echo "-> HLS MODULE     : $(MODULE)"
	@echo "-> HLS SRC FILES  : $(HLS_SRC_FILES)"
	@echo "-> HLS TB  FILES  : $(HLS_TB_FILES)"

# --- 3.1 C Simulation (验证 C 代码逻辑) ---
csim: check_module
	@echo "-> Running HLS C Simulation using $(CSIM_SCRIPT)..."
	@if [ ! -f $(CSIM_SCRIPT) ]; then \
		echo "Error: $(CSIM_SCRIPT) not found!"; exit 1; \
	fi
	$(VITIS_HLS) -f $(CSIM_SCRIPT)

# --- 3.2 C Synthesis (查看资源和时序) ---
csynth: check_module
	@echo "-> Running HLS C Synthesis using $(CSYNTH_SCRIPT)..."
	@if [ ! -f $(CSYNTH_SCRIPT) ]; then \
		echo "Error: $(CSYNTH_SCRIPT) not found!"; exit 1; \
	fi
	$(VITIS_HLS) -f $(CSYNTH_SCRIPT)
	@echo "-> Done. Check synthesis reports in your project directory."

# --- 3.3 C/RTL Co-simulation (联合仿真，查看波形) ---
cosim: check_module
	@echo "-> Running HLS C/RTL Co-simulation using $(COSIM_SCRIPT)..."
	@if [ ! -f $(COSIM_SCRIPT) ]; then \
		echo "Error: $(COSIM_SCRIPT) not found!"; exit 1; \
	fi
	$(VITIS_HLS) -f $(COSIM_SCRIPT)
	@echo "-> Done. Check cosim reports/waveforms in Solution/cosim_$(MODULE)."

# ===================================================================
# 4. 报告提拔 (将综合/仿真结果提升到 docs/reports/ 纳入 Git)
# ===================================================================
# 用法:
#   make report                      # 提拔当前 MODULE 的报告 (默认 MODULE=Compute_CG_NTT)
#   make report MODULE=Compute_BConv_Systolic
#   make report_all                  # 提拔全部已知 BConv 模块的报告
report: check_module
	@SOLN_DIR=Solution/cosim_$(MODULE)/solution1; \
	mkdir -p $(REPORTS_OUT); \
	if [ -f "$$SOLN_DIR/syn/report/csynth.rpt" ]; then \
		cp "$$SOLN_DIR/syn/report/csynth.rpt" "$(REPORTS_OUT)/$(MODULE)_csynth.rpt"; \
		echo "  [+] $(MODULE)_csynth.rpt"; \
	else \
		echo "  [-] csynth.rpt not found (run 'make csynth MODULE=$(MODULE)' first)"; \
	fi; \
	if [ -f "$$SOLN_DIR/sim/report/$(MODULE)_cosim.rpt" ]; then \
		cp "$$SOLN_DIR/sim/report/$(MODULE)_cosim.rpt" "$(REPORTS_OUT)/$(MODULE)_cosim.rpt"; \
		echo "  [+] $(MODULE)_cosim.rpt"; \
	else \
		echo "  [-] $(MODULE)_cosim.rpt not found (run 'make cosim MODULE=$(MODULE)' first)"; \
	fi
	@echo "-> Reports updated in $(REPORTS_OUT)/"
	@echo "-> Run: git add docs/reports/ && git commit"

BCONV_MODULES := Compute_BConv_Naive Compute_BConv Compute_BConv_Systolic

report_all:
	@for m in $(BCONV_MODULES); do \
		$(MAKE) --no-print-directory report MODULE=$$m; \
	done

# ===================================================================
# 5. 清理
# ===================================================================
clean:
	@echo "-> Cleaning build artifacts for TARGET=$(TARGET)..."
	rm -rf $(BUILD_DIR) emconfig.json .Xil .run *.log *.jou

	@echo "-> Cleaning Vitis/VPP cache..."
	rm -rf _x v++_*_backup.log

	@echo "-> Cleaning HLS projects..."
	rm -rf hls_prj_* Solution Solution_* vitis_hls.log

clean_all:
	@echo "-> Cleaning all build targets..."
	rm -rf ./build emconfig.json .Xil .run *.log *.jou _x v++_*_backup.log
	rm -rf hls_prj_* Solution Solution_* vitis_hls.log