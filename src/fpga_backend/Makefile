# ===================================================================
# Unified FPGA Makefile for OpenFHE-CKKS Acceleration
# 
# Features: 
#   1. Vitis Flow: sw_emu, hw_emu, hw (System Integration)
#   2. HLS Flow:   csim, csynth       (Unit Test & Analysis)
# ===================================================================

# # --- 1. 基础配置 ---
PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1
HLS_PART := xcu250-figd2104-2L-e 

# PLATFORM ?= xilinx_u55c_gen3x16_xdma_3_202210_1
# U55C 使用的是 UltraScale+ XCU55C 芯片
# HLS_PART := xcu55c-fsvh2892-2L-e

TARGET   ?= hw

# 目录设置
SRC_DIR    := ./src
INC_DIR    := ./include

# 源文件定义 (主要用于 Vitis Flow 打包，HLS Flow 依赖你的 TCL 脚本配置)
KERNEL_SRC := $(SRC_DIR)/top.cpp 
# KERNEL_SRC += $(SRC_DIR)/arithmetic.cpp
KERNEL_SRC += $(SRC_DIR)/load.cpp
KERNEL_SRC += $(SRC_DIR)/mod_add_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_mult_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_sub_kernel.cpp

KERNEL_NAME := Top

# OpenFHE 路径
OPENFHE_ROOT := /home/CONNECT/xmeng027/work/FHE-BERT-Tiny/openfhe-development-1.4.0
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
XCLBIN   := fhe_kernels_$(TARGET).xclbin
HOST_LIB := libfpga_backend.a

# 编译选项
CXXFLAGS := -std=c++17 -O3 -g -Wall -DOPENFHE_FPGA_ENABLE -fPIC
CXXFLAGS += -I$(INC_DIR) $(OPENFHE_INC) $(XRT_INC)

# Kernel 编译选项 (Vitis Flow)
VPP_FLAGS := -t $(TARGET) --platform $(PLATFORM) --save-temps
VPP_FLAGS += -I$(INC_DIR) -I$(SRC_DIR)


# ===================================================================
# 2. 构建规则 (Vitis Flow - 保持不变)
# ===================================================================

.PHONY: all clean emconfig xclbin sw_emu hw_emu hw csim csynth

all: $(HOST_LIB) $(XCLBIN) emconfig

# --- 2.1 Host Library ---
$(HOST_LIB):
	@echo "-> Building Host Library placeholder..."
	touch $@

# --- 2.2 FPGA Kernel (.xclbin) ---
xclbin: $(XCLBIN)

temp.xo: $(KERNEL_SRC)
	@echo "-> Synthesizing Kernel (Vitis HLS)..."
	$(VPP) -c $(VPP_FLAGS) -k $(KERNEL_NAME) -o $@ $<

$(XCLBIN): temp.xo
	@echo "-> Linking Kernel (Vitis Link)..."
	$(VPP) -l $(VPP_FLAGS) -o $@ $<

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
# 3. HLS 独立流程 (csim / csynth)
# ===================================================================

# 指定你的现有 TCL 脚本文件名
CSIM_SCRIPT   := csim.tcl
CSYNTH_SCRIPT := csynth.tcl

# --- 3.1 C Simulation (验证 C 代码逻辑) ---
csim:
	@echo "-> Running HLS C Simulation using $(CSIM_SCRIPT)..."
	@if [ ! -f $(CSIM_SCRIPT) ]; then \
		echo "Error: $(CSIM_SCRIPT) not found!"; exit 1; \
	fi
	$(VITIS_HLS) -f $(CSIM_SCRIPT)

# --- 3.2 C Synthesis (查看资源和时序) ---
csynth:
	@echo "-> Running HLS C Synthesis using $(CSYNTH_SCRIPT)..."
	@if [ ! -f $(CSYNTH_SCRIPT) ]; then \
		echo "Error: $(CSYNTH_SCRIPT) not found!"; exit 1; \
	fi
	$(VITIS_HLS) -f $(CSYNTH_SCRIPT)
	@echo "-> Done. Check synthesis reports in your project directory."

# ===================================================================
# 4. 清理
# ===================================================================
clean:
	@echo "-> Cleaning local build artifacts..."
	rm -rf $(XCLBIN) $(HOST_LIB) temp.xo emconfig.json .Xil .run *.log *.jou

	@echo "-> Cleaning Vitis/VPP cache..."
	rm -rf _x v++_*_backup.log

	@echo "-> Cleaning HLS projects..."
# 注意：这里会删除生成的工程文件夹 hls_prj_csynth 等
# 但不会删除你的 run_csynth.tcl 脚本
	rm -rf hls_prj_* Solution vitis_hls.log