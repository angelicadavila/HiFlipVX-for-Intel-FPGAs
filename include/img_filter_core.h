/**
* Licence: GNU GPLv3 \n
* You may copy, distribute and modify the software as long as you track
* changes/dates in source files. Any modifications to or software
* including (via compiler) GPL-licensed code must also be made available
* under the GPL along with build & install instructions.
*
* @file    img_filter_core.h
* @author  Lester Kalms <lester.kalms@tu-dresden.de>
* @version 1.3
* @brief Description:\n
*  These are the core functions used for the hardware accelerated image processing functions (Do not call functions from here)
*/

#ifndef SRC_IMG_FILTER_CORE_H_
#define SRC_IMG_FILTER_CORE_H_
#include "HLS/hls.h"
#include "vx_fpga.h"
#include "img_helper.h"


using namespace std;

/*********************************************************************************************************************/
/* Linebuffer Functions */
/*********************************************************************************************************************/

/** @brief Reads data from line buffers
@param VecType     1 vector is 1 linebuffer element
@param KERN_SIZE   The size of the filter kernel
@param VEC_COLS    The amount of columns in the vectorized image
@param input       New image data to be stored into the linebuffer
@param linebuffer  Linebuffers to store (KERN_SIZE-1) image rows
@param output      Data at x coordinates in the linebuffers
@param x           The x coordinate in the vectorized image
*/
template <typename InType, typename BufferType, const vx_uint16 BUFFER_NUM, const vx_uint8 VEC_NUM, const vx_uint16 KERN_SIZE, const vx_uint16 VEC_COLS>
void ReadFromLineBuffer(InType input[VEC_NUM], BufferType linebuffer[BUFFER_NUM][VEC_COLS], InType output[KERN_SIZE][VEC_NUM], const vx_uint16 x) {
#ifdef Xilinx
#pragma HLS INLINE
#endif

	// For data type conversion to decrease BRAM usage: FACTOR*BUFFER_NUM = (KERN_SIZE-1)*VEC_NUM
	const vx_uint16 FACTOR = sizeof(BufferType) / sizeof(InType);

	// Buffer for data type conversion
#ifdef Xilinx
	BufferType buffer1[BUFFER_NUM];
        #pragma HLS array_partition variable=buffer1 complete dim=0
#elif Intel
	BufferType buffer1[BUFFER_NUM] hls_register;
#endif

#ifdef Xilinx
	InType buffer2[BUFFER_NUM*FACTOR];
	#pragma HLS array_partition variable=buffer2 complete dim=0
#elif Intel
	InType buffer2[BUFFER_NUM*FACTOR] hls_register;
#endif


	// Check linebuffer border
	if (x < VEC_COLS)
       {
		// Read data from linebuffer
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < BUFFER_NUM; i++) {
UNROLL_XILINX()
			buffer1[i] = linebuffer[i][x];
		}

		// Unpack data
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < BUFFER_NUM; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 j = 0; j < FACTOR; j++) {
UNROLL_XILINX()
				vx_uint16 shift = j * static_cast<vx_uint16>(sizeof(InType)) * static_cast<vx_uint16>(8);
				buffer2[i*FACTOR + j] = static_cast<InType>(buffer1[i] >> shift);
			}
		}

		// Pack data for output
#ifdef Intel
	#pragma loop_coalesce
#endif
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < KERN_SIZE-1; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 j = 0; j < VEC_NUM; j++) {
UNROLL_XILINX()
				output[i][j] = buffer2[i*VEC_NUM + j];
			}
		}
		UNROLL_INTEL()
		for (vx_uint16 j = 0; j < VEC_NUM; j++) {
		UNROLL_XILINX()
			output[KERN_SIZE - 1][j] = input[j];
		}
	}//end if VEC_COLS
}

/** @brief Writes data to line buffers
@param VecType     1 vector is 1 linebuffer element
@param KERN_SIZE   The size of the filter kernel
@param VEC_COLS    The amount of columns in the vectorized image
@param input       Stores data at the x coordinates of the linebuffers
@param linebuffer  Linebuffers to store (KERN_SIZE-1) image rows
@param x           The x coordinate in the vectorized image
*/
template <typename InType, typename BufferType, const vx_uint16 BUFFER_NUM, const vx_uint8 VEC_NUM, const vx_uint16 KERN_SIZE, const vx_uint16 VEC_COLS>
void WriteToLineBuffer(InType input[KERN_SIZE][VEC_NUM], BufferType linebuffer[BUFFER_NUM][VEC_COLS], const vx_uint16 x) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// For data type conversion to decrease BRAM usage: FACTOR*BUFFER_NUM = (KERN_SIZE-1)*VEC_NUM
	const vx_uint16 FACTOR = sizeof(BufferType) / sizeof(InType);

	// Buffer for data type conversion
#ifdef Xilinx
	InType buffer1[(KERN_SIZE - 1)*VEC_NUM];
	#pragma HLS array_partition variable=buffer1 complete dim=0
#elif Intel
        InType buffer1[(KERN_SIZE - 1)*VEC_NUM] hls_register;
#endif

#ifdef Xilinx
	BufferType buffer2[BUFFER_NUM];
	#pragma HLS array_partition variable=buffer2 complete dim=0
#elif Intel
	BufferType buffer2[BUFFER_NUM] hls_register;
#endif
	// Check linebuffer border
	if (x < VEC_COLS) {

		// Unpack data from input

UNROLL_INTEL()
		for (vx_uint16 i = 0; i < KERN_SIZE - 1; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 j = 0; j < VEC_NUM; j++) {
UNROLL_XILINX()
				buffer1[i*VEC_NUM + j] = input[i + 1][j];
			}
		}

		// Pack data for linebuffer
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < BUFFER_NUM; i++) {
UNROLL_XILINX()
			BufferType data = 0;
UNROLL_INTEL()
			for (vx_uint16 j = 0; j < FACTOR; j++) {
UNROLL_XILINX()
				vx_uint16 shift = j * static_cast<vx_uint16>(sizeof(InType)) * static_cast<vx_uint16>(8);
				data |= (static_cast<BufferType>(buffer1[i*FACTOR + j])) << shift;
			}
			buffer2[i] = data;
		}

		// Write to linebuffer
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < BUFFER_NUM; i++) {
UNROLL_XILINX()
			linebuffer[i][x] = buffer2[i];
		}
	}
}

/*********************************************************************************************************************/
/* Sliding Window Functions for Different Border Types */
/*********************************************************************************************************************/

/** @brief Replicates the y borders if needed for the sliding window
@param VecType     1 vector is 1 linebuffer element
@param IMG_ROWS    Amount of rows in the image
@param KERN_RAD    Radius of the filter kernel
@param KERN_SIZE   Total amount of rows in the sliding window
@param input       Output of the linebuffers
@param output      Input with replicated borders if needed
@param y           y coordinate of the image
*/
template <typename InType, const vx_uint8 VEC_NUM, const vx_uint32 IMG_ROWS, const vx_uint16 KERN_RAD, const vx_uint16 KERN_SIZE>
void SlidingWindowReplicatedY(const InType input[KERN_SIZE][VEC_NUM], InType output[KERN_SIZE][VEC_NUM], const vx_uint16 y) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Get upper pixels and check y border
	if (KERN_RAD > 0) {
UNROLL_INTEL()
		for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
			output[KERN_RAD - 1][v] = (y > KERN_RAD) ? (input[KERN_RAD - 1][v]) : (input[KERN_RAD][v]);
		}
		//output[KERN_RAD - 1] = (y > KERN_RAD) ? (input[KERN_RAD - 1]) : (input[KERN_RAD]);
UNROLL_INTEL()
		for (vx_int32 i = KERN_RAD - 2; i >= 0; i--) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				output[i][v] = (y > static_cast<vx_uint16>(KERN_SIZE - 2 - i)) ? (input[i][v]) : (output[i + 1][v]);
			}
		}
	}

	// Pass through observed pixel in the image
UNROLL_INTEL()
	for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
		output[KERN_RAD][v] = input[KERN_RAD][v];
	}

	// Get lower pixels and check y border
	if (KERN_RAD > 0) {
UNROLL_INTEL()
		for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
			output[KERN_RAD + 1][v] = (y < IMG_ROWS + KERN_RAD - 1) ? (input[KERN_RAD + 1][v]) : (input[KERN_RAD][v]);
		}
UNROLL_INTEL()
		for (vx_uint16 i = KERN_RAD + 2; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				output[i][v] = (y < static_cast<vx_uint16>(IMG_ROWS + KERN_SIZE - 1 - i)) ? (input[i][v]) : (output[i - 1][v]);
			}
		}
	}
}

/** @brief Moves sliding window and a replicated border in x direction
@param ScalarType  The data type of the image elements
@param KERN_SIZE   Total amount of rows in the sliding window
@param VEC_COLS    The amount of columns in the vectorized image
@param VEC_SIZE    The number of elements in a vector
@param WIN_BORD_A  Internal vertical border for the sliding window
@param WIN_BORD_B  Internal vertical border for the sliding window
@param WIN_COLS    The number of columns in a row
@param input       An array of input data for each row
@param window      The sliding window
@param x           x coordinate of the vectorized image
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE, const vx_uint16 VEC_COLS, const vx_uint16 VEC_SIZE, const vx_uint16 WIN_BORD_A, const vx_uint16 WIN_BORD_B, const vx_uint16 WIN_COLS>
void SlidingWindowReplicatedX(ScalarType input[KERN_SIZE][VEC_SIZE], ScalarType window[KERN_SIZE][WIN_COLS], const vx_uint16 x) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Move sliding window and check x border
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()

		// Move sliding window
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < WIN_BORD_A; j++) {
UNROLL_XILINX()
			window[i][j] = window[i][j + VEC_SIZE];
		}

		// Move pixel in sliding window and get/check left x border
UNROLL_INTEL()
		for (vx_uint16 j = WIN_BORD_A; j < WIN_BORD_B; j++) {
UNROLL_XILINX()
			if (x == 0)
				window[i][j] = input[i][0];
			else
				window[i][j] = window[i][j + VEC_SIZE];
		}

		// Get new pixel array and check right x border
UNROLL_INTEL()
		for (vx_uint16 j = WIN_BORD_B; j < WIN_COLS; j++) {
UNROLL_XILINX()
			if (x >= VEC_COLS) {
				window[i][j] = window[i][WIN_BORD_B-1];
			} else {
				window[i][j] = input[i][j - WIN_BORD_B];
			}
		}
	}
}

/** @brief Sets a constant border of 0 for the sliding window
@param VecType     1 vector is 1 linebuffer element
@param IMG_ROWS    Amount of rows in the image
@param KERN_RAD    Radius of the filter kernel
@param KERN_SIZE   Total amount of rows in the sliding window
@param input       Output of the linebuffers
@param output      Input with replicated borders if needed
@param y           y coordinate of the image
*/
template <typename InType, const vx_uint8 VEC_NUM, const vx_uint32 IMG_ROWS, const vx_uint16 KERN_RAD, const vx_uint16 KERN_SIZE>
void SlidingWindowConstantY(const InType input[KERN_SIZE][VEC_NUM], InType output[KERN_SIZE][VEC_NUM], const vx_uint16 y) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Get upper pixels and check y border
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_RAD; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
			output[i][v] = (y > KERN_SIZE - 2 - i) ? (input[i][v]) : (0);
		}
	}

	// Pass through observed pixel in the image
UNROLL_INTEL()
	for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
		output[KERN_RAD][v] = input[KERN_RAD][v];
	}

	// Get lower pixels and check y border
UNROLL_INTEL()
	for (vx_uint16 i = KERN_RAD + 1; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
			output[i][v] = (y < IMG_ROWS + KERN_SIZE - 1 - i) ? (input[i][v]) : (0);
		}
	}
}

