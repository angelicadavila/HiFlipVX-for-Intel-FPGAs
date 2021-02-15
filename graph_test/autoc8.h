#ifndef SRC_IMG_FILTER_TEST_INTEL_H_
#define SRC_IMG_FILTER_TEST_INTEL_H_
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

typedef ihc::mm_master<vx_uint32, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>,ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_uint32, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 
//typedef ihc::mm_master<vx_image<vx_uint32, VEC_NUM>, ihc::aspace<1>,
//ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
//ihc::align<64>,ihc::waitrequest<true>> DramTypeIn0; 
//typedef ihc::mm_master<vx_image<vx_uint32, VEC_NUM>, ihc::aspace<2>,
//ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
//ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 

//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 1
ihc::stream<vx_uint32> stream_r_0; 

ihc::stream<vx_uint32> streamCC_1_2;  //YUV output
//seprating Channels

ihc::stream<vx_uint32> streamCC0_1_2;  //Color convert output
ihc::stream<vx_uint32> streamCC1_1_2;  //Color Convert output
ihc::stream<vx_uint32> streamCC2_1_2;  //Color Convert output
ihc::stream<vx_uint32> streamCC3_1_2;  //Color Convert output

ihc::stream<vx_uint8 > streamY_2_3;  //Y output
ihc::stream<vx_uint8 > streamU_2_4;  //U output
ihc::stream<vx_uint8 > streamV_2_4;  //U output
ihc::stream<vx_uint8 > streamempty_2_4;  //YUV output
//
ihc::stream<vx_uint8 > streamEH_3_4;  //Equalize output
ihc::stream<vx_uint32> streamCC_4_5;  //Combine output
ihc::stream<vx_uint32> streamCC_5_6;  //Combine output


template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgAutocontrast( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg	)
{
	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <vx_uint32, decltype(stream_r_0), stream_r_0, DramTypeIn0, VEC_NUM, 
		WIDTH, HEIGHT,8> vxCopyDRAM_In0 (&InputImg);
	
	ImgConvertColor <vx_uint32, vx_uint32, VEC_NUM, PIXELS_FHD,VX_DF_IMAGE_RGB, 
		VX_DF_IMAGE_NV12, decltype(stream_r_0), decltype(streamCC_1_2), 
		stream_r_0, streamCC_1_2> vxColorNV12Node1;

	 //extract the three channels YUV
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(streamCC_1_2), 
		streamCC_1_2, streamCC0_1_2, streamCC1_1_2> splitStream0;
	vxSplitStream <vx_uint32, VEC_NUM, WIDTH, HEIGHT, decltype(streamCC_1_2), 
		streamCC0_1_2, streamCC2_1_2, streamCC3_1_2> splitStream1;

	ImgChannelExtract <vx_uint32, EXTRACT_CHANNELY, PIXELS_FHD,
		VX_DF_IMAGE_NV12, decltype(streamCC_1_2), decltype(streamY_2_3), 
		streamCC1_1_2, streamY_2_3> vxChnExtractNode2;
	ImgChannelExtract <vx_uint32, EXTRACT_CHANNELU, PIXELS_FHD,
		VX_DF_IMAGE_NV12, decltype(streamCC_1_2), decltype(streamY_2_3), 
		streamCC2_1_2, streamU_2_4> vxChnExtractNode3;
	ImgChannelExtract <vx_uint32, EXTRACT_CHANNELV, PIXELS_FHD,
		VX_DF_IMAGE_NV12, decltype(streamCC_1_2), decltype(streamY_2_3), 
		streamCC3_1_2, streamV_2_4> vxChnExtractNode4;

    ImgEqualizeHist <vx_uint8, VEC_NUM, PIXELS_FHD, HIST_BINS, HIST_RANGE,	
		HIST_OFFSET, decltype(streamY_2_3), decltype(streamEH_3_4), 
		streamY_2_3, streamEH_3_4> vxHistogramNode5;

	ImgChannelCombine <vx_uint32, COMB_CHANNEL, PIXELS_FHD,
		VX_DF_IMAGE_NV12, decltype(streamY_2_3), decltype(streamCC_4_5), 
		streamEH_3_4, streamU_2_4, streamV_2_4, streamempty_2_4, streamCC_4_5>
		vxChCombinetNode6;
  
	  ImgConvertColor <vx_uint32, vx_uint32, VEC_NUM, PIXELS_FHD, VX_DF_IMAGE_NV12, 
  		VX_DF_IMAGE_RGB, decltype(streamCC_1_2), decltype(streamCC_5_6), 
  		streamCC_4_5, streamCC_5_6> vxColorRGBNode1;

	vxDramWrite <vx_uint32, decltype(streamCC_5_6), streamCC_5_6, DramTypeOut0, 
		VEC_NUM, WIDTH, HEIGHT,8> vxCopyDRAM_Out0 (&OutputImg);		
	//return results
   	
//	vxDramWrite <vx_uint32, decltype(stream_r_0), stream_r_0, DramTypeOut0, 
//		VEC_NUM, WIDTH, HEIGHT,8> vxCopyDRAM_Out0 (&OutputImg);		
	vxColorNV12Node1.vxReleaseNode();
	splitStream0.vxReleaseNode();
	splitStream1.vxReleaseNode();
	vxChnExtractNode2.vxReleaseNode();
	vxChnExtractNode3.vxReleaseNode();
	vxChnExtractNode4.vxReleaseNode();
	vxHistogramNode5.vxReleaseNode();
	vxChCombinetNode6.vxReleaseNode();
	vxColorRGBNode1.vxReleaseNode();
	vxCopyDRAM_In0.vxReleaseNode();
	vxCopyDRAM_Out0.vxReleaseNode();
}

#endif /* SRC_IMG_FILTER_BASE_H_ */
