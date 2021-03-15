#ifndef SRC_IMG_COMPONENT_INTEL_H_
#define SRC_IMG_COMPONENT_INTEL_H_
#include "skinTone.h"


/*******************************************************************************/
/*******************************************************************************/
extern "C" template component 
void skinTone<vx_uint8, vx_int8, VEC_NUM, COLS_FHD,ROWS_FHD>(
DramTypeIn0 &InputImg,
DramTypeOut0 &OutputImg 
); 
/*******************************************************************************/
/*******************************************************************************/

#endif
