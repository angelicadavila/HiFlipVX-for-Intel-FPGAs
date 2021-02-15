#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
#include "census.h"
/*******************************************************************************/
/*******************************************************************************/
extern "C" template component 
void ImgCensusTransform<vx_uint8, vx_uint8, VEC_NUM, COLS_FHD,ROWS_FHD, FILTER_SIZE, SOBEL_BORDER>(
DramTypeIn0 &InputImg,
DramTypeOut0 &OutputImg 
); 

/*******************************************************************************/
/*******************************************************************************/


#endif