/** @brief Moves sliding window and a constant border in x direction
@param ScalarType  The data type of the image elements
@param KERN_SIZE   Total amount of rows in the sliding window
@param VEC_COLS    The amount of columns in the vectorized image
@param VEC_SIZE    The number of elements in a vector
@param WIN_BORD_A  Internal vertical border for the sliding window
@param WIN_BORD_B  Internal vertical border for the sliding window
@param WIN_COLS    The number of columns in a row
@param input       An array of input data for each row
@param window      The sliding window
@param x           x coordinate of the vectorized image
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE, const vx_uint16 VEC_COLS, const vx_uint16 VEC_SIZE, const vx_uint16 WIN_BORD_A, const vx_uint16 WIN_BORD_B, const vx_uint16 WIN_COLS>
void SlidingWindowConstantX(ScalarType input[KERN_SIZE][VEC_SIZE], ScalarType window[KERN_SIZE][WIN_COLS], const vx_uint16 x) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()

		// Move sliding window
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < WIN_BORD_A; j++) {
UNROLL_XILINX()
			window[i][j] = window[i][j + VEC_SIZE];
		}

		// Move pixel in sliding window and get/check left x border
UNROLL_INTEL()
		for (vx_uint16 j = WIN_BORD_A; j < WIN_BORD_B; j++) {
UNROLL_XILINX()
			if (x == 0)
				window[i][j] = 0;
			else
				window[i][j] = window[i][j + VEC_SIZE];
		}

		// Get new pixel vector and check right x border
UNROLL_INTEL()
		for (vx_uint16 j = WIN_BORD_B; j < WIN_COLS; j++) {
UNROLL_XILINX()
			if (x >= VEC_COLS)
				window[i][j] = 0;
			else
				window[i][j] = input[i][j - WIN_BORD_B];
		}
	}
}

/** @brief Moves sliding window without considering borders
@param ScalarType  The data type of the image elements
@param KERN_SIZE   Total amount of rows in the sliding window
@param VEC_SIZE    The number of elements in a vector
@param WIN_BORD_B  Internal vertical border for the sliding window
@param WIN_COLS    The number of columns in a row
@param input       An array of input data for each row
@param window      The sliding window
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE, const vx_uint16 VEC_SIZE, const vx_uint16 WIN_BORD_B, const vx_uint16 WIN_COLS>
void SlidingWindowUnchanged(const ScalarType input[KERN_SIZE][VEC_SIZE], ScalarType window[KERN_SIZE][WIN_COLS]) {

#ifdef Xilinx
#pragma HLS INLINE
#endif

UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()

		// Move sliding window
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < WIN_BORD_B; j++) {
UNROLL_XILINX()
			window[i][j] = window[i][j + VEC_SIZE];
		}

		// Get new pixel vector
UNROLL_INTEL()
		for (vx_uint16 j = WIN_BORD_B; j < WIN_COLS; j++) {
UNROLL_XILINX()
			window[i][j] = input[i][j - WIN_BORD_B];
		}
	}
}

/*********************************************************************************************************************/
/* Main sliding Window Functions */
/*********************************************************************************************************************/

/** @brief Sliding window horizontal main function / considers different border types
@param VecType     1 vector is processed in each clock cycle
@param ScalarType  1 vector contains N scalar elements
@param KERN_RAD    Radius of the filter kernel
@param VEC_COLS    The amount of columns in the vectorized image
@param VEC_SIZE    The number of elements in a vector
@param WIN_COLS    The number of columns in a row
@param BORDER_TYPE The border type that is considered in the sliding window
@param input       The input buffer
@param window      The output sliding window
@param x           The x current coordinate
*/
template <typename ScalarType, const vx_uint16 KERN_RAD, const vx_uint16 VEC_COLS, const vx_uint16 VEC_SIZE, const vx_uint16 WIN_COLS, const vx_border_e BORDER_TYPE>
void SlidingWindowHorizontal(const ScalarType input[VEC_SIZE], ScalarType window[1][WIN_COLS], const vx_uint16 x) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Constants
	const vx_uint16 WIN_BORD_A = WIN_COLS - VEC_SIZE - KERN_RAD;
	const vx_uint16 WIN_BORD_B = WIN_COLS - VEC_SIZE;

	// Input data rows in scalar representation (after considering y border)
	#ifdef Xilinx
	ScalarType input_vector[1][VEC_SIZE];
	#pragma HLS array_partition variable=input_vector complete  dim=0
        #elif Intel
	ScalarType input_vector[1][VEC_SIZE]hls_register;
	#endif

UNROLL_INTEL()
	for (vx_uint16 i = 0; i < VEC_SIZE; i++) {
UNROLL_XILINX()
		input_vector[0][i] = input[i];
	}

	// REPLICATED: replicates the border values when exceeding borders
	if (BORDER_TYPE == VX_BORDER_REPLICATE) {

		// Sets sliding window and replicated x borders
		SlidingWindowReplicatedX<ScalarType, 1, VEC_COLS, VEC_SIZE, WIN_BORD_A, WIN_BORD_B, WIN_COLS>(input_vector, window, x);

	// CONSTANT: creates a constant border of zeros around the image
	} else if (BORDER_TYPE == VX_BORDER_CONSTANT) {

		// Sets sliding window and constant x borders
		SlidingWindowConstantX<ScalarType, 1, VEC_COLS, VEC_SIZE, WIN_BORD_A, WIN_BORD_B, WIN_COLS>(input_vector, window, x);

	// UNCHANGED: filters exceeding the borders are invalid
	} else if (BORDER_TYPE == VX_BORDER_UNDEFINED) {

		// Sets sliding window and does not create borders
		SlidingWindowUnchanged<ScalarType, 1, VEC_SIZE, WIN_BORD_B, WIN_COLS>(input_vector, window);
	}
}

/** @brief Sliding window main function / considers different border types
@param VecType     1 vector is processed in each clock cycle
@param ScalarType  1 vector contains N scalar elements
@param IMG_ROWS    Amount of rows in the image
@param KERN_RAD    Radius of the filter kernel
@param VEC_SIZE    The number of elements in a vector
@param KERN_SIZE   Total amount of rows in the sliding window
@param BORDER_TYPE The border type that is considered in the sliding window
@param input       The input buffer
@param window      The output sliding window
@param y           The y current coordinate
*/
template <typename ScalarType, const vx_uint32 IMG_ROWS, const vx_uint16 KERN_RAD, const vx_uint16 VEC_SIZE, const vx_uint16 KERN_SIZE, const vx_border_e BORDER_TYPE>
void SlidingWindowVertical(const ScalarType input[KERN_SIZE][VEC_SIZE], ScalarType window[KERN_SIZE][VEC_SIZE], const vx_uint16 y) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Input data rows in vector representation (after considering y border)
#ifdef Xilinx
	ScalarType buffer_vector[KERN_SIZE][VEC_SIZE];
	#pragma HLS array_partition variable=buffer_vector complete dim=0
#elif Intel
	ScalarType buffer_vector[KERN_SIZE][VEC_SIZE] hls_register;
#endif
	// REPLICATED: replicates the border values when exceeding borders
	if (BORDER_TYPE == VX_BORDER_REPLICATE) {

		// Replicated y borders
		SlidingWindowReplicatedY<ScalarType, VEC_SIZE, IMG_ROWS, KERN_RAD, KERN_SIZE>(input, buffer_vector, y);

	// CONSTANT: creates a constant border of zeros around the image
	} else if (BORDER_TYPE == VX_BORDER_CONSTANT) {

		// Constant y borders
		SlidingWindowConstantY<ScalarType, VEC_SIZE, IMG_ROWS, KERN_RAD, KERN_SIZE>(input, buffer_vector, y);
	}

	// Convert from vector to scalar type
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < VEC_SIZE; j++) {
UNROLL_XILINX()
			if(BORDER_TYPE == VX_BORDER_REPLICATE || BORDER_TYPE == VX_BORDER_CONSTANT)
				window[i][j] = buffer_vector[i][j];
			else if(BORDER_TYPE == VX_BORDER_UNDEFINED)
				window[i][j] = input[i][j];
		}
	}
}

/** @brief Sliding window main function / considers different border types
@param VecType     1 vector is processed in each clock cycle
@param ScalarType  1 vector contains N scalar elements
@param IMG_ROWS    Amount of rows in the image
@param KERN_RAD    Radius of the filter kernel
@param VEC_COLS    The amount of columns in the vectorized image
@param VEC_SIZE    The number of elements in a vector
@param WIN_COLS    The number of columns in a row
@param KERN_SIZE   Total amount of rows in the sliding window
@param BORDER_TYPE The border type that is considered in the sliding window
@param input       The input buffer
@param window      The output sliding window
@param x           The x current coordinate
@param y           The y current coordinate
*/
template <typename InType, const vx_uint8 VEC_NUM, const vx_uint32 IMG_ROWS, const vx_uint16 KERN_RAD, const vx_uint16 VEC_COLS, const vx_uint16 WIN_COLS, const vx_uint16 KERN_SIZE, const vx_border_e BORDER_TYPE>
void SlidingWindow(const InType input[KERN_SIZE][VEC_NUM], InType window[KERN_SIZE][WIN_COLS], const vx_uint16 x, const vx_uint16 y) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Constants
	const vx_uint16 WIN_BORD_A = WIN_COLS - VEC_NUM - KERN_RAD;
	const vx_uint16 WIN_BORD_B = WIN_COLS - VEC_NUM;

	// Input data rows in vector representation (after considering y border)
	#ifdef Xilinx
	InType buffer_vector[KERN_SIZE][VEC_NUM];
	#pragma HLS array_partition variable=buffer_vector complete dim=0
	#elif Intel
	InType buffer_vector[KERN_SIZE][VEC_NUM] hls_register;
	#endif


	// REPLICATED: replicates the border values when exceeding borders
	if (BORDER_TYPE == VX_BORDER_REPLICATE) {

		// Replicated y borders
		SlidingWindowReplicatedY<InType, VEC_NUM, IMG_ROWS, KERN_RAD, KERN_SIZE>(input, buffer_vector, y);

		// Sets sliding window and replicated x borders
		SlidingWindowReplicatedX<InType, KERN_SIZE, VEC_COLS, VEC_NUM, WIN_BORD_A, WIN_BORD_B, WIN_COLS>(buffer_vector, window, x);

	// CONSTANT: creates a constant border of zeros around the image
	} else if (BORDER_TYPE == VX_BORDER_CONSTANT) {

		// Constant y borders
		SlidingWindowConstantY<InType, VEC_NUM, IMG_ROWS, KERN_RAD, KERN_SIZE>(input, buffer_vector, y);

		// Sets sliding window and constant x borders
		SlidingWindowConstantX<InType, KERN_SIZE, VEC_COLS, VEC_NUM, WIN_BORD_A, WIN_BORD_B, WIN_COLS>(buffer_vector, window, x);

	// UNCHANGED: filters exceeding the borders are invalid
	} else if (BORDER_TYPE == VX_BORDER_UNDEFINED) {

		// Sets sliding window and does not create borders
		SlidingWindowUnchanged<InType, KERN_SIZE, VEC_NUM, WIN_BORD_B, WIN_COLS>(input, window);
	}
}

/*********************************************************************************************************************/
/* Filter Sub-Functions */
/*********************************************************************************************************************/

/** @brief Compares and swaps 2 elements
@param InType  Data type of the image pixels
@param A       1. element to be compared
@param B       2. element to be compared
@param H       smaller element
@param L       bigger element
*/
template <typename InType>
void CompareAndSwap(InType A, InType B, InType &L, InType &H) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Compare and Swap
	if (A > B) {
		L = B;
		H = A;
	} else {
		L = A;
		H = B;
	}
}

/*********************************************************************************************************************/
/* Filter Functions */
/*********************************************************************************************************************/

/** @brief Computes a 1d box filter
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the box filter
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE>
ScalarType ComputeBox1d(ScalarType window[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Variable
	vx_uint64 result = 0;

	// Sum all input data
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
		result += static_cast<vx_uint64>(window[i]);
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<ScalarType>(norm);
}

/** @brief Computes a 2d box filter
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the box filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeBox2d(InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Variable
	vx_uint64 result = 0;

	// Sum all input data
UNROLL_INTEL()
	for (vx_uint16 y = 0; y < KERN_SIZE; y++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 x = 0; x < KERN_SIZE; x++) {
UNROLL_XILINX()
			result += static_cast<vx_uint64>(window[y][x]);
		}
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<OutType>(norm);
}

/** @brief Computes a 2d convolution filter
@param InType        The input scalar data type
@param CompType      Data type for internal computation (uint64 or int64)
@param OutType       The output scalar data type
@param KernType      The kernel scalar data type
@param KERN_SIZE     The size of the kernel
@param inKernel      The costum kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the convolution filter
*/
template <typename InType, typename CompType, typename OutType, typename KernType, const vx_uint16 KERN_SIZE>
OutType ComputeConvolve2d(KernType kernel[KERN_SIZE][KERN_SIZE], InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Variables
	CompType result = 0;

	// Compute the costum filter
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < KERN_SIZE; j++) {
UNROLL_XILINX()
			CompType kernelData = static_cast<CompType>(kernel[i][j]);
			CompType windowData = static_cast<CompType>(window[i][j]);
			result += (kernelData * windowData);
		}
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<OutType>(norm);
}

