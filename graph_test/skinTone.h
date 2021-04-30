#ifndef SRC_IMG_SOBEL_TEST_INTEL_H_
#define SRC_IMG_SOBEL_TEST_INTEL_H_
#include "skinTone_component.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
#include "img_other_base.h" 
//#include "vx_threshold.c"
//#include "img_filter_base.h" 
#define WIDTH_MEM 512 

#define SEPARABLE_SOBEL   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )
typedef ihc::mm_master<vx_uint32, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_uint8, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<16>, ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 


//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 1
vx_image<vx_uint32,VEC_INT, vx_stream_e> stream_r_0; 
vx_image<vx_uint32,VEC_INT, vx_stream_e>stream_0_1; 
vx_image<vx_uint32,VEC_INT, vx_stream_e>stream_0_2; 
vx_image<vx_uint32,VEC_INT, vx_stream_e>stream_0_3; 
vx_image<vx_uint32,VEC_INT, vx_stream_e>stream_0_4; 

//Virtual image for RGB channel extraction
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamR_2, streamR_3;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamG_2;  //G output
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamB_2;  //B output

//subtraction node
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamRCC0_2,streamRCC0_3, streamRCC1_3;  //R copies
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamGCC0_2, streamGCC1_2;  //G copies
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamBCC0_2, streamBCC1_2;  //G copies
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamRmG_3;  //R-G
vx_image<vx_uint8,VEC_INT, vx_stream_e>streamRmB_3;  //R-B 

//
vx_image<vx_uint8,VEC_INT, vx_stream_e> streamRT_3;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e> streamGT_3;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e> streamBT_3;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e> streamRmGT_3;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e> streamRmBT_3;  //R output


vx_image<vx_uint8,VEC_INT, vx_stream_e>  streamRaG, streamRaGaB;  //R output
vx_image<vx_uint8,VEC_INT, vx_stream_e>  streamRmGaRmB, streamSkin;  //R output
//  All the values for threshold object need to be include in the template arg.
//	thresh95.vxCreateThreshold(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//    thresh95.vxSetThresholdAttribute(VX_THRESHOLD_THRESHOLD_VALUE, thresValue95, sizeof(vx_uint8));
  	const vx_uint8 thresValue95 = 95;
	extern const vx_threshold<vx_uint8, VX_THRESHOLD_TYPE_BINARY, thresValue95> thresh95;

  	const vx_uint8 thresValue40 = 40;
	const vx_threshold<vx_uint8, VX_THRESHOLD_TYPE_BINARY, thresValue40> thresh40;

  	const vx_uint8 thresValue20 = 0;
	const vx_threshold<vx_uint8, VX_THRESHOLD_TYPE_BINARY, thresValue20> thresh20;
	
  	const vx_uint8 thresValue15 = 15;
	vx_threshold<vx_uint8, VX_THRESHOLD_TYPE_BINARY, thresValue15> thresh15;

  	const vx_uint8 thresValue0 = 0;
	vx_threshold<vx_uint8, VX_THRESHOLD_TYPE_BINARY, thresValue0> thresh0;


