/**
 * This file contains the specific implementation of the components to be compiled by Xilinx
 * @see img_pixelop_core.h for more information
 * */
template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
void ImgCopy(DataType input1[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::COPY_DATA, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input1, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, typename vx_type0, vx_type0 &input0>
void ImgNot(){
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_NOT, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input1, output);

template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgNot(DataType input1[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_NOT, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input1, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgAnd(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_AND, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgOr(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_OR, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgXor(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::BITWISE_XOR, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}



template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgMax(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MAX, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, int stream_type0= vx_stream_e>
void ImgMin(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MIN, VX_CONVERT_POLICY_WRAP, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY>
void ImgAbsDiff(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ABSOLUTE_DIFFERENCE, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY>
void ImgAdd(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ARITHMETIC_ADDITION, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY>
void ImgSubtract(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::ARITHMETIC_SUBTRACTION, CONV_POLICY, VX_ROUND_POLICY_TO_ZERO, 0>(input1, input2, output);
}



template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_round_policy_e ROUND_POLICY>
void ImgMagnitude(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MAGNITUDE, VX_CONVERT_POLICY_SATURATE, ROUND_POLICY, 0>(input1, input2, output);
}


template<typename DataType, vx_uint8 VEC_SIZE, vx_uint32 IMG_PIXEL, vx_convert_policy_e CONV_POLICY, vx_round_policy_e ROUND_POLICY, vx_uint32 SCALE>
void ImgMultiply(DataType input1[IMG_PIXEL / VEC_SIZE], DataType input2[IMG_PIXEL / VEC_SIZE], DataType output[IMG_PIXEL / VEC_SIZE]) {
 #ifdef Xilinx 
#pragma HLS INLINE 
 #endif  
	PixelwiseSameType<DataType, VEC_SIZE, IMG_PIXEL, HIFLIPVX::MULTIPLY, CONV_POLICY, ROUND_POLICY, SCALE>(input1, input2, output);
3~
68
3~23- 3026
}