/** @brief Computes a 1d dilate filter
@param ScalarType    The Scalar Data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@return              The result of the dilate filter
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE>
ScalarType ComputeDilate1d(ScalarType window[KERN_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Compute dilate
	ScalarType result = window[0];
UNROLL_INTEL()
	for (vx_uint16 i = 1; i < KERN_SIZE; i++) {
UNROLL_XILINX()
		result = max(result, window[i]);
	}

	// Return result
	return result;
}

/** @brief Computes a 1d erode filter
@param ScalarType    The Scalar Data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@return              The result of the erode filter
*/
template <typename ScalarType, const vx_uint16 KERN_SIZE>
ScalarType ComputeErode1d(ScalarType window[KERN_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Compute erode
	ScalarType result = window[0];
UNROLL_INTEL()
	for (vx_uint16 i = 1; i < KERN_SIZE; i++) {
UNROLL_XILINX()
		result = min(result, window[i]);
	}

	// Return result
	return result;
}

/** @brief Computes a gaussian 1d filter (optimized to the kernel symmetry)
@param ScalarType    The input/output data type
@param KernType      The kernel scalar data type
@param KERN_SIZE     The size of the kernel
@param kernel        The gaussian kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the gaussian convolution
*/
template <typename ScalarType, typename KernelType, const vx_uint16 KERN_SIZE>
ScalarType ComputeGaussian1d(KernelType kernel[KERN_SIZE], ScalarType window[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Constants
	const vx_uint16 KERN_RAD = KERN_SIZE >> 1;

	// Get middle pixel result
	vx_uint64 result = static_cast<vx_uint64>(kernel[KERN_RAD]) * static_cast<vx_uint64>(window[KERN_RAD]);

	// Add all other pixel to result
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < KERN_RAD; i++) {
UNROLL_XILINX()
		vx_uint64 A = static_cast<vx_uint64>(kernel[i]);
		vx_uint64 B0 = static_cast<vx_uint64>(window[i]);
		vx_uint64 B1 = static_cast<vx_uint64>(window[KERN_SIZE - i - 1]);
		result += A * (B0 + B1);
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<ScalarType>(norm);
}

/** @brief Computes a gaussian 2d filter (optimized to the kernel symmetry)
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KernType      The kernel scalar data type
@param KERN_SIZE     The size of the kernel
@param kernel        The gaussian kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the gaussian convolution
*/
template <typename InType, typename OutType, typename KernType, const vx_uint16 KERN_SIZE>
OutType ComputeGaussian2d(KernType kernel[KERN_SIZE][KERN_SIZE], InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
#endif

	// Constants
	const vx_uint16 KERN_RAD = KERN_SIZE / 2;
	const vx_uint16 KERN_RNG = KERN_SIZE - 1;

	// Store result of the gaussian filter (scalar)
	vx_uint64 result = 0;

	// if: (y == KERN_RAD) && (x == KERN_RAD)
	{
		vx_uint64 sum = static_cast<vx_uint64>(window[KERN_RAD][KERN_RAD]);
		result = (static_cast<vx_uint64>(kernel[KERN_RAD][KERN_RAD]) * sum);
	}

	// if: (y == KERN_RAD) && (x < KERN_RAD)
UNROLL_INTEL()
	for (vx_uint16 x = 0; x < KERN_RAD; x++) {
UNROLL_XILINX()
		const vx_uint16 RADIUS = KERN_RAD - x;
		vx_uint64 sum = 0;
		sum += static_cast<vx_uint64>(window[KERN_RAD][x]);
		sum += static_cast<vx_uint64>(window[KERN_RAD][KERN_RNG - x]);
		sum += static_cast<vx_uint64>(window[KERN_RAD + RADIUS][x + RADIUS]);
		sum += static_cast<vx_uint64>(window[KERN_RAD - RADIUS][x + RADIUS]);
		result += (static_cast<vx_uint64>(kernel[KERN_RAD][x]) * sum);
	}

	// if: (y == x) && (x < KERN_RAD)
UNROLL_INTEL()
	for (vx_uint16 y = 0; y < KERN_RAD; y++) {
UNROLL_XILINX()
		vx_uint64 sum = 0;
		sum += static_cast<vx_uint64>(window[y][y]);
		sum += static_cast<vx_uint64>(window[y][KERN_RNG - y]);
		sum += static_cast<vx_uint64>(window[KERN_RNG - y][y]);
		sum += static_cast<vx_uint64>(window[KERN_RNG - y][KERN_RNG - y]);
		result += (static_cast<vx_uint64>(kernel[y][y]) * sum);
	}

	// if: (y > 0) && (y < KERN_RAD) && (y > x)
UNROLL_INTEL()
	for (vx_uint16 x = 0; x <= KERN_RAD; x++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 y = x + 1; y < KERN_RAD; y++) {
UNROLL_XILINX()
			vx_int64 sum = 0;
			sum += static_cast<vx_uint64>(window[y][x]);
			sum += static_cast<vx_uint64>(window[x][y]);
			sum += static_cast<vx_uint64>(window[KERN_RNG - y][x]);
			sum += static_cast<vx_uint64>(window[x][KERN_RNG - y]);
			sum += static_cast<vx_uint64>(window[KERN_RNG - x][y]);
			sum += static_cast<vx_uint64>(window[y][KERN_RNG - x]);
			sum += static_cast<vx_uint64>(window[KERN_RNG - x][KERN_RNG - y]);
			sum += static_cast<vx_uint64>(window[KERN_RNG - y][KERN_RNG - x]);
			result += (static_cast<vx_uint64>(kernel[y][x]) * sum);
		}
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<OutType>(norm);
}

/** @brief Computes a median filter
@details Algorithm taken from the the following paper:
A fault-tolerant implementation of the median filter
from L. A. Aranda and P. Reviriego and J. A. Maestro
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@return              The result of the median filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeMedian3x3(InType window[KERN_SIZE][KERN_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Variables
	InType B0, B1, B3, B4, B6, B7;
	InType C1, C2, C4, C5, C7, C8;
	InType D0, D1, D3, D4, D6, D7;
	InType E0, E1, E3, E4, E7, E8;
	InType F1, F2, F4, F5, F6, F7;
	InType G3, G4, H3, H4, I4, I5, J3, J4;

	// Input pixel
	InType A0 = window[0][0], A1 = window[0][1], A2 = window[0][2];
	InType A3 = window[1][0], A4 = window[1][1], A5 = window[1][2];
	InType A6 = window[2][0], A7 = window[2][1], A8 = window[2][2];

	// Get median with 19 compare and swap units
	CompareAndSwap(A0, A1, B0, B1);
	CompareAndSwap(A3, A4, B3, B4);
	CompareAndSwap(A6, A7, B6, B7);
	CompareAndSwap(B1, A2, C1, C2);
	CompareAndSwap(B4, A5, C4, C5);
	CompareAndSwap(B7, A8, C7, C8);
	CompareAndSwap(B0, C1, D0, D1);
	CompareAndSwap(B3, C4, D3, D4);
	CompareAndSwap(B6, C7, D6, D7);
	CompareAndSwap(D0, D3, E0, E1);
	CompareAndSwap(D1, D4, E3, E4);
	CompareAndSwap(C5, C8, E7, E8);
	CompareAndSwap(E1, D6, F1, F2);
	CompareAndSwap(E4, D7, F4, F5);
	CompareAndSwap(C2, E7, F6, F7);
	CompareAndSwap(E3, F4, G3, G4);
	CompareAndSwap(F2, G4, H3, H4);
	CompareAndSwap(H4, F6, I4, I5);
	CompareAndSwap(H3, I4, J3, J4);

	// Take median value
	return static_cast<OutType>(J4);
}

/** @brief Computes a median filter
@details Algorithm taken from the the following book:
Knuth in The Art of Computer Programming, vol 3 (algorithm 5.2.2M)
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@return              The result of the median filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeMedian(InType window[KERN_SIZE][KERN_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Constants and variables
	const vx_uint16 BUFFER_SIZE = KERN_SIZE * KERN_SIZE;
	const vx_uint16 BUFFER_RAD = BUFFER_SIZE >> 1;
	vx_uint16 START = 1;
	vx_uint16 row_ptr = 0;
	vx_uint16 SCALE = BUFFER_SIZE - 1;
	vx_uint16 MSB = 0;

	// Register stages for pipelining of the sorting
	#ifdef Xilinx
	InType buffer[BUFFER_SIZE + 1][BUFFER_SIZE];
	#pragma HLS array_partition variable=buffer complete dim=0
	#elif Intel
	InType buffer[BUFFER_SIZE + 1][BUFFER_SIZE] hls_register;
	#endif

	// Pre-computation
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < sizeof(vx_uint16) * 8; i++) {
UNROLL_XILINX()
		if ((SCALE & static_cast<vx_uint16>(1 << i)) != 0)
			MSB = i;
	}
UNROLL_INTEL()
	for (vx_uint16 i = 0; i < MSB; i++) {
UNROLL_XILINX()
		START *= 2;
	}

	// 2d to 1d input window conversion
UNROLL_INTEL()
	for (vx_uint16 i = 0, ptr = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 j = 0; j < KERN_SIZE; j++) {
UNROLL_XILINX()
			buffer[0][ptr] = window[i][j];
			ptr++;
		}
	}

	// Sort array using odd-even mergesort
UNROLL_INTEL()
	for (vx_uint16 p = START; p > 0; p >>= 1) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 q = START, r = 0, d = p; d > 0; d = q - p, q >>= 1, r = p) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint16 i = 0; i < BUFFER_SIZE; i++) {
UNROLL_XILINX()
				buffer[row_ptr + 1][i] = buffer[row_ptr][i];
			}
UNROLL_INTEL()
			for (vx_uint16 i = 0; i < BUFFER_SIZE - d; i++) {
UNROLL_XILINX()
				if ((i & p) == r)
					CompareAndSwap<InType>(buffer[row_ptr][i], buffer[row_ptr][i + d], buffer[row_ptr + 1][i], buffer[row_ptr + 1][i + d]);
			}
			row_ptr++;
		}
	}

	// Take median value
	return static_cast<OutType>(buffer[row_ptr][BUFFER_RAD]);
}

/** @brief Computes the x derivative (optimized for kernel symmetry)
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KernType      The kernel scalar data type
@param KERN_SIZE     The size of the kernel
@param kernel        The sobel/scharr kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the x derivative computation
*/
template <typename InType, typename OutType, typename KernType, const vx_uint16 KERN_SIZE>
OutType ComputeDerivativeX(KernType kernel[KERN_SIZE][KERN_SIZE], InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Constants
	const vx_uint16 KERN_RAD = KERN_SIZE >> 1;
	const vx_uint16 KERN_RANGE = KERN_SIZE - 1;

	// Compute the filter result
	vx_int64 result = 0;

UNROLL_INTEL()
	for (vx_uint16 y = 0; y < KERN_RAD + 1; y++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 x = 0; x < KERN_RAD; x++) {
UNROLL_XILINX()

			// Constants
			const vx_uint16 yn = y + KERN_RANGE - y * 2;
			const vx_uint16 xn = x + KERN_RANGE - x * 2;

			// Compute mul
			vx_int64 mul = abs(static_cast<vx_int64>(kernel[y][x]));

			// compute sum
			vx_int64 sum;
			if (y < KERN_RAD) {
				sum = static_cast<vx_int64>(window[y][xn]) +
					static_cast<vx_int64>(window[yn][xn]) -
					static_cast<vx_int64>(window[y][x]) -
					static_cast<vx_int64>(window[yn][x]);
			} else {
				sum = static_cast<vx_int64>(window[y][xn]) -
					static_cast<vx_int64>(window[y][x]);
			}

			// Add to result
			result += sum*mul;
		}
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<OutType>(norm);
}

/** @brief Computes the y derivative (optimized for kernel symmetry)
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KernType      The kernel scalar data type
@param KERN_SIZE     The size of the kernel
@param kernel        The sobel/scharr kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the y derivative computation
*/
template <typename InType, typename OutType, typename KernType, const vx_uint16 KERN_SIZE>
OutType ComputeDerivativeY(KernType kernel[KERN_SIZE][KERN_SIZE], InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Constants
	const vx_uint16 KERN_RAD = KERN_SIZE >> 1;
	const vx_uint16 KERN_RANGE = KERN_SIZE - 1;

	// Compute the filter result
	vx_int64 result = 0;

UNROLL_INTEL()
	for (vx_uint16 x = 0; x < KERN_RAD + 1; x++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 y = 0; y < KERN_RAD; y++) {
UNROLL_XILINX()
			// Constants
			const vx_uint16 yn = y + KERN_RANGE - y * 2;
			const vx_uint16 xn = x + KERN_RANGE - x * 2;

			// Compute mul
			vx_int64 mul = abs(static_cast<vx_int64>(kernel[y][x]));

			// compute sum
			vx_int64 sum;
			if (x < KERN_RAD) {
				sum = static_cast<vx_int64>(window[yn][x]) +
					static_cast<vx_int64>(window[yn][xn]) -
					static_cast<vx_int64>(window[y][x]) -
					static_cast<vx_int64>(window[y][xn]);
			} else {
				sum = static_cast<vx_int64>(window[yn][x]) -
					static_cast<vx_int64>(window[y][x]);
			}

			// Add to result
			result += sum*mul;
		}
	}

	// Normalize
	const vx_uint64 rounding = static_cast<vx_uint64>(1) << (kernel_shift - 1);
	const vx_uint64 norm = (result * kernel_mult + rounding) >> kernel_shift;

	// Return result
	return static_cast<OutType>(norm);
}

/*********************************************************************************************************************/
/* Filter Main Function */
/*********************************************************************************************************************/

/** @brief Selects and computes a 1d horizontal filter
@param ScalarType   Data type of the input/output image
@param KernelType   Data type of the kernel
@param KERN_SIZE    Filter kernel size
@param WIN_COLS     Columns of the sliding window
@param VEC_NUM      Vector size
@param window       The sliding window
@param kernel       The convolution kernels
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
@param output       The results (vector) (per clock cycle)
*/
template <typename ScalarType, typename KernelType, vx_uint16 KERN_SIZE, vx_uint16 WIN_COLS, vx_uint16 VEC_NUM>
void ComputeFilterHorizontal(ScalarType window[WIN_COLS], KernelType kernel[KERN_SIZE],
	const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type, ScalarType output[VEC_NUM]) {
#ifdef Xilinx
#pragma HLS INLINE
#endif

UNROLL_INTEL()
	for (vx_uint16 vecId = 0; vecId < VEC_NUM; vecId++) {
UNROLL_XILINX()

		// Window for single vector element
		#ifdef Xilinx
		ScalarType kernel_window[KERN_SIZE];
		#pragma HLS array_partition variable=kernel_window complete dim=0
		#elif Intel
		ScalarType kernel_window[KERN_SIZE] hls_register;
		#endif

		// Get window for single vector element
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
			kernel_window[i] = window[i + vecId];
		}

		// Compute filter for single vector element
		ScalarType result = 0;
		switch (kernel_type) {
		case HIFLIPVX::GAUSSIAN_FILTER:
			result = ComputeGaussian1d<ScalarType, KernelType, KERN_SIZE>(kernel, kernel_window, kernel_mult, kernel_shift);
			break;
		case HIFLIPVX::BOX_FILTER:
			result = ComputeBox1d<ScalarType, KERN_SIZE>(kernel_window, kernel_mult, kernel_shift);
			break;
		case HIFLIPVX::ERODE_IMAGE:
			result = ComputeErode1d<ScalarType, KERN_SIZE>(kernel_window);
			break;
		case HIFLIPVX::DILATE_IMAGE:
			result = ComputeDilate1d<ScalarType, KERN_SIZE>(kernel_window);
			break;
		default:
			break;
		}
		output[vecId] = result;
	}
}

