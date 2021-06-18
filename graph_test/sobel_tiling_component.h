#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
#include "sobel_tiling.h"

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
	DramTypeIn00 &Input00, DramTypeOut00 &Output00, DramTypeOut10 &Output10, 
	DramTypeIn01 &Input01, DramTypeOut01 &Output01, DramTypeOut11 &Output11, 
const uint rows
); 
/*******************************************************************************/
/*******************************************************************************/

#endif
