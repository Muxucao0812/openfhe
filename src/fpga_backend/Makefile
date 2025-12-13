# ===================================================================
# Unified FPGA Makefile for OpenFHE-CKKS Acceleration
# 
# Features: 
#   1. Vitis Flow: sw_emu, hw_emu, hw (System Integration)
#   2. HLS Flow:   csim, csynth       (Unit Test & Analysis)
# ===================================================================

# # --- 1. 基础配置 ---
# PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1
# # HLS 需要具体的 Part Number，这里对应 U250
# HLS_PART := xcu250-figd2104-2L-e 


PLATFORM ?= xilinx_u55c_gen3x16_xdma_3_202210_1

# [修改点 2] U55C 的 FPGA 芯片具体型号
# U55C 使用的是 UltraScale+ XCU55C 芯片
HLS_PART := xcu55c-fsvh2892-2L-e


TARGET   ?= hw

# 目录设置
SRC_DIR    := ./src
INC_DIR    := ./include

# 源文件定义
KERNEL_SRC := $(SRC_DIR)/top.cpp 
# KERNEL_SRC += $(SRC_DIR)/arithmetic.cpp
KERNEL_SRC += $(SRC_DIR)/load.cpp
KERNEL_SRC += $(SRC_DIR)/mod_add_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_mult_kernel.cpp
KERNEL_SRC += $(SRC_DIR)/mod_sub_kernel.cpp


KERNEL_NAME := Top

# [测试平台]: 仅用于 make csim
# 如果你的 testbench 写在 top.cpp 里(有main函数)，这里可以是空的
# 否则指向你的测试文件，例如: $(SRC_DIR)/testbench.cpp
TB_SRC     := 

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


# Kernel 编译选项

VPP_FLAGS := -t $(TARGET) --platform $(PLATFORM) --save-temps

VPP_FLAGS += -I$(INC_DIR) -I$(SRC_DIR)


# ===================================================================
# 2. 构建规则 (Vitis Flow)
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
sw_emu:
	$(MAKE) all TARGET=sw_emu

hw_emu:
	$(MAKE) all TARGET=hw_emu

hw:
	$(MAKE) all TARGET=hw

# ===================================================================
# 3. HLS 独立流程 (csim / csynth)
# ===================================================================
# 注意：这里我们动态生成 Tcl 脚本，确保文件列表永远与 Makefile 一致！

# 定义 Tcl 脚本生成函数
define GEN_HLS_TCL
	echo "open_project hls_prj_$(1)"; \
	echo "set_top $(KERNEL_NAME)"; \
	echo "add_files $(KERNEL_SRC) -cflags \"-I$(INC_DIR)\""; \
	if [ ! -z "$(TB_SRC)" ]; then \
		echo "add_files -tb $(TB_SRC) -cflags \"-I$(INC_DIR)\""; \
	fi; \
	echo "open_solution solution1"; \
	echo "set_part $(HLS_PART)"; \
	echo "create_clock -period 3.33"; \
	echo "$(2)_design"; \
	echo "exit"; 
endef

# --- 3.1 C Simulation (验证 C 代码逻辑) ---
csim:
	@echo "-> Running HLS C Simulation..."
	@echo "   (Note: Requires a main() function in $(KERNEL_SRC) or TB_SRC)"
	@($(call GEN_HLS_TCL,csim,csim)) > run_csim.tcl
	$(VITIS_HLS) -f run_csim.tcl

# --- 3.2 C Synthesis (查看资源和时序) ---
csynth:
	@echo "-> Running HLS C Synthesis..."
	@($(call GEN_HLS_TCL,csynth,csynth)) > run_csynth.tcl
	$(VITIS_HLS) -f run_csynth.tcl
	@echo "-> Report generated at: hls_prj_csynth/solution1/syn/report/$(KERNEL_NAME)_csynth.rpt"

# ===================================================================
# 4. 清理
# ===================================================================
clean:
	@echo "-> Cleaning local build artifacts..."
	rm -rf $(XCLBIN) $(HOST_LIB) temp.xo emconfig.json .Xil .run *.log *.jou

	@echo "-> Cleaning Vitis/VPP cache..."
	rm -rf _x v++_*_backup.log

	@echo "-> Cleaning HLS projects & Autopilot DB..."
	rm -rf hls_prj_* Solution *.autopilot vitis_hls.log