/** @brief Selects and computes a 1d vertical filter
@param ScalarType   Data type of the input/output image
@param KernelType   Data type of the kernel
@param KERN_SIZE    Filter kernel size
@param VEC_NUM      Vector size
@param window       The sliding window
@param kernel       The convolution kernels
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
@param output       The results (vector) (per clock cycle)
*/
template <typename ScalarType, typename KernelType, vx_uint16 KERN_SIZE, vx_uint16 VEC_NUM>
void ComputeFilterVertical(ScalarType window[KERN_SIZE][VEC_NUM], KernelType kernel[KERN_SIZE],
	const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type, vx_image<ScalarType, VEC_NUM> &output) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Output array of the different filter (scalar)
	vx_image<ScalarType, VEC_NUM> output_array;

UNROLL_INTEL()
	for (vx_uint16 vecId = 0; vecId < VEC_NUM; vecId++) {
UNROLL_XILINX()

		// Window for single vector element
		#ifdef Xilinx
		ScalarType kernel_window[KERN_SIZE];
		#pragma HLS array_partition variable=kernel_window complete dim=0
		#elif Intel
		ScalarType kernel_window[KERN_SIZE] hls_register;
		#endif

		// Get window for single vector element
UNROLL_INTEL()
		for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
			kernel_window[i] = window[i][vecId];
		}

		// Compute filter for single vector element
		ScalarType result = 0;
		switch (kernel_type) {
		case HIFLIPVX::GAUSSIAN_FILTER:
			result = ComputeGaussian1d<ScalarType, KernelType, KERN_SIZE>(kernel, kernel_window, kernel_mult, kernel_shift);
			break;
		case HIFLIPVX::BOX_FILTER:
			result = ComputeBox1d<ScalarType, KERN_SIZE>(kernel_window, kernel_mult, kernel_shift);
			break;
		case HIFLIPVX::ERODE_IMAGE:
			result = ComputeErode1d<ScalarType, KERN_SIZE>(kernel_window);
			break;
		case HIFLIPVX::DILATE_IMAGE:
			result = ComputeDilate1d<ScalarType, KERN_SIZE>(kernel_window);
			break;
		default:
			break;
		}
		output_array.pixel[vecId] = result;
	}

	output = output_array;
}


/** @brief Computes a Non-Max suppression 3x3  filter
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the box filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeNonMaxSuppression(InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Input pixel
	InType A0 = window[0][0], A1 = window[0][1], A2 = window[0][2];
	InType A3 = window[1][0], A4 = window[1][1], A5 = window[1][2];
	InType A6 = window[2][0], A7 = window[2][1], A8 = window[2][2];

       	bool O0=0;

	if (A4 < A0) //p(x,y)>=p(x-1,y1)
		O0 = 1; //suppress
	if (A4 < A1)
		O0 = 1; //suppress
	if (A4 < A2)
		O0 = 1; //suppress
	if (A4 < A3)
		O0 = 1; //suppress
	if (A4 < A5)
		O0 = 1; //suppress
	if (A4 < A6)
		O0 = 1; //suppress
	if (A4 < A7)
		O0 = 1; //suppress
	if (A4 < A8)
		O0 = 1; //suppress

	if (O0) A4=0;	
	/*Threashold.*/
    if (A4>8) A4=127;
	else A4=0;
	return static_cast<OutType>(A4);
}


/** @brief Computes a Non-Max suppression 3x3  filter with pahse data
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the box filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeNonMaxSuppressionPhase(InType window[KERN_SIZE][KERN_SIZE], 
		InType phase, const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Input pixel
	InType A0 = window[0][0], A1 = window[0][1], A2 = window[0][2];
	InType A3 = window[1][0], A4 = window[1][1], A5 = window[1][2];
	InType A6 = window[2][0], A7 = window[2][1], A8 = window[2][2];

       	bool O0=0;
    //Negative diagnoal
    if ( phase == 3 ){
		if (A4 < A0) //p(x,y)>=p(x-1,y1)
			O0 = 1; //suppress
		if (A4 < A8)
			O0 = 1; //suppress
	}
	//Horizontal
    if ( phase == 1 ){
		if (A4 < A3)
			O0 = 1; //suppress
		if (A4 < A5)
			O0 = 1; //suppress
	}
	//Vertical
    if ( phase == 0 ){
		if (A4 < A7)
			O0 = 1; //suppress
		if (A4 < A1)
			O0 = 1; //suppress
	}
	//Positive Diagonal
    if ( phase == 2 ){
		if (A4 < A2)
			O0 = 1; //suppress
		if (A4 < A6)
			O0 = 1; //suppress
	}

	if (O0) A4=0;	
	/*Threashold.*/
    if (A4>8) A4=127;
	else A4=0;
	return static_cast<OutType>(A4);
}


/** @brief Computes a Census tranform 3x3  filter
@param InType        The input scalar data type
@param OutType       The output scalar data type
@param KERN_SIZE     The size of the kernel
@param window        The sliding window of this scalar computation
@param kernel_mult   Multiplier for normalization
@param kernel_shift  Shifter For normalization
@return              The result of the box filter
*/
template <typename InType, typename OutType, const vx_uint16 KERN_SIZE>
OutType ComputeCensusTransform(InType window[KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift) {
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Input pixel
	InType A0 = window[0][0], A1 = window[0][1], A2 = window[0][2];
	InType A3 = window[1][0], A4 = window[1][1], A5 = window[1][2];
	InType A6 = window[2][0], A7 = window[2][1], A8 = window[2][2];
    
    OutType bin_value = 0x00;

	if (A4 > A0) //p(x,y)>=p(x-1,y1)
		bin_value |= 0x80;
	if (A4 > A1)
		bin_value |= 0x40;
	if (A4 > A2)
		bin_value |= 0x20;
	if (A4 > A3)
		bin_value |= 0x10;
	if (A4 > A5)
		bin_value |= 0x08;
	if (A4 > A6)
		bin_value |= 0x04;
	if (A4 > A7)
		bin_value |= 0x02;
	if (A4 > A8)
		bin_value |= 0x01;

	return static_cast<OutType>(bin_value);
}



/** @brief Selects and computes a filter
@param InType       Data type of the input image
@param OutType      Data type of the output image
@param KernType     Data type of the kernel
@param VEC_NUM     Vector size
@param KERN_NUM     Amount of filter kernel
@param KERN_SIZE    Filter kernel size
@param WIN_COLS     Columns of the sliding window
@param window       The sliding window
@param kernel       The convolution kernels
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
@param output       The results (vector) (per clock cycle)
*/
template <typename InType, typename OutType, typename KernType, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 KERN_SIZE, vx_uint16 WIN_COLS>
void ComputeFilter(InType window[KERN_SIZE][WIN_COLS], KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE],
	const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM], vx_image_t<OutType, VEC_NUM> output[KERN_NUM]) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	vx_image_t<OutType, VEC_NUM> output_vector[KERN_NUM] hls_register;

	// Compute KERN_NUM different filter with VEC_NUM elements per clock cycle
UNROLL_INTEL()
	for (vx_uint16 kernId = 0; kernId < KERN_NUM; kernId++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 vecId = 0; vecId < VEC_NUM; vecId++) {
UNROLL_XILINX()

			// Window for single vector element
#ifdef Xilinx
			InType kernel_window[KERN_SIZE][KERN_SIZE];
			#pragma HLS array_partition variable=kernel_window complete dim=0
#elif Intel
			InType kernel_window[KERN_SIZE][KERN_SIZE] hls_register;
#endif

			// Get window for single vector element
UNROLL_INTEL()
			for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
				for (vx_uint16 j = 0; j < KERN_SIZE; j++) {
UNROLL_XILINX()
					kernel_window[i][j] = window[i][j + vecId];
				}
			}

			// Check, if input or output is signed
			bool isSigned = (numeric_limits<InType>::is_signed) || (numeric_limits<OutType>::is_signed);

			// Compute filter for single vector element
			OutType result = 0;
			switch (kernel_type[kernId]) {
			case HIFLIPVX::GAUSSIAN_FILTER:
				result = ComputeGaussian2d<InType, OutType, KernType, KERN_SIZE>(kernel[kernId], kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::DERIVATIVE_X:
				result = ComputeDerivativeX<InType, OutType, KernType, KERN_SIZE>(kernel[kernId], kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::DERIVATIVE_Y:
				result = ComputeDerivativeY<InType, OutType, KernType, KERN_SIZE>(kernel[kernId], kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::CUSTOM_CONVOLUTION:
				if (isSigned)
					result = ComputeConvolve2d<InType, vx_int64, OutType, KernType, KERN_SIZE>(kernel[kernId], kernel_window, kernel_mult, kernel_shift);
				else
					result = ComputeConvolve2d<InType, vx_uint64, OutType, KernType, KERN_SIZE>(kernel[kernId], kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::BOX_FILTER:
				result = ComputeBox2d<InType, OutType, KERN_SIZE>(kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::MEDIAN_FILTER:
				if (KERN_SIZE == 3)
					result = ComputeMedian3x3<InType, OutType, KERN_SIZE>(kernel_window);
				else
					result = ComputeMedian<InType, OutType, KERN_SIZE>(kernel_window);
				break;
			case HIFLIPVX::NON_MAX_SUPPRESSION:
				result = ComputeNonMaxSuppression<InType, OutType, KERN_SIZE>(kernel_window, kernel_mult, kernel_shift);
				break;
			case HIFLIPVX::CENSUS_TRANSFORM:
				result = ComputeCensusTransform<InType, OutType, KERN_SIZE>(kernel_window, kernel_mult, kernel_shift);
				break;
			default:
				break;
			}

			output_vector[kernId].pixel[vecId] = result;
		}
	}
	if (KERN_NUM > 0)
		output[0] = output_vector[0];
	if (KERN_NUM > 1)
		output[1] = output_vector[1];
}

/// 2/2 overload of compute filter
/** @brief Selects and computes a filter
@param InType       Data type of the input image
@param OutType      Data type of the output image
@param KernType     Data type of the kernel
@param VEC_NUM     Vector size
@param KERN_NUM     Amount of filter kernel
@param KERN_SIZE    Filter kernel size
@param WIN_COLS     Columns of the sliding window
@param window       The sliding window
@param kernel       The convolution kernels
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
@param output       The results (vector) (per clock cycle)
*/
template <typename InType, typename OutType, typename KernType, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 KERN_SIZE, vx_uint16 WIN_COLS>
void ComputeFilter(InType window[KERN_SIZE][WIN_COLS], InType data_input[VEC_NUM],
	 KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE],
	const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM], vx_image_t<OutType, VEC_NUM> output[KERN_NUM]) {

#ifdef Xilinx
#pragma HLS INLINE
#endif
	vx_image_t<OutType, VEC_NUM> output_vector[KERN_NUM] hls_register;

	// Compute KERN_NUM different filter with VEC_NUM elements per clock cycle
UNROLL_INTEL()
	for (vx_uint16 kernId = 0; kernId < KERN_NUM; kernId++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint16 vecId = 0; vecId < VEC_NUM; vecId++) {
UNROLL_XILINX()

			// Window for single vector element
#ifdef Xilinx
			InType kernel_window[KERN_SIZE][KERN_SIZE];
			#pragma HLS array_partition variable=kernel_window complete dim=0
#elif Intel
			InType kernel_window[KERN_SIZE][KERN_SIZE] hls_register;
#endif

			// Get window for single vector element
UNROLL_INTEL()
			for (vx_uint16 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
				for (vx_uint16 j = 0; j < KERN_SIZE; j++) {
UNROLL_XILINX()
					kernel_window[i][j] = window[i][j + vecId];
				}
			}

			// Check, if input or output is signed
			bool isSigned = (numeric_limits<InType>::is_signed) || (numeric_limits<OutType>::is_signed);

			// Compute filter for single vector element
			OutType result = 0;
			switch (kernel_type[kernId]) {
			case HIFLIPVX::NON_MAX_SUPPRESSION_PHASE4:
				result = ComputeNonMaxSuppressionPhase<InType, OutType, 
					KERN_SIZE>(kernel_window, data_input[vecId], kernel_mult, kernel_shift);
				break;
			default:
				break;
			}
			output_vector[kernId].pixel[vecId] = result;
		}
	}
	if (KERN_NUM > 0)
		output[0] = output_vector[0];
	if (KERN_NUM > 1)
		output[1] = output_vector[1];
}


