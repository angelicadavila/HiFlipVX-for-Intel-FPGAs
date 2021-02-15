
#ifndef _HiFlipVX_MANUFACTUREFPGA_H_
#define _HiFlipVX_MANUFACTUREFPGA_H_

/** @brief Decides what type of funtion is implemented accordig the manufacturer
@param Xilinx      If is a Xilinx device the input streamming is based on an array of the vx_type
@param Intel	   If is an Intel device Xilinx pointers generates arbitration that generates a lo iniation interval, the interfac should be in this case an streamming type, passed  as direction argument.
*/
#ifdef Xilinx
	#define _vx_in(...) __VA_ARGS__
	#define _vx_out(...) __VA_ARGS__
	#define _vx_vecsize(...) [__VA_ARGS__]//Xilinx needs to define the size of the streamming buffer
#elif Intel
	#define _vx_in(...) __VA_ARGS__&
	//#define _vx_in(...) ::ihc::stream_in<__VA_ARGS__>&
	#define _vx_out(...) __VA_ARGS__&
	//#define _vx_out(...) ::ihc::stream_out<__VA_ARGS__>&
	#define _vx_vecsize(...) //In Intel the pointers are removed
#endif




#endif
