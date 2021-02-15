#ifndef SRC_IMG_FILTER_TEST_INTEL_H_
#define SRC_IMG_FILTER_TEST_INTEL_H_
#include "canny_component.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
//#include "img_filter_base.h" 
#define WIDTH_MEM 256
#define SEPARABLE_FILTER   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>,ihc::waitrequest<true>> DramTypeIn0; 

typedef ihc::mm_master<vx_image<vx_int8, VEC_NUM>, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 


//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 8
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_0; 
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_0_1; 
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamX_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamY_1_2;  
//copies of Magnitud and Phase
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamX0_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamY0_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamX1_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamY1_1_2;  
//
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamMag_2_3;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamPha_2_3;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamNMS_3_4;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> result; 

template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgCanny( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg	)
{
	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <SrcType, decltype(stream_r_0), stream_r_0, DramTypeIn0, VEC_NUM, 
		WIDTH, HEIGHT,8> vxCopyDRAM_In0 (&InputImg);
	//for canny filter 1-Gauus..2- Sobel 
	ImgGaussian <SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, 
		SEPARABLE_FILTER, decltype(stream_r_0), decltype(stream_0_1), 
		stream_r_0, stream_0_1> vxGaussianNode0;
	//Sobel Filter
	ImgSobel <SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, 
		decltype(stream_0_1), decltype(streamX_1_2), decltype(streamY_1_2),
		stream_0_1, streamX_1_2, streamY_1_2> vxSobelNode1;

	//Split output for Magnitud and Phase
	vxSplitStream <vx_int8, VEC_NUM, WIDTH, HEIGHT, decltype(streamX_1_2), 
		streamX_1_2, streamX0_1_2, streamX1_1_2> splitStream0;
	vxSplitStream <vx_int8, VEC_NUM, WIDTH, HEIGHT, decltype(streamY_1_2), 
		streamY_1_2, streamY0_1_2, streamY1_1_2> splitStream1;
		
	//Magnitud Node
	ImgMagnitude <DstType, VEC_NUM, PIXELS_FHD, VX_ROUND_POLICY_TO_NEAREST_EVEN,
		decltype(streamX0_1_2), decltype(streamY0_1_2), decltype(streamMag_2_3), 
		streamX0_1_2, streamY0_1_2, streamMag_2_3>vxMagnitudNode2;
	//Phase Node
	ImgPhase4 <DstType, VEC_NUM, PIXELS_FHD, VX_ROUND_POLICY_TO_NEAREST_EVEN,
		decltype(streamX1_1_2), decltype(streamY_1_2), decltype(streamPha_2_3), 
		streamX1_1_2, streamY1_1_2, streamPha_2_3>vxPhaseNode2;

	//Non-max suppression
	ImgNonMaximaSuppressionPhase <DstType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, 
		BORDER_TYPE, decltype(streamMag_2_3), decltype(streamNMS_3_4), 
		streamMag_2_3, streamPha_2_3, streamNMS_3_4> vxNonMaximaSuppressionNode3;
	//return results
	vxDramWrite <DstType, decltype(streamNMS_3_4), streamNMS_3_4, DramTypeOut0, 
		VEC_NUM, WIDTH, HEIGHT> vxCopyDRAM_Out0 (&OutputImg);
	
	vxGaussianNode0.vxReleaseNode();
	vxSobelNode1.vxReleaseNode();
	vxMagnitudNode2.vxReleaseNode();
	vxPhaseNode2.vxReleaseNode();
	vxNonMaximaSuppressionNode3.vxReleaseNode();
	vxCopyDRAM_In0.vxReleaseNode();
	vxCopyDRAM_Out0.vxReleaseNode();
}

#endif /* SRC_IMG_FILTER_BASE_H_ */
