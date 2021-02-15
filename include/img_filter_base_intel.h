#ifndef SRC_IMG_FILTER_BASE_INTEL_H_
#define SRC_IMG_FILTER_BASE_INTEL_H_
#include "img_filter_core.h"
#include "link_OpenCLIntel.h"
#include "HLS/task.h" //system of task

//Intel wrapper to call the function as a task, Intel require: The streammings only can pass to a funtion through argument template
//are an invalid implementation
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0,int stream_type1, int stream_type2,typename vx_type0, typename vx_type1, typename vx_type2, vx_type0 &input0, vx_type1 &output1, vx_type2 &output2>
void ImgSobelIntel(){
	HwSobel<SrcType, DstType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0,stream_type1, stream_type2>(input0, output1, output2);
}
//ToDo: All the funtion under the same struct to build a vx_node, wth image parameters as commom
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, 
			vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, 
			typename vx_type0, typename vx_type1, typename vx_type2, 
			vx_type0 &input0, vx_type1 &output1, vx_type2 &output2, 
			int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
struct ImgSobel{
     ImgSobel(){
#ifdef Xilinx
     ImgSobel<SrcType, DstType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1, output2);
#elif Intel
     ihc::launch((ImgSobelIntel<SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1,stream_type2,decltype(input0),decltype(output1),decltype(output2),input0,output1,output2>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgSobelIntel<SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE,stream_type0,stream_type1,stream_type2,decltype(input0),decltype(output1),decltype(output2),input0,output1,output2>));
#endif
	 }
};
/** @brief  Computes a Box filter over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param SEPARABLE   Is true, if separable filter should be used
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, bool SEPARABLE, int stream_type0, int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1>
void ImgBoxIntel(){
	HwBox<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE, stream_type0, stream_type1>(input0, output1);
}

template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, bool SEPARABLE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgBox{
     ImgBox(){
#ifdef Xilinx
     ImgBox<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE, SEPARABLE_FILTER>(input0, output1);
#elif Intel
     ihc::launch((ImgBoxIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE_FILTER, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgBoxIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE_FILTER,stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
};

/** @brief  Convolves the input with the client supplied convolution matrix. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned, signed)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
@param conv        The custom convolution kernel
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1>
void ImgConvolveIntel(const vx_uint8 convolve_kernel[3][3]) {
	HwConvolve<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>(input0, output1, convolve_kernel);
}

template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgConvolve{
     ImgConvolve(){
			 // Convolution kernel for the HwConvolve filter
			 // example of convolutional kernel
			  const vx_uint8 convolve_kernel[3][3] = {
			  	{ 191, 185, 157 },
			  	{ 237, 171, 245 },
			  	{ 219, 235, 173 }	};

#ifdef Xilinx
     ImgConvolve<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1, convolve_kernel);
#elif Intel
     ihc::launch((ImgConvolveIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>),convolve_kernel);
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgConvolveIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>));
#endif
	 }
};

/** @brief  Implements Dilation, which grows the white space in a Boolean image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, int stream_type1, typename vx_type0, vx_type0 &input0, vx_type0 &output1>
void ImgDilateIntel() {
	HwDilate<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1>(input0, output1);
}
template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgDilate{
     ImgDilate(){
#ifdef Xilinx
     ImgDilate<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgDilateIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgDilateIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>));
#endif
	 }
};

/** @brief  Implements Erosion, which shrinks the white space in a Boolean image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, typename vx_type0, vx_type0 &input0, vx_type0 &output1>
void ImgErodeIntel() {
	HwErode<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>(input0, output1);
}
template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgErode{
     ImgErode(){
#ifdef Xilinx
     ImgErode<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgErodeIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgErodeIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1, decltype(input0), decltype(output1), input0, output1>));
#endif
	 }
};

/** @brief  Computes a Gaussian filter over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param SEPARABLE   Is true, if separable filter should be used
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE, int stream_type0, int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1>
void ImgGaussianIntel(){
	HwGaussian<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE, stream_type0, stream_type1>(input0, output1);
}
template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgGaussian{
     ImgGaussian(){
#ifdef Xilinx
     ImgGaussian<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE, SEPARABLE_FILTER>(input0, output1);
#elif Intel
     ihc::launch((ImgGaussianIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE_FILTER, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgGaussianIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE_FILTER,stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
};

/** @brief  Computes a median pixel value over a window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, typename vx_type0, vx_type0 &input0, vx_type0 &output1>
void ImgMedianIntel(){
	HwMedian<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>(input0,output1);
}
template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgMedian{
     ImgMedian(){
#ifdef Xilinx
     ImgMedian<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgMedianIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgMedianIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
};

/** @brief  Implements the Scharr Image Filter Kernel. The output images dimensions should be the same as the dimensions of the input image.
@param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit)
@param DstType     Data type of the output image (signed)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output1 for intel FPGAs
@param stream_type2 type of streamming for of Output2 for intel FPGAs
@param input       The input image
@param output1     The output image (x derivative)
@param output2     The output image (y derivative)
*/
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_border_e BORDER_TYPE, int stream_type0,int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, vx_type1 &output2>
void ImgScharr3x3Intel(){
	HwScharr3x3<SrcType, DstType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, BORDER_TYPE>(input0, output1, output2);
}
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, typename vx_type2, vx_type0 &input0, vx_type1 &output1, vx_type2 &output2, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e, int stream_type2= vx_stream_e>
struct ImgScharr3x3{
     ImgScharr3x3(){
#ifdef Xilinx
     ImgScharr3x3<SrcType, DstType, VEC_NUM, COLS_FHD, ROWS_FHD, BORDER_TYPE>(input0, output1, output2);
#elif Intel
     ihc::launch((ImgScharr3x3Intel<SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, BORDER_TYPE,stream_type0,stream_type1,stream_type2,decltype(input0),decltype(output1),decltype(output2),input0,output1,output2>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgScharr3x3Intel<SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, BORDER_TYPE,stream_type0,stream_type1,stream_type2,decltype(input0),decltype(output1),decltype(output2),input0,output1,output2>));
#endif
	 }
};


