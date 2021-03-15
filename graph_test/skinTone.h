#ifndef SRC_IMG_SOBEL_TEST_INTEL_H_
#define SRC_IMG_SOBEL_TEST_INTEL_H_
#include "skinTone_component.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
#include "img_other_base.h" 
//#include "vx_threshold.c"
//#include "img_filter_base.h" 
#define WIDTH_MEM 256 

#define SEPARABLE_SOBEL   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )
typedef ihc::mm_master<vx_uint32, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_uint32, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 


//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 1
ihc::stream<vx_uint32> stream_r_0; 
ihc::stream<vx_uint32> stream_0_1; 
ihc::stream<vx_uint32> stream_0_2; 
ihc::stream<vx_uint32> stream_0_3; 
ihc::stream<vx_uint32> stream_0_4; 

//Virtual image for RGB channel extraction
ihc::stream<vx_uint8 > streamR_2;  //R output
ihc::stream<vx_uint8 > streamG_2;  //G output
ihc::stream<vx_uint8 > streamB_2;  //B output
//subtraction node
ihc::stream<vx_uint8 > streamRCC0_2, streamRCC1_2;  //R copies
ihc::stream<vx_uint8 > streamGCC0_2, streamGCC1_2;  //G copies
ihc::stream<vx_uint8 > streamBCC0_2, streamBCC1_2;  //G copies
ihc::stream<vx_uint8 > streamRmG_3;  //R-G
ihc::stream<vx_uint8 > streamRmB_3;  //R-B 
//

template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT>
void skinTone( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg)
{

	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <vx_uint32, decltype(stream_r_0), stream_r_0, DramTypeIn0, 1, 
		WIDTH, HEIGHT,16> vxCopyDRAM_In0 (&InputImg);

	//copy the imput stream for 3 
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(stream_r_0), 
		stream_r_0, stream_0_1, stream_0_2> splitStream1;
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(stream_0_1), 
		stream_0_1, stream_0_3, stream_0_4> splitStream2;
	
	//Extract RGB channels
	ImgChannelExtract <vx_uint32, VX_CHANNEL_R, PIXELS_FHD,
		VX_DF_IMAGE_RGB, decltype(stream_0_2), decltype(streamR_2), 
		stream_0_2, streamR_2> vxChnExtractNode2;
	ImgChannelExtract <vx_uint32, VX_CHANNEL_G, PIXELS_FHD,
		VX_DF_IMAGE_RGB, decltype(stream_0_3), decltype(streamG_2), 
		stream_0_3, streamG_2> vxChnExtractNode3;
	ImgChannelExtract <vx_uint32, VX_CHANNEL_B, PIXELS_FHD,
		VX_DF_IMAGE_RGB, decltype(stream_0_4), decltype(streamB_2), 
		stream_0_4, streamB_2> vxChnExtractNode4;

    //stream R-G-B copies
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(streamR_2), 
		streamR_2, streamRCC0_2, streamRCC1_2> splitStreamR;
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(streamG_2), 
		streamG_2, streamGCC0_2, streamGCC1_2> splitStreamG;
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(streamB_2), 
		streamB_2, streamBCC0_2, streamBCC1_2> splitStreamB;
	// subtract R - G
	vxSubtract<vx_uint8, VEC_INT, PIXELS_FHD, VX_CONVERT_POLICY_SATURATE,
		decltype(streamRCC0_2), decltype(streamRmG_3), streamRCC0_2, streamGCC0_2, 
		streamRmG_3> vxSubtractNode0;
	// subtract R - B
	vxSubtract<vx_uint8, VEC_INT, PIXELS_FHD, VX_CONVERT_POLICY_SATURATE,
		decltype(streamRCC1_2), decltype(streamRmB_3), streamRCC1_2, streamBCC0_2, 
		streamRmB_3> vxSubtractNode1;
	
	//apply threshold
  	vx_uint8 thresValue95 = 95;
	vx_threshold<vx_uint8> thresh95;
	thresh95.vxCreateThreshold(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
    thresh95.vxSetThresholdAttribute(VX_THRESHOLD_THRESHOLD_VALUE, thresValue95, sizeof(vx_uint8));
	
  	vx_uint8 thresValue40 = 40;
	vx_threshold<vx_uint8> thresh40;
	thresh40.vxCreateThreshold(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
    thresh95.vxSetThresholdAttribute(VX_THRESHOLD_THRESHOLD_VALUE, thresValue40, sizeof(vx_uint8));

  	vx_uint8 thresValue20 = 20;
	vx_threshold<vx_uint8> thresh20;
	thresh20.vxCreateThreshold(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
    thresh20.vxSetThresholdAttribute(VX_THRESHOLD_THRESHOLD_VALUE, thresValue20, sizeof(vx_uint8));
	
  	vx_uint8 thresValue15 = 15;
	vx_threshold<vx_uint8> thresh15;
	thresh15.vxCreateThreshold(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
    thresh15.vxSetThresholdAttribute(VX_THRESHOLD_THRESHOLD_VALUE, thresValue15, sizeof(vx_uint8));
//	vxDramWrite <DstType, decltype(streamX_1_2), streamX_1_2, DramTypeOut0, 
//		VEC_NUM, WIDTH, HEIGHT,32> vxCopyDRAM_Out0 (&OutputImg);
//
//	vxSobelNode1.vxReleaseNode();
//	vxCopyDRAM_In0.vxReleaseNode();
//	vxCopyDRAM_Out0.vxReleaseNode();
}

#endif /* SRC_IMG_SOBEL_BASE_H_ */
