#ifndef SRC_IMG_SOBEL_TEST_INTEL_H_
#define SRC_IMG_SOBEL_TEST_INTEL_H_
#include "sobel_component.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
//#include "img_filter_base.h" 
#define WIDTH_MEM 256 

#define SEPARABLE_SOBEL   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_image<vx_int8, VEC_NUM>, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 
typedef ihc::mm_master<vx_image<vx_int8, VEC_NUM>, ihc::aspace<3>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut1; 


//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 8
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_0; 
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_0_1; 
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamX_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamY_1_2;  

template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgSobelFunc( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg, DramTypeOut1 &OutputImg1	)
{
	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <SrcType, decltype(stream_r_0), stream_r_0, DramTypeIn0, VEC_NUM, 
		WIDTH, HEIGHT,32> vxCopyDRAM_In0 (&InputImg);
	//for canny filter 1-Gauus..2- Sobel 
	//Sobel Filter
	ImgSobel <SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, 
		decltype(stream_r_0), decltype(streamX_1_2), decltype(streamY_1_2),
		stream_r_0, streamX_1_2, streamY_1_2> vxSobelNode1;

	vxDramWrite <DstType, decltype(streamX_1_2), streamX_1_2, DramTypeOut0, 
		VEC_NUM, WIDTH, HEIGHT,32> vxCopyDRAM_Out0 (&OutputImg);

	vxDramWrite <DstType, decltype(streamY_1_2), streamY_1_2, DramTypeOut1, 
		VEC_NUM, WIDTH, HEIGHT,32> vxCopyDRAM_Out1 (&OutputImg1);
	
	vxSobelNode1.vxReleaseNode();
	vxCopyDRAM_In0.vxReleaseNode();
	vxCopyDRAM_Out0.vxReleaseNode();
	vxCopyDRAM_Out1.vxReleaseNode();
}

#endif /* SRC_IMG_SOBEL_BASE_H_ */
