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

//typedef ihc::mm_master<vx_uint8, ihc::aspace<1>,
//ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
//ihc::align<64>,ihc::waitrequest<true>> DramTypeIn0; 
//typedef ihc::mm_master<vx_uint8, ihc::aspace<2>,
//ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
//ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<1>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>,ihc::waitrequest<true>> DramTypeIn0; 
typedef ihc::mm_master<vx_image<vx_uint8, VEC_NUM>, ihc::aspace<2>,
ihc::awidth<32>, ihc::dwidth<WIDTH_MEM>, ihc::latency<0>, ihc::maxburst<8>,
ihc::align<64>, ihc::waitrequest<true>> DramTypeOut0; 

//all the streamming interfaces should be global for system of task in HLS Intel.
//trying to balance the task with buffers between task
#define VEC_INT 8
//ihc::stream<vx_uint8> stream_r_0; 

//vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_0; 
vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_1; 


//vx_image<vx_uint8,VEC_INT , vx_stream_e> stream_r_0[5];
//template<void (*FUNC_PTR)(void)>
//    struct Func {};
//
//template<template <typename _T, class ... _Params> class ihc::stream >
//template< template<class TYPE, const size_t SIZE=VEC_INT> class vx_image_t> 
class exm{
  public: 
    struct vx_image_c {
		vx_uint8 pixel[VEC_INT];  
};
//    ihc::stream<vx_image_c> _a;
    vx_image<vx_uint8,VEC_INT , vx_stream_e> _a;
//    vx_image_t pixel;
    //vx_uint8 pixel[VEC_INT];
//	ihc::stream<decltype(pixel)> _a;
//    ihc::stream_in<vx_uint8> _a; 
    exm () 
	{
	};
    ~exm () 
	{
	};


};


class creat{
public:
  int size() const;
	exm& operator[] (int index);


};
    auto vxCreateVirtualImage () 
	{ 
        
	   return new exm; 	
	};


   // static exm *obj= new exm[5];
    //auto virts=vxCreateVirtualImage(obj[0]);
    //exm<vx_image_t> virts;
   creat virts;
//   virts[0] = vxCreateVirtualImage();
 
templat <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, 
	vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE	>
void ImgCopyt( DramTypeIn0 &InputImg, DramTypeOut0 &OutputImg	)
{
//    exm virts[1] = {new exm};
   // vx_image<vx_uint8, VEC_INT> data; 
   // data = stream_r_0._a.read(); 
//   auto data=virts[0]._a.read();
//    auto data=_b.read();
  // vxDramRead <SrcType, decltype(stream_r_0._a), stream_r_0._a, DramTypeIn0, VEC_NUM, 
   vxDramRead <SrcType, decltype(virts), virts, DramTypeIn0, VEC_NUM, 
//   vxDramRead <SrcType, decltype(stream_r_0._a), (stream_r_0._a), DramTypeIn0, VEC_NUM, 
				 WIDTH, HEIGHT,8> vxCopyDRAM_In0 (&InputImg); 	
//	vxDramWrite <SrcType, decltype(stream_r_0), stream_r_0, DramTypeOut0, 
//		VEC_NUM, WIDTH, HEIGHT,8> vxCopyDRAM_Out0 (&OutputImg);
 	
//	vxCopyDRAM_In0.vxReleaseNode();
//	vxCopyDRAM_Out0.vxReleaseNode();
}

#endif /* SRC_IMG_FILTER_BASE_H_ */
