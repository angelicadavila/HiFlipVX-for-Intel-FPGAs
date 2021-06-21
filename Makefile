#************************************************************************** 
#
# Makefile for intel FPGA devices 
#
# MANUFACTURER is the flag for the library to eneable INTEL/XILINX 
# FPGA_FAMILY specifies the device type for intel
#
#  usage:
#        make canny-test-x86-64  : to test the code in CPU
#        make canny-fpga         : to generate RTL for FPGA
#
#  output:
#		 test-x86-64    :Bynary for CPU
#						: Input- .bmp image
#        HiFlipVX_Intel : contains RTL for FPGa
#
#  Author: Ang√lica D√vila -Universidad de Zaragoza
#
#************************************************************************** 

MANUFACTURER :=Intel
#uncomment for stratix 10 GX - default
#FPGA_FAMILY :=Stratix10 
FPGA_FAMILY := 1sm21bhu2f53e2vgs1

#Read the board BSP configuration based on enviroment variable of custom  FPGAs
#All is based on Intel OpenCL FPGA installation
BOARD_DEFAULT := $(shell xmllint --xpath 'string(//board_env/hardware/@default)' $(AOCL_BOARD_PACKAGE_ROOT)/board_env.xml) 
$(info default BSP Board: $(BOARD_DEFAULT))
#With the default board, access to memory configuration
MEM_WIDTH_BOARD := $(strip $(shell xmllint --xpath 'string(//board/global_mem/interface/@width)' $(AOCL_BOARD_PACKAGE_ROOT)/hardware/$(strip $(BOARD_DEFAULT))/board_spec.xml)) 
$(info Memory width DQ*BL : $(MEM_WIDTH_BOARD))
SOURCE_FILES := ./include/bmp_tool/bmp_tools.cpp


CXX :=i++
override CXXFLAGS := $(CXXFLAGS)
VERBOSE := 1

INC := -I./include -I./include/bmp_tool -I./graph_test

TARGET_DIR := bin_host

.PHONY: default
default: canny-fpga
#default: census-fpga

###########################################################################
#whitout modelsim simulation
.PHONY:canny-fpga
canny-fpga: 	CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) -D WIDTH_MEM=$(MEM_WIDTH_BOARD) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
canny-fpga:   SOURCE_FILES := $(SOURCE_FILES) ./graph_test/canny.cpp 
canny-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "--------------for Intel FPGAS -----------------"
	@echo "-------------------CANNY ----------------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "-----------------------------------------------"	

.PHONY: canny-test-x86-64
canny-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
canny-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/canny.cpp 
canny-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)

CLEAN_FILES := test-x86-64\
		HFVX_fpga\

###########################################################################
###########################################################################
.PHONY:census-fpga
census-fpga: 	CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
census-fpga:   SOURCE_FILES := $(SOURCE_FILES) ./graph_test/census.cpp 
census-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "--------------for Intel FPGAS  -----------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "-------------------CENSUS ---------------------"
	@echo "-----------------------------------------------"	

.PHONY: census-test-x86-64
census-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
census-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/census.cpp 
census-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)


###########################################################################
###########################################################################
.PHONY:autoc-fpga
autoc-fpga: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
autoc-fpga:  SOURCE_FILES := $(SOURCE_FILES) ./graph_test/autoc8.cpp 
autoc-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "-------------for Intel FPGAS  -----------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "----------------AUTO CONTRAST------------------"
	@echo "-----------------------------------------------"	

.PHONY: autoc-test-x86-64
autoc-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
autoc-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/autoc8.cpp 
autoc-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)

###########################################################################
#whitout modelsim simulation
.PHONY:sobel-fpga
sobel-fpga:	CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
sobel-fpga:	SOURCE_FILES := $(SOURCE_FILES) ./graph_test/sobel.cpp
sobel-fpga:	$(SOURCE_FILES)
		$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
		@echo "-----------------------------------------------" 
		@echo "------------COMPILING HIFLIPVX-----------------"
		@echo "--------------for Intel FPGAS -----------------"
		@echo "----------------- SOBEL  ----------------------"
		@echo "---------------- UZ - TUD ---------------------"
		@echo "-----------------------------------------------" 

