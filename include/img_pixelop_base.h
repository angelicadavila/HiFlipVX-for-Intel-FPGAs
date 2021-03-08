/**
* Licence: GNU GPLv3 \n
* You may copy, distribute and modify the software as long as you track
* changes/dates in source files. Any modifications to or software
* including (via compiler) GPL-licensed code must also be made available
* under the GPL along with build & install instructions.
*
* @file    img_pixelop_base.h
* @author  Lester Kalms <lester.kalms@tu-dresden.de>
* @version 1.3
* @brief Description:\n
*  These are all pixel operation functions (Call from here)
*/

#ifndef SRC_IMG_PIXELOP_BASE_H_
#define SRC_IMG_PIXELOP_BASE_H_

#include "img_pixelop_core.h"
#ifdef Xilinx
	#include "img_pixelop_component_xilinx.h"
#endif
/*********************************************************************************************************************/
/* Pixelwise Operations (Same Data Type) */
/*********************************************************************************************************************/

/** @brief Copy data from an input data object into another data object. The input and output object must have the same object type and meta data.
@param DataType        Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
#ifdef Xilinx 
void ImgCopy(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgCopy(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE, stream_type1> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::COPY_DATA, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input1, output);
}


/** @brief Performs a bitwise NOT operation on a input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
#ifdef Xilinx 
void ImgNot(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgNot(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE, stream_type1> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_NOT, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input1, output);
}


/** @brief Performs a bitwise AND operation between two images.The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL,
	typename vx_type0, typename vx_type1, typename vx_type2, 
	vx_type0 &input0, vx_type1 &input1, vx_type2 &output,
	int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
void ImgAndIntel()
{
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_AND, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}
//cass wrap
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL,
	typename vx_type0, typename vx_type1, typename vx_type2, 
	vx_type0 &input0, vx_type1 &input1, vx_type2 &output,
	int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
struct vxAnd(){ 
	vxAnd(){
#ifdef Xilinx 
#pragma HLS INLINE 
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_AND, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
#elif Intel
	ihc::launch((ImgAndIntel<DataType, VEC_SIZE, IMG_PIXEL, decltype(input0),
		decltype(input1), decltype(output), input0, input1, output>));
#endif
	}
     void vxReleaseNode(){
#ifdef Xilinx
#elif __INTELFPGA_COMPILER__
	ihc::collect((ImgAndIntel<DataType, VEC_SIZE, IMG_PIXEL, decltype(input0),
		decltype(input1), decltype(output), input0, input1, output>));
#endif
	}
};

/** @brief Performs a bitwise INCLUSIVE OR operation between two images. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgOr(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgOr(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE, stream_type1> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_OR, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

/** @brief Performs a bitwise EXCLUSIVE OR (XOR) operation between two images. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgXor(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgXor(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE, stream_type1> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_XOR, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

/** @brief Implements a pixel-wise maximum kernel. The output image dimensions should be the same as the dimensions of the input image.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgMax(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgMax(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MAX, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

/** @brief Implements a pixel-wise minimum kernel. The output image dimensions should be the same as the dimensions of the input image.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgMin(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgMin(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MIN, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

/** @brief Computes the absolute difference between two images. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param CONV_POLICY     The round conversion <tt>\ref vx_convert_policy_e</tt>
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgAbsDiff(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgAbsDiff(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ABSOLUTE_DIFFERENCE, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

/** @brief Performs addition between two images. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param CONV_POLICY     The round conversion <tt>\ref vx_convert_policy_e</tt>
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY,int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgAdd(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgAdd(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ARITHMETIC_ADDITION, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


/** @brief Performs subtraction between two images.The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param CONV_POLICY     The round conversion <tt>\ref vx_convert_policy_e</tt>
@param input1          Input image
@param input2          Input image
@param output          Output image
*/

/*@brief wrapper function for Intel FPGAs. stream arguments are valid only in template.
*/
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY, typename vx_type0, typename vx_type1, vx_type0 &input1, vx_type0 &input2, vx_type1 &output>
void ImgSubtractIntel () {
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ARITHMETIC_SUBTRACTION, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}

template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, 
	vx_convert_policy_e CONV_POLICY, typename vx_type0, typename vx_type1, 
	vx_type0 &input1, vx_type0 &input2, vx_type1 &output,
	int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct vxSubtract{
	vxSubtract(){
		#ifdef Xilinx 
		#pragma HLS INLINE 
		PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ARITHMETIC_SUBTRACTION, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
		#elif Intel
		ihc::launch (( ImgSubtractIntel<DataType, VEC_SIZE, IMG_PIXEL, CONV_POLICY, decltype(input1), decltype(output), input1, input2, output >)); 
		#endif
	}
	void vxReleaseNode(){ 
		 #ifdef Xilinx 
		 #elif Intel
		ihc::collect (( ImgSubtractIntel<DataType, VEC_SIZE, IMG_PIXEL, CONV_POLICY, decltype(input1), decltype(output), input1, input2, output >)); 
		 #endif  
	}
};