/** @brief  Checks the input parameters of the convolution filter
@param InType       Data type of the input image (8-bit, 16-bit, 32-bit)
@param OutType      Data type of the output image (8-bit, 16-bit, 32-bit)
@param VEC_NUM      Amount of pixels computed in parallel (1, 2, 4, 8)
@param KERN_NUM     Amount of pixels computed in parallel (1, 2)
@param IMG_COLS     Image width
@param KERN_SIZE    Kernel size (3, 5, 7, 9, 11)
*/
template<typename InType, typename OutType, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 KERN_SIZE>
void CheckFilterParameters() {

	// Check function parameters/types
	const vx_uint16 src_size = sizeof(InType);
	const vx_uint16 dst_size = sizeof(OutType);
	const bool allowed_kernel_size = (KERN_SIZE == 3) || (KERN_SIZE == 5) || (KERN_SIZE == 7) || (KERN_SIZE == 9) || (KERN_SIZE == 11);
	const bool allowed_vector_size = (VEC_NUM == 1) || (VEC_NUM == 2) || (VEC_NUM == 4) || (VEC_NUM == 8);
	const bool allowed_data_type = (src_size == 1) || (src_size == 2) || (src_size == 4);
	const bool allowed_kernel_type = (src_size == 1) || (src_size == 2);
	const bool allowed_kernel_num = (KERN_NUM == 1) || (KERN_NUM == 2);
	STATIC_ASSERT(allowed_kernel_size, kernel_size_must_be_3_5_7_9_11);
	STATIC_ASSERT((IMG_COLS % VEC_NUM == 0), image_colums_are_not_multiple_of_vector_size);
	STATIC_ASSERT((src_size == dst_size), size_of_in_and_out_type_must_be_equal);
	STATIC_ASSERT(allowed_vector_size, vector_size_must_be_1_2_4_8);
	STATIC_ASSERT(allowed_data_type, data_type_must_be_8_16_32_bit);
	STATIC_ASSERT(allowed_kernel_type, kernel_data_type_must_be_8_16_bit);
	STATIC_ASSERT(allowed_kernel_num, kernel_num_can_only_be_1__or_2);
}

/*********************************************************************************************************************/
/* Main Functions */
/*********************************************************************************************************************/

/** @brief  Computes multiple 1d convolution filter of kernel_type type
@param ScalarType   Data type of the input/output image (8-bit, 16-bit, 32-bit)
@param KernelType   Data type of the kernel (8-bit, 16-bit, 32-bit)
@param BufferType   (sizeof(InType) * VEC_NUM * (KERN_SIZE - 1)) == (sizeof(BufferType) * BUFFER_NUM)
@param BUFFER_NUM   (sizeof(InType) * VEC_NUM * (KERN_SIZE - 1)) == (sizeof(BufferType) * BUFFER_NUM)
@param VEC_PIX      The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM      Amount of pixels computed in parallel (1, 2, 4, 8)
@param KERN_NUM     Amount of pixels computed in parallel (1, 2)
@param IMG_COLS     Image width
@param IMG_ROWS     Image height
@param KERN_SIZE    Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE  Type of border (Constant, Replicated or Undefined)
@param input        Input image
@param output       Output image
@param kernel       The convolution kernel
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
*/
template <typename ScalarType, typename KernelType, typename BufferType, vx_uint16 BUFFER_NUM, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter1dFunc(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX], KernelType kernel[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type) {
#elif Intel
void ComputeFilter1dFunc(vx_image<ScalarType, VEC_NUM, stream_type0>& input, vx_image<ScalarType, VEC_NUM, stream_type1>& output, KernelType kernel[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Constants
	const vx_uint16 VEC_COLS = IMG_COLS / VEC_NUM;
	const vx_uint16 KERN_RAD = KERN_SIZE >> 1;
	const vx_uint16 WIN_COLS = 2 * KERN_RAD + VEC_NUM + (VEC_NUM - (KERN_RAD % VEC_NUM)) % VEC_NUM;
	const vx_uint16 OHD_COLS = (WIN_COLS - KERN_RAD) / VEC_NUM - 1;
	const vx_uint16 LINE_BUFFER_WIDTH = VEC_NUM*(KERN_SIZE - 1);

	// Linebuffer
	BufferType linebuffer[BUFFER_NUM][VEC_COLS];

	// Sliding window for complete vector
	#ifdef Xilinx
	ScalarType window_hor[1][WIN_COLS];
	#pragma HLS array_partition variable=window_hor complete dim=0
	ScalarType window_ver[KERN_SIZE][VEC_NUM];
	#pragma HLS array_partition variable=window_ver complete dim=0
	#elif Intel
	ScalarType window_hor[1][WIN_COLS]hls_register;
	ScalarType window_ver[KERN_SIZE][VEC_NUM]hls_register;
	#endif

	vx_uint16 op_x1= (VEC_COLS) + OHD_COLS;	
    vx_uint16 op_x2= ((op_x1 & 0xFF00) >> (8 - 1)) * 8 ; //msb x 2	
    vx_uint8 lim_x1=  (op_x2 & 0xFF) + 1;
	if (lim_x1 == 0)
		lim_x1++;
    vx_uint8 lim_x2=  op_x1 & 0xFF;

	// Compute the filter (pipelined)
	vx_uint32 ptr_src = 0, ptr_dst = 0;
#pragma ii 1
	for (vx_uint16 y = 0; y < IMG_ROWS + KERN_RAD; y++) {
#pragma ivdep array(linebuffer)
		for (vx_uint16 x = 0; x < VEC_COLS + OHD_COLS; x++) {
		#ifdef Xilinx
			// Variables (Registers)
		ScalarType input_buffer[VEC_NUM];
		#pragma HLS array_partition variable=input_buffer complete dim=0
      		ScalarType buffer[KERN_SIZE][VEC_NUM];
		#pragma HLS array_partition variable=buffer complete dim=0
      		ScalarType internal_buffer[VEC_NUM];
		#pragma HLS array_partition variable=internal_buffer complete dim=0
		#elif Intel
					ScalarType input_buffer[VEC_NUM] hls_register;
      		ScalarType buffer[KERN_SIZE][VEC_NUM] hls_register;
      		ScalarType internal_buffer[VEC_NUM] hls_register;
		#endif
			vx_image_t<ScalarType, VEC_NUM> input_data;
			vx_image_t<ScalarType, VEC_NUM> output_data;

			// Read input data from global memory
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				#ifdef Xilinx
				input_data = input[ptr_src];
				#elif Intel
				input_data = input.read();
				#endif
				ptr_src++;
			}
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				input_buffer[v] = input_data.pixel[v];
			}

			// Compute Horizontal Filter including sliding window
			SlidingWindowHorizontal<ScalarType, KERN_RAD, VEC_COLS, VEC_NUM, WIN_COLS, BORDER_TYPE>(input_buffer, window_hor, x);
			ComputeFilterHorizontal<ScalarType, KernelType, KERN_SIZE, WIN_COLS, VEC_NUM>(window_hor[0], kernel, kernel_mult, kernel_shift, kernel_type, internal_buffer);

			// Compute linebuffer
			vx_int16 xs = x - OHD_COLS;
			if (xs >= 0) {
				ReadFromLineBuffer<ScalarType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(internal_buffer, linebuffer, buffer, xs);
			}

			// Compute Vertical Filter including sliding window
			SlidingWindowVertical<ScalarType, IMG_ROWS, KERN_RAD, VEC_NUM, KERN_SIZE, BORDER_TYPE>(buffer, window_ver, y);

			if (xs >= 0) {
				WriteToLineBuffer<ScalarType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(buffer, linebuffer, xs);
			}

			ComputeFilterVertical<ScalarType, KernelType, KERN_SIZE, VEC_NUM>(window_ver, kernel, kernel_mult, kernel_shift, kernel_type, output_data);

			// Write output data to global memory
			if ((y >= KERN_RAD) && (x >= OHD_COLS)) {
				#ifdef Xilinx
				output[ptr_dst] = output_data;
				#elif Intel
				output.write(output_data);
				#endif
				ptr_dst++;
			}
		}
	}
}

