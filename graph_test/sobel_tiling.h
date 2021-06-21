#ifndef SRC_IMG_SOBEL_TEST_INTEL_H_
#define SRC_IMG_SOBEL_TEST_INTEL_H_
#include "sobel_tiling_component.h"
#include "img_filter_base_intel.h" 
#include "img_pixelop_base.h" 
//#include "img_filter_base.h" 
#define WIDTH_MEM 256 
#define TILES 2
#define VEC_INT 8

#define SEPARABLE_SOBEL   (vx_bool)vx_true_e

/** @brief  Avalon MM to connect with DRAM \n */
//latency 0 indacates a variable latency
//waitrequest is necessary when the system uses burst_size > 1
//aspace memory banks
//awidth address bus bits (from BSP file: Board_spec.xml )


//Create
typedef vxCreateImage <vx_uint8, WIDTH_MEM, 1, VEC_NUM> DramTypeIn00; 
typedef vxCreateImage <vx_int8, WIDTH_MEM, 2, VEC_NUM> DramTypeOut00; 
typedef vxCreateImage <vx_int8, WIDTH_MEM, 3, VEC_NUM> DramTypeOut10; 

typedef vxCreateImage <vx_uint8, WIDTH_MEM, 4, VEC_NUM> DramTypeIn01; 
typedef vxCreateImage <vx_int8, WIDTH_MEM, 5, VEC_NUM> DramTypeOut01; 
typedef vxCreateImage <vx_int8, WIDTH_MEM, 6, VEC_NUM> DramTypeOut11; 

//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task

typedef vx_image<vx_uint8,VEC_INT, vx_stream_e> vx_image_uint8;
typedef vx_image<vx_int8,VEC_INT, vx_stream_e> vx_image_int8;

// stream_sourceNode_DstNode_Tile
vx_image_uint8 stream_r_0_0,stream_r_0_1;   
vx_image_int8 streamX_1_2_0,streamX_1_2_1;    
vx_image_int8 streamY_1_2_0,streamY_1_2_1;    

//Sobell object for multiple instances of the filter in tiling mode
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, typename DramTypeIn,
	typename DramTypeOut0, typename DramTypeOut1,//Memory inpur ports
	vx_image_uint8 &stream_r_0, vx_image_int8 &streamX_1_2, vx_image_int8 &streamY_1_2
>
struct vxSobel
{
	vxSobel(DramTypeIn &InputImg, DramTypeOut0 &OutputImg, DramTypeOut1 &OutputImg1){

		vxDramRead <SrcType, decltype(stream_r_0), stream_r_0, DramTypeIn, VEC_NUM, 
		WIDTH, HEIGHT,8> vxCopyDRAM_In0 (HEIGHT, &InputImg);
		//Sobel Filter
		ImgSobel <SrcType, DstType,VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, 
		decltype(stream_r_0), decltype(streamX_1_2), decltype(streamY_1_2),
		stream_r_0, streamX_1_2, streamY_1_2> vxSobelNode1;

		vxDramWrite <DstType, decltype(streamX_1_2), streamX_1_2, DramTypeOut0, 
		VEC_NUM, WIDTH, HEIGHT,8> vxCopyDRAM_Out0 (&OutputImg);

		vxDramWrite <DstType, decltype(streamY_1_2), streamY_1_2, DramTypeOut1, 
		VEC_NUM, WIDTH, HEIGHT,8> vxCopyDRAM_Out1 (&OutputImg1);
	
//	vxSobelNode1.vxReleaseNode();
//	vxCopyDRAM_In0.vxReleaseNode();
//	vxCopyDRAM_Out0.vxReleaseNode();
//	vxCopyDRAM_Out1.vxReleaseNode();

	}
};
// varname_InputNumber_Tile
template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgSobelFunc( 
	DramTypeIn00 & __restrict Input00, DramTypeOut00 &Output00, DramTypeOut10 &Output10, 
	DramTypeIn01 &Input01, DramTypeOut01 &Output01, DramTypeOut11 &Output11 
)
{
	// Tiling the number of rows in image processing
	#if TILES >= 1
	vxSobel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT/TILES, KERN_SIZE, BORDER_TYPE,
			DramTypeIn00, DramTypeOut00, DramTypeOut10, 
			stream_r_0_0, streamX_1_2_0, streamY_1_2_0 >
			vxSobelTile0(Input00, Output00, Output10);
	#endif

	#if TILES >= 2
	vxSobel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT/TILES, KERN_SIZE, BORDER_TYPE,
			DramTypeIn01, DramTypeOut01, DramTypeOut11,
			stream_r_0_1, streamX_1_2_1, streamY_1_2_1 >
			vxSobelTile1(Input01, Output01, Output11);
	#endif

//	#if TILES >= 3
//	vxSobel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT/TILES, KERN_SIZE, BORDER_TYPE>
//			(&InputImg00, &OutputImg00, &OutputImg10)(Input00, Output00, Output10)
//			vxSobelTile2(Input02, Output02, Output12);
//	#endif
//
//	#if TILES >= 4
//	vxSobel<SrcType, DstType, VEC_NUM, WIDTH, HEIGHT/TILES, KERN_SIZE, BORDER_TYPE>
//			(&InputImg00, &OutputImg00, &OutputImg10)(Input00, Output00, Output10)
//			vxSobelTile3(Input00, Output00, Output01);
//	#endif
}


#endif /* SRC_IMG_SOBEL_BASE_H_ */