template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT>
void skinTone( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg)
{

	//Is it doing prefetching? buff->trying to improve the buffer.
	vxDramRead <vx_uint32, decltype(stream_r_0), stream_r_0, DramTypeIn0, 1, 
		WIDTH, HEIGHT,4> vxCopyDRAM_In0 (&InputImg);

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
	vxSplitStream <vx_uint8, VEC_INT, WIDTH, HEIGHT, decltype(streamR_2), 
		streamR_2, streamR_3, streamRCC0_2> splitStreamR;
	vxSplitStream <vx_uint8, VEC_INT, WIDTH, HEIGHT, decltype(streamR_2), 
		streamR_3, streamRCC0_3, streamRCC1_3> splitStreamR2;

	vxSplitStream <vx_uint8, VEC_INT, WIDTH, HEIGHT, decltype(streamG_2), 
		streamG_2, streamGCC0_2, streamGCC1_2> splitStreamG;
	vxSplitStream <vx_uint8, VEC_INT, WIDTH, HEIGHT, decltype(streamB_2), 
		streamB_2, streamBCC0_2, streamBCC1_2> splitStreamB;

	// subtract R - G
	vxSubtract<vx_uint8, VEC_INT, PIXELS_FHD, VX_CONVERT_POLICY_SATURATE,
		decltype(streamRCC0_3), decltype(streamRmG_3), streamRCC0_3, streamGCC0_2, 
		streamRmG_3> vxSubtractNode0;
	// subtract R - B
	vxSubtract<vx_uint8, VEC_INT, PIXELS_FHD, VX_CONVERT_POLICY_SATURATE,
		decltype(streamRCC1_3), decltype(streamRmB_3), streamRCC1_3, streamBCC0_2, 
		streamRmB_3> vxSubtractNode1;
	
	//apply threshold
    vxThresholdNode<vx_uint8, PIXELS_FHD, VEC_INT, decltype(thresh95), decltype(streamRCC0_2),
		decltype(streamRT_3),streamRCC0_2, streamRT_3>vxThreshR (thresh95);
    vxThresholdNode<vx_uint8, PIXELS_FHD, VEC_INT, decltype(thresh40), decltype(streamGCC1_2),
		decltype(streamRT_3),streamGCC1_2, streamGT_3>vxThreshG (thresh40);
    vxThresholdNode<vx_uint8, PIXELS_FHD, VEC_INT, decltype(thresh20), decltype(streamBCC1_2),
		decltype(streamRT_3),streamBCC1_2, streamBT_3>vxThreshB (thresh20);
	//thresh for substraction pixels
    vxThresholdNode<vx_uint8, PIXELS_FHD, VEC_INT, decltype(thresh15), decltype(streamRmG_3),
		decltype(streamRT_3),streamRmG_3, streamRmGT_3>vxThreshRmG (thresh15);
    vxThresholdNode<vx_uint8, PIXELS_FHD, VEC_INT, decltype(thresh0), decltype(streamRmB_3),
		decltype(streamRT_3),streamRmB_3, streamRmBT_3>vxThreshRmB (thresh0);

	//aply And to all thresh output	
	vxAndNode<vx_uint8, VEC_INT, PIXELS_FHD, decltype(streamRT_3), decltype(streamGT_3),
		decltype(streamRaG), streamRT_3, streamGT_3,streamRaG> vxAnd1;		
	
	vxAndNode<vx_uint8, VEC_INT, PIXELS_FHD, decltype(streamRaG), decltype(streamBT_3),
		decltype(streamRaGaB), streamRaG, streamBT_3,streamRaGaB> vxAnd2;		

	vxAndNode<vx_uint8, VEC_INT, PIXELS_FHD, decltype(streamRmGT_3), decltype(streamRmBT_3),
		decltype(streamRmGaRmB), streamRmGT_3, streamRmBT_3,streamRmGaRmB> vxAnd3;		

	vxAndNode<vx_uint8, VEC_INT, PIXELS_FHD, decltype(streamRaGaB), decltype(streamRmGaRmB),
		decltype(streamSkin), streamRaGaB, streamRmGaRmB,streamSkin> vxAnd4;		

	vxDramWrite <vx_uint8, decltype(streamSkin), streamSkin, DramTypeOut0, 
		VEC_INT, WIDTH, HEIGHT,8> vxCopyDRAM_Out0 (&OutputImg);
	//vxDramWrite <vx_uint8, decltype(streamSkin), streamSkin, DramTypeOut0, 
	//	VEC_INT, WIDTH, HEIGHT,32> vxCopyDRAM_Out0 (&OutputImg);

	vxCopyDRAM_In0.vxReleaseNode();
	splitStream1.vxReleaseNode(); 
	splitStream2.vxReleaseNode(); 
	vxChnExtractNode2.vxReleaseNode(); 
	vxChnExtractNode3.vxReleaseNode(); 
	vxChnExtractNode4.vxReleaseNode(); 
	splitStreamR.vxReleaseNode(); 
	splitStreamR2.vxReleaseNode(); 
	splitStreamG.vxReleaseNode(); 
	splitStreamB.vxReleaseNode(); 
	vxSubtractNode0.vxReleaseNode(); 
	vxSubtractNode1.vxReleaseNode(); 
	vxThreshR.vxReleaseNode(); 
	//vxThreshG.vxReleaseNode(); 
	//vxThreshB.vxReleaseNode(); 
	vxAnd1.vxReleaseNode(); 
	vxAnd2.vxReleaseNode(); 
	vxAnd3.vxReleaseNode(); 
	vxAnd4.vxReleaseNode(); 
	vxCopyDRAM_Out0.vxReleaseNode();
}

#endif /* SRC_IMG_SOBEL_BASE_H_ */
