#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
#include "autoc8.h"

//component instance
/*******************************************************************************/
/*******************************************************************************/
extern "C" template component 
void ImgAutocontrast<vx_uint32, vx_uint32, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
DramTypeIn0 &InputImg,
DramTypeOut0 &OutputImg 
//DramTypeOut1 &OutputImg1 
); 
/*******************************************************************************/
/*******************************************************************************/

#endif
