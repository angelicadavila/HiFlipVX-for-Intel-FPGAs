# HiFlipVX for Intel FPGAs

This work is an extension of HiFlipVx Library for Xilinx, a programming library 
based on OpenVX. This extension modifies
the programming model for Intel FPGA Devices using the Intel System of Task.

This library supports embbeded and acceletarion with the interoperation with OpenCL as aocl library.

## Main Changes
* This library uses the Intel System of Task to enable concurrency in the OpenVX graph
* Pragma definitions was adapted to Intel standar.
* Makefile added for linux system
* Tool extension to interoperation with OpenCL/SyCL as library

### Prerequisites
- Ubuntu 18.04 with kenel  4.15.0-115
- Quartus 19.4 with Intel HLS enviroment. i.e:
  ```sh
  source intelFPGA_pro/19.4/hls/init_hls.sh
  ```
- Set Board Support Package for FPGA board accelerator in enviroment variable. i.e. for Stratix 10 GX:
  ```sh
	export AOCL_BOARD_PACKAGE_ROOT = intelFPGA_pro/19.4/hld/board/s10_ref
  ```
- xmllint tool dependency
  ```sh
	sudo apt-get install libxml2-utils
  ```
### Test Examples
* Sobel Filter
* Copy data DRAM

### Graph Examples
* Canny Edge
* Census Transfor
* AutoContrast

## How to use

* To start with an example choose one of the test for example Copy, first emulate on CPU 
```bash
  make copy-test-x86-64 
```
* Run and Verify test-x86-64
```bash
  ./test-x86-64 
```
* Generate the RTL files for the OpenVX graph
```bash
  make copy-fpga 
```
* For discrete FPGA connected to a CPU uses the tool extension, be sure you have installed
the Intel FPGA BSP
```bash
  ./create_graph.sh 
```
This tool extension creates a folder outside of the HiFlipVX folder with the name of the OpenVX graph and start the compilation process of a OpenCL library, ready to be used with host OpenCL code. By default the kernel profiling is active.

###### Make graph examples
List of  graph examples in Makefile
* Canny Edge
```bash
  make canny-fpga 
```
* Census Transfor
```bash
  make census-fpga 
```
* AutoContrast
```bash
  make autoc-fpga 
```

### Publications

M. A. D\'avila-GuzmÃ¡n, R. G. Tejero, M. Villarroya-GaudÃ³, D. S. Gracia, L. Kalms and D.
GÃ¶hringer, "A Cross-Platform OpenVX Library for FPGA Accelerators," 2021 29th
Euromicro International Conference on Parallel, Distributed and Network-Based
Processing (PDP), 2021, pp. 75-83, doi: 10.1109/PDP52278.2021.00020.


##### bibtex:

    @INPROCEEDINGS{9407110,
 	 author={DÃ¡vila-GuzmÃ¡n, Maria AngÃ©lica and Tejero, RubÃ©n Gran and Villarroya-	GaudÃ³, MarÃ­a and Gracia, DarÃ­o SuÃ¡rez and Kalms, Lester and GÃ¶hringer, Diana},
	  booktitle={2021 29th Euromicro International Conference on Parallel, Distributed and Network-Based Processing (PDP)}, 
	  title={A Cross-Platform OpenVX Library for FPGA Accelerators}, 
  	year={2021},
  	volume={},
  	number={},
  	pages={75-83},
  	doi={10.1109/PDP52278.2021.00020}}
