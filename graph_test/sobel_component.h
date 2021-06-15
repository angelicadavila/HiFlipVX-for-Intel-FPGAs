#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
#include "sobel.h"

//component instance
//template component 
//void ImgSobel<vx_uint8,vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER, vx_streamIn_e,vx_streamOut_e, vx_streamOut_e> 
//	(_vx_in(vx_image<vx_uint8, VEC_NUM,vx_streamIn_e>) input0, 
//	 _vx_out(vx_image<vx_int8, VEC_NUM,vx_streamOut_e>) output1,
//	 _vx_out(vx_image<vx_int8, VEC_NUM,vx_streamOut_e>) output2); 


/*******************************************************************************/
/*******************************************************************************/
extern "C" template component 
void ImgSobelFunc<vx_uint8, vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
DramTypeIn0 &InputImg,
DramTypeOut0 &OutputImg, 
DramTypeOut1 &OutputImg1,
const uint rows
); 
/*******************************************************************************/
/*******************************************************************************/

#endif