/** @brief  Computes multiple 1d convolution filter of kernel_type type
@param ScalarType   Data type of the input/output image (8-bit, 16-bit, 32-bit)
@param KernType     Data type of the kernel (8-bit, 16-bit, 32-bit)
@param VEC_PIX      The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM      Amount of pixels computed in parallel (1, 2, 4, 8)
@param IMG_COLS     Image width
@param IMG_ROWS     Image height
@param KERN_SIZE    Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE  Type of border (Constant, Replicated or Undefined)
@param input        Input image
@param output       Output image
@param kernel       The convolution kernel
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
*/
template <typename ScalarType, typename KernelType, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter1d(vx_image<ScalarType, VEC_NUM> input [(IMG_ROWS*IMG_COLS) / VEC_NUM], vx_image<ScalarType, VEC_NUM> *output, KernelType kernel[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type) {
#elif Intel
void ComputeFilter1d(vx_image<ScalarType, VEC_NUM, stream_type0>& input, vx_image<ScalarType, VEC_NUM, stream_type1>& output, KernelType kernel[KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif

	// Check parameter input
	CheckFilterParameters<ScalarType, ScalarType, VEC_NUM, 1, IMG_COLS, KERN_SIZE>();

	// Constants to compute optimum buffer width
	const vx_uint16 DATA_WIDTH = sizeof(ScalarType) * VEC_NUM * (KERN_SIZE - 1);
	const vx_uint16 BUFFER_UINT32 = DATA_WIDTH % 4;
	const vx_uint16 BUFFER_UINT16 = DATA_WIDTH % 2;
	const vx_uint16 BUFFER_NUM = (BUFFER_UINT32 == 0) ? (DATA_WIDTH >> 2) : ((BUFFER_UINT16 == 0) ? (DATA_WIDTH >> 1) : (DATA_WIDTH));
	const vx_uint16 BUFFER_WIDTH = (BUFFER_UINT32 == 0) ? (4) : ((BUFFER_UINT16 == 0) ? (2) : (1));

	// Sets the Buffer Data type to the maximum possible, to reduce the amount of BRAM used
	if (BUFFER_WIDTH == 4)
		ComputeFilter1dFunc<ScalarType, KernelType, vx_uint32, BUFFER_NUM, VEC_PIX, VEC_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel, kernel_mult, kernel_shift, kernel_type);
	else if (BUFFER_WIDTH == 2)
		ComputeFilter1dFunc<ScalarType, KernelType, vx_uint16, BUFFER_NUM, VEC_PIX, VEC_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel, kernel_mult, kernel_shift, kernel_type);
	else if (BUFFER_WIDTH == 1)
		ComputeFilter1dFunc<ScalarType, KernelType, vx_uint8, BUFFER_NUM, VEC_PIX, VEC_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Computes multiple 2d convolution filter of kernel_type type
@param InType       Data type of the input image (8-bit, 16-bit, 32-bit)
@param OutType      Data type of the output image (8-bit, 16-bit, 32-bit)
@param KernType     Data type of the kernel (8-bit, 16-bit, 32-bit)
@param BufferType   (sizeof(InType) * VEC_NUM * (KERN_SIZE - 1)) == (sizeof(BufferType) * BUFFER_NUM)
@param BUFFER_NUM   (sizeof(InType) * VEC_NUM * (KERN_SIZE - 1)) == (sizeof(BufferType) * BUFFER_NUM)
@param VEC_PIX      The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM      Amount of pixels computed in parallel (1, 2, 4, 8)
@param KERN_NUM     Amount of pixels computed in parallel (1, 2)
@param IMG_COLS     Image width
@param IMG_ROWS     Image height
@param KERN_SIZE    Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE  Type of border (Constant, Replicated or Undefined)
@param input        Input image
@param output1      1. Output image
@param output2      2. Output image
@param kernel       The convolution kernel
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
*/
template <typename InType, typename OutType, typename KernType, typename BufferType, vx_uint16 BUFFER_NUM, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e, int stream_type2 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2dFunc(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX],
			 vx_image<OutType, VEC_NUM, stream_type1> output1 [VEC_PIX],
			 vx_image<OutType, VEC_NUM, stream_type2> output2 [VEC_PIX],
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2dFunc(vx_image<InType, VEC_NUM, stream_type0>& input ,
			 vx_image<OutType, VEC_NUM, stream_type1>& output1 ,
			 vx_image<OutType, VEC_NUM, stream_type2>& output2 ,
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Constants
	const vx_uint16 VEC_COLS = IMG_COLS / VEC_NUM;
	const vx_uint16 KERN_RAD = KERN_SIZE / 2;
	const vx_uint16 WIN_COLS = 2 * KERN_RAD + VEC_NUM + (VEC_NUM - (KERN_RAD % VEC_NUM)) % VEC_NUM;
	const vx_uint16 OHD_COLS = (WIN_COLS - KERN_RAD) / VEC_NUM - 1;
	const vx_uint16 LINE_BUFFER_WIDTH = VEC_NUM*(KERN_SIZE - 1);

	// Linebuffer
	BufferType linebuffer[BUFFER_NUM][VEC_COLS];

#ifdef Xilinx
	// Sliding window for complete vector
	InType window[KERN_SIZE][WIN_COLS];
#pragma HLS array_partition variable=window complete dim=0
#elif Intel
	InType window[KERN_SIZE][WIN_COLS] hls_register;
#endif
	// Compute the filter (pipelined)
	vx_uint32 ptr_src = 0, ptr_dst = 0;


#pragma ii 1
	for (vx_uint16 y = 0; y < IMG_ROWS + KERN_RAD; y++) {
#pragma ivdep array(linebuffer)
		for (vx_uint16 x = 0; x < VEC_COLS + OHD_COLS; x++) {

#ifdef Xilinx
			InType input_buffer[VEC_NUM];
			#pragma HLS array_partition variable=input_buffer complete dim=0
#elif Intel
			InType input_buffer[VEC_NUM] hls_register;
#endif

#ifdef Xilinx
			InType buffer[KERN_SIZE][VEC_NUM];
			#pragma HLS array_partition variable=buffer complete dim=0
#elif Intel
			InType buffer[KERN_SIZE][VEC_NUM] hls_register;
#endif

			// Input & Output
			vx_image_t<InType, VEC_NUM> input_data;
			vx_image_t<OutType, VEC_NUM> output_data[KERN_NUM];

			// Read input data from global memory
			#ifdef Xilinx
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input[ptr_src];
				ptr_src++;
			}
			#elif Intel
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input.read();
				ptr_src++;
			}
			#endif
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				input_buffer[v] = input_data.pixel[v];
			}

			// Read data from line_buffer to buffer
			ReadFromLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(input_buffer, linebuffer, buffer, x);

			// Move sliding window with replicated/constant border
			SlidingWindow<InType, VEC_NUM, IMG_ROWS, KERN_RAD, VEC_COLS, WIN_COLS, KERN_SIZE, BORDER_TYPE>(buffer, window, x, y);

			// Write data from buffer to line_buffer
			WriteToLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(buffer, linebuffer, x);

			// Compute filter
			ComputeFilter<InType, OutType, KernType, VEC_NUM, KERN_NUM, KERN_SIZE, WIN_COLS>(window, kernel, kernel_mult, kernel_shift, kernel_type, output_data);

			// Write output data to global memory
			if ((y >= KERN_RAD) && (x >= OHD_COLS)) {
				if (KERN_NUM > 0)
				#ifdef Xilinx
					output1[ptr_dst] = output_data[0];
				#elif Intel
					output1.write(output_data[0]);
				#endif
				if (KERN_NUM > 1)
				#ifdef Xilinx
					output2[ptr_dst] = output_data[1];
				#elif Intel
					output2.write( output_data[1]);
				#endif
				ptr_dst++;
			}
		}
	}
}
// 2/3 overload for Intel deves, null pointer is invalid operation, 
//
template <typename InType, typename OutType, typename KernType, typename BufferType, vx_uint16 BUFFER_NUM, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2dFunc(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX],
			 vx_image<OutType, VEC_NUM, stream_type1> output1 [VEC_PIX],
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2dFunc(vx_image<InType, VEC_NUM, stream_type0>& input ,
			 vx_image<OutType, VEC_NUM, stream_type1>& output1 ,
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Constants
	const vx_uint16 VEC_COLS = IMG_COLS / VEC_NUM;
	const vx_uint16 KERN_RAD = KERN_SIZE / 2;
	const vx_uint16 WIN_COLS = 2 * KERN_RAD + VEC_NUM + (VEC_NUM - (KERN_RAD % VEC_NUM)) % VEC_NUM;
	const vx_uint16 OHD_COLS = (WIN_COLS - KERN_RAD) / VEC_NUM - 1;
	const vx_uint16 LINE_BUFFER_WIDTH = VEC_NUM*(KERN_SIZE - 1);

	// Linebuffer
	BufferType linebuffer[BUFFER_NUM][VEC_COLS];

#ifdef Xilinx
	// Sliding window for complete vector
	InType window[KERN_SIZE][WIN_COLS];
#pragma HLS array_partition variable=window complete dim=0
#elif Intel
	InType window[KERN_SIZE][WIN_COLS] hls_register;
#endif
	// Compute the filter (pipelined)
	vx_uint32 ptr_src = 0, ptr_dst = 0;
	

#pragma ii 1
	for (vx_uint16 y = 0; y < IMG_ROWS + KERN_RAD; y++) {
#pragma ivdep array(linebuffer)
		for (vx_uint16 x = 0; x < VEC_COLS + OHD_COLS; x++) {

#ifdef Xilinx
			InType input_buffer[VEC_NUM];
			#pragma HLS array_partition variable=input_buffer complete dim=0
#elif Intel
			InType input_buffer[VEC_NUM] hls_register;
#endif

#ifdef Xilinx
			InType buffer[KERN_SIZE][VEC_NUM];
			#pragma HLS array_partition variable=buffer complete dim=0
#elif Intel
			InType buffer[KERN_SIZE][VEC_NUM] hls_register;
#endif

			// Input & Output
			vx_image_t<InType, VEC_NUM> input_data;
			vx_image_t<OutType, VEC_NUM> output_data[KERN_NUM];

			// Read input data from global memory
			#ifdef Xilinx
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input[ptr_src];
				ptr_src++;
			}
			#elif Intel
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input.read();
				ptr_src++;
			}
			#endif
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				input_buffer[v] = input_data.pixel[v];
			}

			// Read data from line_buffer to buffer
			ReadFromLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(input_buffer, linebuffer, buffer, x);

			// Move sliding window with replicated/constant border
			SlidingWindow<InType, VEC_NUM, IMG_ROWS, KERN_RAD, VEC_COLS, WIN_COLS, KERN_SIZE, BORDER_TYPE>(buffer, window, x, y);

			// Write data from buffer to line_buffer
			WriteToLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(buffer, linebuffer, x);

			// Compute filter
			ComputeFilter<InType, OutType, KernType, VEC_NUM, KERN_NUM, KERN_SIZE, WIN_COLS>(window, kernel, kernel_mult, kernel_shift, kernel_type, output_data);

			// Write output data to global memory
			if ((y >= KERN_RAD) && (x >= OHD_COLS)) {
				if (KERN_NUM > 0)
				#ifdef Xilinx
					output1[ptr_dst] = output_data[0];
				#elif Intel
					output1.write(output_data[0]);
				#endif
				ptr_dst++;
			}
		}
	}
}


// 3/3 overload for two stream (phase canny) 
//
template <typename InType, typename OutType, typename KernType, typename BufferType, vx_uint16 BUFFER_NUM, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2dInFunc(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX],
			 vx_image<InType, VEC_NUM, stream_type0> input1 [VEC_PIX],
			 vx_image<OutType, VEC_NUM, stream_type1> output1 [VEC_PIX],
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2dInFunc(vx_image<InType, VEC_NUM, stream_type0>& input ,
			 vx_image<InType, VEC_NUM, stream_type0>& input1,
			 vx_image<OutType, VEC_NUM, stream_type1>& output1 ,
	KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Constants
	const vx_uint16 VEC_COLS = IMG_COLS / VEC_NUM;
	const vx_uint16 KERN_RAD = KERN_SIZE / 2;
	const vx_uint16 WIN_COLS = 2 * KERN_RAD + VEC_NUM + (VEC_NUM - (KERN_RAD % VEC_NUM)) % VEC_NUM;
	const vx_uint16 OHD_COLS = (WIN_COLS - KERN_RAD) / VEC_NUM - 1;
	const vx_uint16 LINE_BUFFER_WIDTH = VEC_NUM*(KERN_SIZE - 1);

	// Linebuffer
	BufferType linebuffer[BUFFER_NUM][VEC_COLS];

#ifdef Xilinx
	// Sliding window for complete vector
	InType window[KERN_SIZE][WIN_COLS];
#pragma HLS array_partition variable=window complete dim=0
#elif Intel
	InType window[KERN_SIZE][WIN_COLS] hls_register;
#endif
	// Compute the filter (pipelined)
	vx_uint32 ptr_src = 0, ptr_dst = 0;
	

#pragma ii 1
	for (vx_uint16 y = 0; y < IMG_ROWS + KERN_RAD; y++) {
#pragma ivdep array(linebuffer)
		for (vx_uint16 x = 0; x < VEC_COLS + OHD_COLS; x++) {

#ifdef Xilinx
			InType input_buffer[VEC_NUM];
			#pragma HLS array_partition variable=input_buffer complete dim=0
			InType input_buffer1[VEC_NUM];
			#pragma HLS array_partition variable=input_buffer complete dim=0
#elif Intel
			InType input_buffer[VEC_NUM] hls_register;
			InType input_buffer1[VEC_NUM] hls_register;
#endif

#ifdef Xilinx
			InType buffer[KERN_SIZE][VEC_NUM];
#elif Intel
			InType buffer[KERN_SIZE][VEC_NUM] hls_register;
#endif

			// Input & Output
			vx_image_t<InType, VEC_NUM> input_data;
			vx_image_t<InType, VEC_NUM> input_data1;
			vx_image_t<OutType, VEC_NUM> output_data[KERN_NUM];

			// Read input data from global memory
			#ifdef Xilinx
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input[ptr_src];
				input_data1= input1[pts_src]		
				ptr_src++;
			}
			#elif Intel
			if ((y < IMG_ROWS) && (x < VEC_COLS)) {
				input_data = input.read();
				input_data1 = input1.read();
				ptr_src++;
			}
			#endif
UNROLL_INTEL()
			for (vx_uint16 v = 0; v < VEC_NUM; v++) {
UNROLL_XILINX()
				input_buffer[v] = input_data.pixel[v];
				input_buffer1[v] = input_data1.pixel[v];
			}

			// Read data from line_buffer to buffer
			ReadFromLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(input_buffer, linebuffer, buffer, x);

			// Move sliding window with replicated/constant border
			SlidingWindow<InType, VEC_NUM, IMG_ROWS, KERN_RAD, VEC_COLS, WIN_COLS, KERN_SIZE, BORDER_TYPE>(buffer, window, x, y);

			// Write data from buffer to line_buffer
			WriteToLineBuffer<InType, BufferType, BUFFER_NUM, VEC_NUM, KERN_SIZE, VEC_COLS>(buffer, linebuffer, x);

			// Compute filter
			ComputeFilter<InType, OutType, KernType, VEC_NUM, KERN_NUM, KERN_SIZE, WIN_COLS>(window, input_buffer1, kernel, kernel_mult, kernel_shift, kernel_type, output_data);

			// Write output data to global memory
			if ((y >= KERN_RAD) && (x >= OHD_COLS)) {
				if (KERN_NUM > 0)
				#ifdef Xilinx
					output1[ptr_dst] = output_data[0];
				#elif Intel
					output1.write(output_data[0]);
				#endif
				ptr_dst++;
			}
		}
	}
}
/******************************************************************************/

