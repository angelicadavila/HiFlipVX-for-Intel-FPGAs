#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
//#include "canny.h"
//#include "census.h"
#include "autoc.h"

//component instance
//template component 
//void ImgSobel<vx_uint8,vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER, vx_streamIn_e,vx_streamOut_e, vx_streamOut_e> 
//	(_vx_in(vx_image<vx_uint8, VEC_NUM,vx_streamIn_e>) input0, 
//	 _vx_out(vx_image<vx_int8, VEC_NUM,vx_streamOut_e>) output1,
//	 _vx_out(vx_image<vx_int8, VEC_NUM,vx_streamOut_e>) output2); 


/*******************************************************************************/
/*******************************************************************************/
//extern "C" template component 
//void ImgCanny<vx_uint8, vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
//DramTypeIn0 &InputImg,
//DramTypeOut0 &OutputImg 
////DramTypeOut1 &OutputImg1 
//); 
/*******************************************************************************/
/*******************************************************************************/
extern "C" template component 
void ImgAutoContrast<vx_uint8, vx_uint8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
DramTypeIn0 &InputImg,
DramTypeOut0 &OutputImg 
); 

/*******************************************************************************/
/*******************************************************************************/
//extern "C" template component 
//void ImgCensusTransform<vx_uint8, vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
//DramTypeIn0 &InputImg,
//DramTypeOut0 &OutputImg 
//); 

/*******************************************************************************/
/*******************************************************************************/

//template component void ImgMedian<vx_uint8,VEC_NUM, COLS_FHD, ROWS_FHD, FILTER_SIZE, MEDIAN_BORDER, stream_in, stream_out>
//	(_vx_in(vx_image<vx_uint8, VEC_NUM,stream_in>) input0, 
//	 _vx_out(vx_image<vx_uint8, VEC_NUM,stream_out>) output1); 


//
//template component
//void HwBox<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output);
//}
//
//
//template component
//void
//		
//	HwConvolve<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output, conv);
//}
//
//
//template component
//void
//		
//	HwDilate<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output);
//}
//
//
//template component
//void
//		
//	HwErode<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output);
//}
//
//
//template component
//void
//		
//	HwGaussian<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output);
//}
//
//
//template component
//void
//		
//	HwMedian<vx_uint8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<vx_uint8, VEC_NUM> *)input, (vx_image<vx_uint8, VEC_NUM> *)output);
//}
//
//
//template component
//void
//		
//	HwScharr3x3<SrcType, vx_int8, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, BORDER_TYPE>((vx_image<SrcType, VEC_NUM> *)input, (vx_image<vx_int8, VEC_NUM> *)output1, (vx_image<vx_int8, VEC_NUM> *)output2);
//}
//

#endif