/** @brief Implements the Gradient Magnitude Computation Kernel. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param ROUND_POLICY    The round policy <tt>\ref vx_round_policy_e</tt>
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
//template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY,typename vx_type0, typename vx_type1, typename vx_type2>
void ImgMagnitudeXilinx(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE 
#endif
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MAGNITUDE, VX_CONVERT_POLICY_SATURATE, ROUND_POLICY, 0, vx_type0, vx_type1, vx_type2>(input1, input2, output);
}

/*@brief wrapper function for Intel FPGAs. stream arguments are valid only in template.
 */
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, 
typename vx_type0, typename vx_type1, typename vx_type2, 
vx_type0 &input0, vx_type1 &input1, vx_type2 &output2, 
int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
void ImgMagnitudeIntel(){
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MAGNITUDE, VX_CONVERT_POLICY_SATURATE, ROUND_POLICY, 0, vx_type0, vx_type1, vx_type2>(input0, input1, output2);
}

/**@brief Wrapper class for Xilinx/Intel integration
 * @see ImgMagnitud implementation
 */
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, 
         typename vx_type0, typename vx_type1, typename vx_type2, 
	 vx_type0 &input0, vx_type1 &input1, vx_type2 &output2,
         int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
struct ImgMagnitude{
	ImgMagnitude(){
#ifdef Xilinx
	ImgMagnitudeXilinx<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY>(input0, input1, output2);
#elif __INTELFPGA_COMPILER__
	ihc::launch((ImgMagnitudeIntel<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY, vx_type0, vx_type1, vx_type2, input0, input1, output2>));
#endif
	}
     	void vxReleaseNode(){
#ifdef Xilinx
#elif __INTELFPGA_COMPILER__
	ihc::collect((ImgMagnitudeIntel<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY, vx_type0, vx_type1, vx_type2, input0, input1, output2>));
#endif
	}
};


/** @brief Implements the Phase Computation Kernel for 4 degreess. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param ROUND_POLICY    The round policy <tt>\ref vx_round_policy_e</tt>
@param input1          Input image
@param input2          Input image
@param output          Output image
*/
//template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY,typename vx_type0, typename vx_type1, typename vx_type2>
void ImgPhase4Xilinx(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#ifdef Xilinx
#pragma HLS INLINE 
#endif
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::PHASE4, VX_CONVERT_POLICY_SATURATE, ROUND_POLICY, 0, vx_type0, vx_type1, vx_type2>(input1, input2, output);
}

/*@brief wrapper function for Intel FPGAs. stream arguments are valid only in template.
 */
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, 
typename vx_type0, typename vx_type1, typename vx_type2, 
vx_type0 &input0, vx_type1 &input1, vx_type2 &output2, 
int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
void ImgPhase4Intel(){
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::PHASE4, VX_CONVERT_POLICY_SATURATE, ROUND_POLICY, 0, vx_type0, vx_type1, vx_type2>(input0, input1, output2);
}

/**@brief Wrapper class for Xilinx/Intel integration
 * @see ImgPhase4 implementation
 */
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY, 
         typename vx_type0, typename vx_type1, typename vx_type2, 
	 vx_type0 &input0, vx_type1 &input1, vx_type2 &output2,
         int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
struct ImgPhase4{
	ImgPhase4(){
#ifdef Xilinx
	ImgPhase4Xilinx<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY>(input0, input1, output2);
#elif __INTELFPGA_COMPILER__
	ihc::launch((ImgPhase4Intel<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY, vx_type0, vx_type1, vx_type2, input0, input1, output2>));
#endif
	}
     	void vxReleaseNode(){
#ifdef Xilinx
#elif __INTELFPGA_COMPILER__
	ihc::collect((ImgPhase4Intel<DataType, VEC_NUM, IMG_PIXEL, ROUND_POLICY, vx_type0, vx_type1, vx_type2, input0, input1, output2>));
#endif
	}
};


/** @brief Performs element-wise multiplication between two images and a scalar value. The output image dimensions should be the same as the dimensions of the input images.
@param DataType      Data type of the image pixels
@param VEC_SIZE        Amount of pixels computed in parallel
@param IMG_PIXEL       Amount of pixels in the image
@param CONV_POLICY     The round conversion <tt>\ref vx_convert_policy_e</tt>
@param ROUND_POLICY    The round policy <tt>\ref vx_round_policy_e</tt>
@param SCALE           A number multiplied to each product before overflow handling.
@param input1          Input image
@param input2          Input image
@param output          Output image
*/

template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY, vx_round_policy_e ROUND_POLICY, vx_uint32 SCALE, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
#ifdef Xilinx 
void ImgMultiply(vx_image<DataType, VEC_SIZE> input1[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> input2[IMG_PIXEL / VEC_SIZE], vx_image<DataType, VEC_SIZE> output[IMG_PIXEL / VEC_SIZE]) {
#elif Intel
void ImgMultiply(vx_image<DataType, VEC_SIZE, stream_type0> &input1 , vx_image<DataType, VEC_SIZE> &input2, vx_image<DataType, VEC_SIZE, stream_type2> &output) {
#endif
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MULTIPLY, CONV_POLICY, ROUND_POLICY, SCALE>(input1, input2, output);
}

#endif /* SRC_IMG_PIXELOP_BASE_H_ */