/** @brief  Computes multiple 2d convolution filter of kernel_type type
@param InType       Data type of the input image (8-bit, 16-bit, 32-bit)
@param OutType      Data type of the output image (8-bit, 16-bit, 32-bit)
@param KernType     Data type of the kernel (8-bit, 16-bit, 32-bit)
@param VEC_PIX      The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM      Amount of pixels computed in parallel (1, 2, 4, 8)
@param KERN_NUM     Amount of pixels computed in parallel (1, 2)
@param IMG_COLS     Image width
@param IMG_ROWS     Image height
@param KERN_SIZE    Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE  Type of border (Constant, Replicated or Undefined)
@param input        Input image
@param output1      1. Output image
@param output2      2. Output image
@param kernel       The convolution kernel
@param kernel_mult  Resulting pixels are 1. multiplied by this amount
@param kernel_shift Resulting pixels are 2. shifted to the right by this amount
@param kernel_type  The type of convolution
*/
template <typename InType, typename OutType, typename KernType, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e, int stream_type2 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2d(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX], vx_image<OutType, VEC_NUM,stream_type1> output1 [VEC_PIX], vx_image<OutType, VEC_NUM,stream_type2> output2 [VEC_PIX], KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2d(vx_image<InType, VEC_NUM, stream_type0>& input, vx_image<OutType, VEC_NUM,stream_type1>& output1 , vx_image<OutType, VEC_NUM,stream_type2>& output2 , KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Check parameter input
	CheckFilterParameters<InType, OutType, VEC_NUM, KERN_NUM, IMG_COLS, KERN_SIZE>();

	// Constants to compute optimum buffer width
	const vx_uint16 DATA_WIDTH = sizeof(InType) * VEC_NUM * (KERN_SIZE - 1);
	const vx_uint16 BUFFER_UINT32 = DATA_WIDTH % 4;
	const vx_uint16 BUFFER_UINT16 = DATA_WIDTH % 2;
	const vx_uint16 BUFFER_NUM = (BUFFER_UINT32 == 0) ? (DATA_WIDTH >> 2) : ((BUFFER_UINT16 == 0) ? (DATA_WIDTH >> 1) : (DATA_WIDTH));
	const vx_uint16 BUFFER_WIDTH = (BUFFER_UINT32 == 0) ? (4) : ((BUFFER_UINT16 == 0) ? (2) : (1));

      // Sets the Buffer Data type to the maximum possible, to reduce the amount of BRAM used
	if (BUFFER_WIDTH == 4)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint32, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,stream_type2>(input, output1, output2, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 2)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint16, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0,stream_type1,stream_type2>(input, output1, output2, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 1)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint8, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1,stream_type2>(input, output1, output2, kernel, kernel_mult, kernel_shift, kernel_type);
}

//Overload to generated less outputs, due null references are not valid (Intel needs references)
template <typename InType, typename OutType, typename KernType, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2d(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX], vx_image<OutType, VEC_NUM,stream_type1> output1 [VEC_PIX], KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2d(vx_image<InType, VEC_NUM, stream_type0>& input, vx_image<OutType, VEC_NUM,stream_type1>& output1, KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Check parameter input
	CheckFilterParameters<InType, OutType, VEC_NUM, KERN_NUM, IMG_COLS, KERN_SIZE>();

	// Constants to compute optimum buffer width
	const vx_uint16 DATA_WIDTH = sizeof(InType) * VEC_NUM * (KERN_SIZE - 1);
	const vx_uint16 BUFFER_UINT32 = DATA_WIDTH % 4;
	const vx_uint16 BUFFER_UINT16 = DATA_WIDTH % 2;
	const vx_uint16 BUFFER_NUM = (BUFFER_UINT32 == 0) ? (DATA_WIDTH >> 2) : ((BUFFER_UINT16 == 0) ? (DATA_WIDTH >> 1) : (DATA_WIDTH));
	const vx_uint16 BUFFER_WIDTH = (BUFFER_UINT32 == 0) ? (4) : ((BUFFER_UINT16 == 0) ? (2) : (1));

      // Sets the Buffer Data type to the maximum possible, to reduce the amount of BRAM used
	if (BUFFER_WIDTH == 4)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint32, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output1, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 2)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint16, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0,stream_type1>(input, output1, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 1)
			ComputeFilter2dFunc<InType, OutType, KernType, vx_uint8, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1>(input, output1, kernel, kernel_mult, kernel_shift, kernel_type);
}

//overload to received 2 streams as input, as example phase from canny edge
template <typename InType, typename OutType, typename KernType, vx_uint32 VEC_PIX, vx_uint16 VEC_NUM, vx_uint16 KERN_NUM, vx_uint16 IMG_COLS, vx_uint16 IMG_ROWS, vx_uint16 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void ComputeFilter2dIn(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX],vx_image<InType, VEC_NUM, stream_type0> input1 [VEC_PIX] ,vx_image<OutType, VEC_NUM,stream_type1> output1 [VEC_PIX], KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#elif Intel
void ComputeFilter2dIn(vx_image<InType, VEC_NUM, stream_type0>& input, vx_image<InType, VEC_NUM, stream_type0>& input1,vx_image<OutType, VEC_NUM,stream_type1>& output1, KernType kernel[KERN_NUM][KERN_SIZE][KERN_SIZE], const vx_uint64 kernel_mult, const vx_uint32 kernel_shift, const HIFLIPVX::FilterOperation kernel_type[KERN_NUM]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Check parameter input
	CheckFilterParameters<InType, OutType, VEC_NUM, KERN_NUM, IMG_COLS, KERN_SIZE>();

	// Constants to compute optimum buffer width
	const vx_uint16 DATA_WIDTH = sizeof(InType) * VEC_NUM * (KERN_SIZE - 1);
	const vx_uint16 BUFFER_UINT32 = DATA_WIDTH % 4;
	const vx_uint16 BUFFER_UINT16 = DATA_WIDTH % 2;
	const vx_uint16 BUFFER_NUM = (BUFFER_UINT32 == 0) ? (DATA_WIDTH >> 2) : ((BUFFER_UINT16 == 0) ? (DATA_WIDTH >> 1) : (DATA_WIDTH));
	const vx_uint16 BUFFER_WIDTH = (BUFFER_UINT32 == 0) ? (4) : ((BUFFER_UINT16 == 0) ? (2) : (1));

      // Sets the Buffer Data type to the maximum possible, to reduce the amount of BRAM used
	if (BUFFER_WIDTH == 4)
			ComputeFilter2dInFunc<InType, OutType, KernType, vx_uint32, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, input1, output1, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 2)
			ComputeFilter2dInFunc<InType, OutType, KernType, vx_uint16, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE, stream_type0,stream_type1>(input, input1, output1, kernel, kernel_mult, kernel_shift, kernel_type);
		else if (BUFFER_WIDTH == 1)
			ComputeFilter2dInFunc<InType, OutType, KernType, vx_uint8, BUFFER_NUM, VEC_PIX, VEC_NUM, KERN_NUM, IMG_COLS, IMG_ROWS, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1>(input, input1, output1, kernel, kernel_mult, kernel_shift, kernel_type);
}

/*********************************************************************************************************************/
/* Filter Function Definition */
/*********************************************************************************************************************/