###########################################################################
#whitout modelsim simulation
.PHONY:sobel-tiling-fpga
sobel-tiling-fpga:	CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
sobel-tiling-fpga:	SOURCE_FILES := $(SOURCE_FILES) ./graph_test/sobel_tiling.cpp
sobel-tiling-fpga:	$(SOURCE_FILES)
		$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
		@echo "-----------------------------------------------" 
		@echo "------------COMPILING HIFLIPVX-----------------"
		@echo "--------------for Intel FPGAS -----------------"
		@echo "-------------- SOBEL TILING  ------------------"
		@echo "---------------- UZ - TUD ---------------------"
		@echo "-----------------------------------------------" 

.PHONY:sobel-tiling4-fpga
sobel-tiling4-fpga:	CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
sobel-tiling4-fpga:	SOURCE_FILES := $(SOURCE_FILES) ./graph_test/sobel_tiling4.cpp
sobel-tiling4-fpga:	$(SOURCE_FILES)
		$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
		@echo "-----------------------------------------------" 
		@echo "------------COMPILING HIFLIPVX-----------------"
		@echo "--------------for Intel FPGAS -----------------"
		@echo "-------------- SOBEL TILING  ------------------"
		@echo "--------------  4 TILES ---- ------------------"
		@echo "---------------- UZ - TUD ---------------------"
		@echo "-----------------------------------------------" 

###########################################################################
###########################################################################
.PHONY:copy-fpga
copy-fpga: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
copy-fpga:  SOURCE_FILES := $(SOURCE_FILES) ./graph_test/copy_test.cpp 
copy-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "-------------for Intel FPGAS  -----------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "---------------COPY CONTRAST ------------------"
	@echo "-----------------------------------------------"	

.PHONY: copy-test-x86-64
copy-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
copy-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/copy_test.cpp 
copy-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)



###########################################################################
###########################################################################
.PHONY:autoc8-fpga
autoc8-fpga: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
autoc8-fpga:  SOURCE_FILES := $(SOURCE_FILES) ./graph_test/autoc8.cpp 
autoc8-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "-------------for Intel FPGAS  -----------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "----------------AUTO CONTRAST------------------"
	@echo "-----------------------------------------------"	

.PHONY: autoc8-test-x86-64
autoc8-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
autoc8-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/autoc8.cpp 
autoc8-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)




###########################################################################
###########################################################################
.PHONY:skinTone-fpga
skinTone-fpga: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) -march=$(FPGA_FAMILY) --simulator none -v -o HiFlipVX_Intel
skinTone-fpga:  SOURCE_FILES := $(SOURCE_FILES) ./graph_test/skinTone.cpp 
skinTone-fpga: 	$(SOURCE_FILES)
	$(CXX) $(SOURCE_FILES) $(CXXFLAGS) 
	@echo "-----------------------------------------------"	
	@echo "------------COMPILING HIFLIPVX-----------------"
	@echo "-------------for Intel FPGAS  -----------------"
	@echo "---------------- UZ - TUD ---------------------"
	@echo "---------- Skyn Tone Detection ----------------"
	@echo "-----------------------------------------------"	

.PHONY: skinTone-test-x86-64
skinTone-test-x86-64: CXXFLAGS := $(CXXFLAGS) -D $(MANUFACTURER) $(INC) $(HLS_CXX_FLAGS) -march=x86-64 -o test-x86-64 
skinTone-test-x86-64:SOURCE_FILES := $(SOURCE_FILES) ./graph_test/skinTone.cpp 
skinTone-test-x86-64: $(SOURCE_FILES)
	     $(ECHO)$ $(CXX) $(SOURCE_FILES) $(CXXFLAGS)

