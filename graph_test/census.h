#ifndef SRC_IMG_CENSUS_TEST_INTEL_H_
#define SRC_IMG_CENSUS_TEST_INTEL_H_
#include "component_intel.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
#include "img_other_base.h" 
//#include "img_filter_base.h" 
 #define WIDTH_MEM 256 

#define SEPARABLE_FILTER   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>, ihc::align<64>, ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 
//typedef ihc::mm_master<vx_distribution<vx_uint32, VEC_NUM>, ihc::aspace<2>,
//ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 
//

//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 8
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_0; 
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamX_1_2;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamY_1_2;  
//
vx_image<vx_int8 ,VEC_INT , vx_stream_e> streamMag_2_3;  

vx_image<vx_uint8 ,VEC_INT , vx_stream_e> streamCT_3_4;  
vx_distribution<vx_uint32 ,1, vx_stream_e> streamHST_4_5;  
vx_image<vx_int8 ,VEC_INT , vx_stream_e> result; 


template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgCensusTransform( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg	)
{
	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <SrcType, decltype(stream_r_0), stream_r_0, DramTypeIn0, VEC_NUM, 
		WIDTH, HEIGHT,8> vxCopyDRAM_In0 (&InputImg);
	//Sobel Filter
	ImgSobel <SrcType, vx_int8,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, 
		decltype(stream_r_0), decltype(streamX_1_2), decltype(streamY_1_2),
		stream_r_0, streamX_1_2, streamY_1_2> vxSobelNode1;

	//Magnitud Node
	ImgMagnitude <vx_int8, VEC_NUM, PIXELS_FHD, VX_ROUND_POLICY_TO_NEAREST_EVEN,
		decltype(streamX_1_2), decltype(streamY_1_2), decltype(streamMag_2_3), 
		streamX_1_2, streamY_1_2, streamMag_2_3>vxMagnitudNode2;

	//census Transform
	ImgCensus <vx_int8, SrcType, VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, 
		BORDER_TYPE, decltype(streamMag_2_3), decltype(streamCT_3_4), 
		streamMag_2_3, streamCT_3_4> vxCensusNode3;

//	ImgHistogram <SrcType, VEC_NUM, PIXELS_FHD, HIST_BINS, HIST_RANGE, HIST_OFFSET, 
//		decltype(streamCT_3_4), decltype(streamHST_4_5), 
//		streamCT_3_4, streamHST_4_5> vxHistogramNode3;
//
//	vxDistributionDramWrite <vx_uint32, decltype(streamHST_4_5), streamHST_4_5, DramTypeOut0, 
//		1, HIST_BINS> vxCopyDRAM_Out0 (&OutputImg);

	vxDramWrite <vx_uint8, decltype(streamCT_3_4), streamCT_3_4, DramTypeOut0, 
		VEC_NUM, WIDTH, HEIGHT> vxCopyDRAM_Out0 (&OutputImg);
	
	vxSobelNode1.vxReleaseNode();
	vxMagnitudNode2.vxReleaseNode();
	vxCensusNode3.vxReleaseNode();
//	vxCopyDRAM_In0.vxReleaseNode();
//	vxCopyDRAM_Out0.vxReleaseNode();
}


#endif /* SRC_IMG_FILTER_BASE_H_ */