/** @brief  Computes a Box filter over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param SEPARABLE   Is true, if separable filter should be used
@param input       Input image
@param output      Output image
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwBox(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwBox(vx_image<ScalarType, VEC_NUM, stream_type0>& input , vx_image<ScalarType, VEC_NUM, stream_type1>& output ) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<ScalarType>::is_signed == false, scalar_type_must_be_unsigned);

	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	vx_uint64 kernel_mult = 1;
	vx_uint32 kernel_shift = 0;

	if (SEPARABLE == vx_false_e) {

		// Function Input
		#ifdef Xilinx
		KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
		#pragma HLS array_partition variable=kernel_vector complete dim=0
		#elif Intel
		KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE] hls_register;
		#endif

		// Compute kernel and normalization (Compile-Time)
UNROLL_INTEL()
		for (vx_uint8 y = 0; y < KERN_SIZE; y++) {
UNROLL_XILINX()
UNROLL_INTEL()
			for (vx_uint8 x = 0; x < KERN_SIZE; x++) {
UNROLL_XILINX()
				kernel_vector[0][y][x] = 1;
			}
		}
		ComputeNormalization2d<KernelType, KERN_SIZE>(kernel_vector[0], kernel_mult, kernel_shift);

		// Kernel normalization and type
		const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::BOX_FILTER };

		// Compute Filter
		ComputeFilter2d<ScalarType, ScalarType, KernelType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);

	} else {

		// Function Input
		#ifdef Xilinx
		KernelType kernel_vector[KERN_SIZE];
		#pragma HLS array_partition variable=kernel_vector complete dim=0
		#elif Intel
		KernelType kernel_vector[KERN_SIZE]hls_register;
		#endif
		// Compute kernel and normalization (Compile-Time)
UNROLL_INTEL()
		for (vx_uint8 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
			kernel_vector[i] = 1;
		}
		ComputeNormalization1d<KernelType, KERN_SIZE>(kernel_vector, kernel_mult, kernel_shift);

		// Kernel normalization and type
		const HIFLIPVX::FilterOperation kernel_type = HIFLIPVX::BOX_FILTER;

		// Compute Filter
		ComputeFilter1d<ScalarType, KernelType, VEC_PIX, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);
	}
}

/** @brief  Convolves the input with the client supplied convolution matrix. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned, signed)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param input       Input image
@param output      Output image
@param conv        The custom convolution kernel
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwConvolve(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX], const ScalarType conv[KERN_SIZE][KERN_SIZE]) {
#elif Intel
void HwConvolve(vx_image<ScalarType, VEC_NUM, stream_type0>& input , vx_image<ScalarType, VEC_NUM, stream_type1>& output , const ScalarType conv[KERN_SIZE][KERN_SIZE]) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	vx_uint64 kernel_mult = 1;
	vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	ScalarType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	ScalarType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE] hls_register;
	#endif
	// Compute kernel and normalization (Compile-Time)
UNROLL_INTEL()
	for (vx_uint8 i = 0; i < KERN_SIZE; i++) {
UNROLL_XILINX()
UNROLL_INTEL()
		for (vx_uint8 j = 0; j < KERN_SIZE; j++) {
UNROLL_XILINX()
			kernel_vector[0][i][j] = conv[i][j];
		}
	}
	ComputeNormalization2d<ScalarType, KERN_SIZE>(kernel_vector[0], kernel_mult, kernel_shift);

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::CUSTOM_CONVOLUTION };

	// Compute Filter
	ComputeFilter2d<ScalarType, ScalarType, ScalarType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Implements Dilation, which grows the white space in a Boolean image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param input       Input image
@param output      Output image
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwDilate(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwDilate(vx_image<ScalarType, VEC_NUM, stream_type0>& input , vx_image<ScalarType, VEC_NUM, stream_type1>& output ) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif

	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<ScalarType>::is_signed == false, scalar_type_must_be_unsigned);

	// Kernel Parameters
	typedef vx_uint16 KernelType;

	#ifdef Xilinx// Function Input
	KernelType kernel_vector[KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_SIZE] hls_register;
	#endif
	// Kernel normalization and type
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;
	const HIFLIPVX::FilterOperation kernel_type = HIFLIPVX::DILATE_IMAGE;

	// Compute Filter
	ComputeFilter1d<ScalarType, KernelType, VEC_PIX, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Implements Erosion, which shrinks the white space in a Boolean image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param input       Input image
@param output      Output image
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwErode(vx_image<ScalarType, VEC_NUM> input[VEC_PIX], vx_image<ScalarType, VEC_NUM> output[VEC_PIX]) {
#elif Intel
void HwErode(vx_image<ScalarType, VEC_NUM, stream_type0>& input, vx_image<ScalarType, VEC_NUM, stream_type1>& output) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif
	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<ScalarType>::is_signed == false, scalar_type_must_be_unsigned);

	// Kernel Parameters
	typedef vx_uint16 KernelType;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
        #elif Intel
	KernelType kernel_vector[KERN_SIZE] hls_register;
	#endif
	// Kernel normalization and type
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;
	const HIFLIPVX::FilterOperation kernel_type = HIFLIPVX::ERODE_IMAGE;

	// Compute Filter
	ComputeFilter1d<ScalarType, KernelType, VEC_PIX, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Computes a Gaussian filter over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param SEPARABLE   Is true, if separable filter should be used
@param input       Input image
@param output      Output image
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwGaussian(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwGaussian(vx_image<ScalarType, VEC_NUM, stream_type0>& input , vx_image<ScalarType, VEC_NUM, stream_type1>& output ) {
 #endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<ScalarType>::is_signed == false, scalar_type_must_be_unsigned);

	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	vx_uint64 kernel_mult = 1;
	vx_uint32 kernel_shift = 0;

	if (SEPARABLE == vx_false_e) {

		// Function Input
		#ifdef Xilinx
		KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
		#pragma HLS array_partition variable=kernel_vector complete dim=0
	        #elif Intel
		KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE] hls_register;
		#endif
		// Compute kernel and normalization (Compile-Time)
		ComputeGaussianKernels<KernelType, KERN_SIZE>(kernel_vector[0]);
		ComputeNormalization2d<KernelType, KERN_SIZE>(kernel_vector[0], kernel_mult, kernel_shift);

		// Set normalization and type
		const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::GAUSSIAN_FILTER };

		// Compute Filter
		ComputeFilter2d<ScalarType, ScalarType, KernelType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);

	} else {

		// Function Input
		#ifdef Xilinx
		KernelType kernel_vector[KERN_SIZE][KERN_SIZE];
		#pragma HLS array_partition variable=kernel_vector complete dim=0
		#elif Intel
		KernelType kernel_vector[KERN_SIZE][KERN_SIZE] hls_register;
		#endif
		// Compute kernel and normalization (Compile-Time)
		ComputeGaussianKernels<KernelType, KERN_SIZE>(kernel_vector);
		ComputeNormalization1d<KernelType, KERN_SIZE>(kernel_vector[0], kernel_mult, kernel_shift);

		// Kernel normalization and type
		const HIFLIPVX::FilterOperation kernel_type = HIFLIPVX::GAUSSIAN_FILTER;

		// Compute Filter
		ComputeFilter1d<ScalarType, KernelType, VEC_PIX, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input, output, kernel_vector[0], kernel_mult, kernel_shift, kernel_type);
	}
}

/** @brief  Computes a median pixel value over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param input       Input image
@param output      Output image
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE,int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e>
#ifdef Xilinx
void HwMedian(vx_image<ScalarType, VEC_NUM, stream_type0> input [VEC_PIX], vx_image<ScalarType, VEC_NUM,stream_type1> output [VEC_PIX]) {
#elif Intel
void HwMedian(vx_image<ScalarType, VEC_NUM, stream_type0>& input , vx_image<ScalarType, VEC_NUM, stream_type1>& output ) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<ScalarType>::is_signed == false, scalar_type_must_be_unsigned);

	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE]hls_register;
	#endif

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::MEDIAN_FILTER };

	// Compute Filter
	ComputeFilter2d<ScalarType, ScalarType, KernelType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1>(input, output, kernel_vector, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Implements the Scharr Image Filter Kernel. The output images dimensions should be the same as the dimensions of the input image.
@param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit)
@param DstType     Data type of the output image (signed)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param input       The input image
@param output1     The output image (x derivative)
@param output2     The output image (y derivative)
*/
template <typename InType, typename OutType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e, int stream_type2 = vx_stream_e>
#ifdef Xilinx
void HwScharr3x3(vx_image<InType, VEC_NUM, stream_type0> input [VEC_PIX], vx_image<OutType, VEC_NUM, stream_type1> output1 [VEC_PIX], vx_image<OutType, VEC_NUM, stream_type2> output2 [VEC_PIX]) {
#elif Intel
void HwScharr3x3(vx_image<InType, VEC_NUM, stream_type0>& input , vx_image<OutType, VEC_NUM, stream_type1>& output1 , vx_image<OutType, VEC_NUM, stream_type2>& output2 ) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<InType>::is_signed == false, derivative_input_must_be_unsigned);
	STATIC_ASSERT(numeric_limits<OutType>::is_signed == true, derivative_output_must_be_signed);

	// Kernel Type
	typedef vx_int16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 2;
	const vx_uint8 KERN_SIZE = 3;
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 5;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[2][KERN_SIZE][KERN_SIZE] = {
		{
			{ 3, 0, -3 },
			{ 10, 0, -10 },
			{ 3, 0, -3 },
		}, {
			{ 3, 10, 3 },
			{ 0, 0, 0 },
			{ -3, -10, -3 },
		} };
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[2][KERN_SIZE][KERN_SIZE] hls_register= {
		{
			{ 3, 0, -3 },
			{ 10, 0, -10 },
			{ 3, 0, -3 },
		}, {
			{ 3, 10, 3 },
			{ 0, 0, 0 },
			{ -3, -10, -3 },
		} };
	#endif
	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::DERIVATIVE_X, HIFLIPVX::DERIVATIVE_Y };

	// Compute Filter
	ComputeFilter2d<InType, OutType, KernelType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>(input, output1, output2, kernel_vector, kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Implements the Sobel Image Filter Kernel. The output images dimensions should be the same as the dimensions of the input image.
@param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit)
@param DstType     Data type of the output image (signed)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 Intel hls stream type for input, ignored in Xilinx
@param stream_type1 Intel hls stream type for output, ignored in Xilinx
@param stream_type2 Intel hls stream type for output, ignored in Xilinx
@param input       The input image
@param output1     The output image (x derivative)
@param output2     The output image (y derivative)
*/
template <typename InType, typename OutType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0 = vx_stream_e, int stream_type1 = vx_stream_e, int stream_type2 = vx_stream_e>
#ifdef Xilinx
void HwSobel(vx_image<InType, VEC_NUM, stream_type0>input [VEC_PIX],vx_image<OutType, VEC_NUM, stream_type1> output1 [VEC_PIX], vx_image<OutType, VEC_NUM, stream_type2> output2 [VEC_PIX]) {
#elif Intel
void HwSobel(vx_image<InType, VEC_NUM, stream_type0> &input, vx_image<OutType, VEC_NUM, stream_type1> &output1, vx_image<OutType, VEC_NUM, stream_type2> &output2 ) {
#endif
#ifdef Xilinx
#pragma HLS INLINE
#endif

	// Check function parameters/types
	STATIC_ASSERT(numeric_limits<InType>::is_signed == false, input_must_be_unsigned);
	STATIC_ASSERT(numeric_limits<OutType>::is_signed == true, output_must_be_signed);

	// Kernel Type
	typedef vx_int16 KernelType;

	// Kernel Amount
	const vx_uint8 KERN_NUM = 2;

	// Kernel Normalization
	vx_uint64 kernel_mult = 1;
	vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE] hls_register;
	#endif

	// Compute kernel and normalization (Compile-Time)
	ComputeSobelKernels<KernelType, KERN_SIZE>(kernel_vector[0], kernel_vector[1]);
	ComputeNormalization2d<KernelType, KERN_SIZE>(kernel_vector[0], kernel_mult, kernel_shift);

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::DERIVATIVE_X, HIFLIPVX::DERIVATIVE_Y };

	// Compute Filter
	ComputeFilter2d<InType, OutType, KernelType, VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE,
					BORDER_TYPE,stream_type0,stream_type1,stream_type2>(input, output1, output2, kernel_vector,
					kernel_mult, kernel_shift, kernel_type);
}

/** @brief  Implements the Non-Max Suppression Kernel. The output images Find local maxima in an image, or otherwise suppress pixels that are not local maxima. Image dimensions should be the same as the dimensions of the input image.
@param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit)
@param DstType     Data type of the output image (signed)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 Intel hls stream type for input, ignored in Xilinx
@param stream_type1 Intel hls stream type for output, ignored in Xilinx
@param stream_type2 Intel hls stream type for output, ignored in Xilinx
@param input       The input image
@param output1     The output image (x derivative)
@param output2     The output image (y derivative)
*/
template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE,typename vx_type0 = vx_image<ScalarType, VEC_NUM, vx_stream_e>, typename vx_type1 = vx_image<ScalarType, VEC_NUM, vx_stream_e> >
#ifdef Xilinx
void HwNonMaximaSuppression(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwNonMaximaSuppression(vx_type0 &input, vx_type1 &output){
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
//ToDo: find conditions 
//
	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE]hls_register;
	#endif

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::NON_MAX_SUPPRESSION };

	// Compute Filter
	ComputeFilter2d<ScalarType, ScalarType, KernelType, 
		VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE,
		BORDER_TYPE>(input, output, kernel_vector,
		kernel_mult, kernel_shift, kernel_type);
}
//variation of nonMaxSuppression for canny edge detector, receive phase as input

template<typename ScalarType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE,typename vx_type0 = vx_image<ScalarType, VEC_NUM, vx_stream_e>, typename vx_type1 = vx_image<ScalarType, VEC_NUM, vx_stream_e> >
#ifdef Xilinx
void HwNonMaximaSuppressionPhase4(vx_image<ScalarType, VEC_NUM> input [VEC_PIX], vx_image<ScalarType, VEC_NUM> phase[VEC_PIX],vx_image<ScalarType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwNonMaximaSuppressionPhase4(vx_type0 &input, vx_type0 &phase, vx_type1 &output){
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
//ToDo: find conditions 
//
	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE]hls_register;
	#endif

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = 
		{ HIFLIPVX::NON_MAX_SUPPRESSION_PHASE4 };

	// Compute Filter
	ComputeFilter2dIn<ScalarType, ScalarType, KernelType, 
		VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE,
		BORDER_TYPE>(input, phase, output, kernel_vector,
		kernel_mult, kernel_shift, kernel_type);
}


/** @brief  Implements the Non-Max Suppression Kernel. The output images Find local maxima in an image, or otherwise suppress pixels that are not local maxima. Image dimensions should be the same as the dimensions of the input image.
@param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit)
@param DstType     Data type of the output image (signed)(8-bit, 16-bit, 32-bit)
@param VEC_PIX     The amount of pixels diveded by the pixels computed in parallel
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 Intel hls stream type for input, ignored in Xilinx
@param stream_type1 Intel hls stream type for output, ignored in Xilinx
@param stream_type2 Intel hls stream type for output, ignored in Xilinx
@param input       The input image
@param output1     The output image (x derivative)
@param output2     The output image (y derivative)
*/
template<typename InType, typename OutType, vx_uint32 VEC_PIX, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE,typename vx_type0 = vx_image<InType, VEC_NUM, vx_stream_e>, typename vx_type1 = vx_image<OutType, VEC_NUM, vx_stream_e> >
#ifdef Xilinx
void HwCensusTransform(vx_image<Intype, VEC_NUM> input [VEC_PIX], vx_image<outType, VEC_NUM> output [VEC_PIX]) {
#elif Intel
void HwCensusTransform(vx_type0 &input, vx_type1 &output){
#endif
#ifdef Xilinx
#pragma HLS INLINE
 #endif

	// Check function parameters/types
//ToDo: find conditions 
//
	// Kernel Type
	typedef vx_uint16 KernelType;

	// Kernel Parameters
	const vx_uint8 KERN_NUM = 1;
	const vx_uint64 kernel_mult = 1;
	const vx_uint32 kernel_shift = 0;

	// Function Input
	#ifdef Xilinx
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE];
	#pragma HLS array_partition variable=kernel_vector complete dim=0
	#elif Intel
	KernelType kernel_vector[KERN_NUM][KERN_SIZE][KERN_SIZE]hls_register;
	#endif

	// Kernel normalization and type
	const HIFLIPVX::FilterOperation kernel_type[KERN_NUM] = { HIFLIPVX::CENSUS_TRANSFORM };

	// Compute Filter
	ComputeFilter2d<InType, OutType, KernelType, 
		VEC_PIX, VEC_NUM, KERN_NUM, WIDTH, HEIGHT, KERN_SIZE,
		BORDER_TYPE>(input, output, kernel_vector,
		kernel_mult, kernel_shift, kernel_type);
}

#endif /* SRC_IMG_FILTER_CORE_H_ */
