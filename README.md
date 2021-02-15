# HiFlipVX for Intel FPGAs

This work is an extension of HiFlipVx Library for Xilinx, a programming library 
based on OpenVX. This extension modifies
the programming model for Intel FPGA Devices using the Intel System of Task.

This library achives the the goal of a portable library between Xilinx and Intel


##Main Changes
* This library uses the Intel System of Task to enable concurrency in the OpenVX graph
* Pragma definitions was adapted to Intel standar. The unrolling was moved befor the for loop
* Makefile added for linux system
* Tool extension

##TEST EXAMPLES
* Sobel Filter
* Copy data DRAM

##GRAPH EXAMPLES
* Canny Edge
* Census Transfor
* AutoContrast

## How to use

* Initialize the enviroment variables for i++ (Intel HLS compiler)
* To start with an example chose one of the test for example Canny Edge, firt emulate on CPU 
 to verify the result
```bash
  make copy-test-x86-64 
```
* Run and Verify test-x86-64
* Generate the RTL files for the OpenVX graph
```bash
  make copy-fpga 
```
* For discrete FPGA connected to a CPU uses the tool extension, be sure you have installed
the Intel FPGA BSP
```bash
  ./create_graph.sh 
```
This tool extension creates a folder outside of the HiFlipVX folder with the name of the OpenVX graph and start the compilation process of a OpenCL library.

## Authors
Intel extension and tool for OpenCL - angelicadg@unizar.es 