/** @brief  Computes a Non-Max suppression over a 3x3 window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1>
void ImgNonMaximaSuppressionIntel(){
 	HwNonMaximaSuppression<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, vx_type0, vx_type1>(input0, output1);
}

template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgNonMaximaSuppression{
     ImgNonMaximaSuppression(){
#ifdef Xilinx
     ImgNonMaximaSuppression<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgNonMaximaSuppressionIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0, output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgNonMaximaSuppressionIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
};


/** @brief  Computes a Non-Max suppression over a 3x3 window for canny with
 * phhse 
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param phase        type of streamming for phase data 
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type0 &input1, vx_type1 &output1>
void ImgNonMaximaSuppressionPhaseIntel(){
 	HwNonMaximaSuppressionPhase4<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, vx_type0, vx_type1>(input0, input1, output1);
}

template <typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type0 &input1, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgNonMaximaSuppressionPhase{
     ImgNonMaximaSuppressionPhase(){
#ifdef Xilinx
     ImgNonMaximaSuppressionPhase<SrcType, VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgNonMaximaSuppressionPhaseIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0, input1,output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgNonMaximaSuppressionPhaseIntel<SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,input1,output1>));
#endif
	 }
};



/** @brief  Computes a Census Transform  over a 3x3 window of the input image. The output image dimensions should be the same as the dimensions of the input image.
@param DataType    Data type of the input/output image (unsigned)(8-bit, 16-bit, 32-bit)
@param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8)
@param WIDTH       Image width
@param HEIGHT      Image height
@param KERN_SIZE   Kernel size (3, 5, 7, 9, 11)
@param BORDER_TYPE Type of border (Constant, Replicated or Undefined)
@param stream_type0 type of streamming for of Input for intel FPGAs
@param stream_type1 type of streamming for of Output for intel FPGAs
@param input       Input image
@param output      Output image
*/
template <typename InType, typename OutType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, int stream_type0, int stream_type1, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1>
void ImgCensusIntel(){
 	HwCensusTransform<InType,OutType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, vx_type0, vx_type1>(input0, output1);
}

template <typename SrcType,typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename vx_type0, typename vx_type1, vx_type0 &input0, vx_type1 &output1, int stream_type0= vx_stream_e, int stream_type1= vx_stream_e>
struct ImgCensus{
     ImgCensus(){
#ifdef Xilinx
     ImgCensus<SrcType, DstType,VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, BORDER_TYPE>(input0, output1);
#elif Intel
     ihc::launch((ImgCensusIntel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0, output1>));
#endif
	 }
     void vxReleaseNode(){
#ifdef Xilinx
#elif Intel
     ihc::collect((ImgCensusIntel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, stream_type0, stream_type1,decltype(input0),decltype(output1),input0,output1>));
#endif
	 }
};

#endif /* SRC_IMG_FILTER_BASE_H_ */
