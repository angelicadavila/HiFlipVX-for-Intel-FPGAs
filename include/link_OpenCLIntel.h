#ifndef SRC_IMG_LINK_OPENCL_INTEL_H_ 
#define SRC_IMG_LINK_OPENCL_INTEL_H_ 
#include "definitions.h"

/*********************************************************************************************************************/
/** @brief	Transfer data from openCL Avalon Memory Master interface to streaming interface
 * @param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit) 
 * @param StreamType  Data type of the streamming where dara from DRAM are write
 * @param stream_out  Stream object where dara from DRAM are write
 * @param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8) 
 * @param WIDTH       Image width  
 * @param HEIGHT      Image height 
 * @param COALESCED   size of the burst petition to DRAM
 * */
using namespace ihc;  
////forcing a Burst Coalesced LSU/
using burst_coalesced = lsu<style<BURST_COALESCED>>;
template<typename SrcType, typename StreamType, StreamType &stream_out,typename
DramTypeIn, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint COALESCED,
enable_if_t<!std::is_same<SrcType, vx_uint32>::value,int> = 0 >
void vxReadDram(DramTypeIn *dram_in){
// verifies the data type in src, template specialization is not currently
// available o`n c++

			vx_image<SrcType, VEC_NUM> data[COALESCED];
			auto aux_dram=&(*dram_in[0]);
			using burst_coalesced =ihc::lsu<ihc::style<ihc::BURST_COALESCED>>;
			#pragma ii 1
			for (auto i = 0; i < WIDTH * HEIGHT/(VEC_NUM*COALESCED); i++){
		//	First version		
				UNROLL_INTEL()
				for (vx_uint8 j = 0; j < COALESCED; ++j) 
					data[j]= burst_coalesced::load(&aux_dram[i*COALESCED + j]);

				for (vx_uint8 j = 0; j < COALESCED; ++j){ 
						stream_out.write(data[j]);	
				}
			} 
}

//template speciallization for uint32 reads streams
template<typename SrcType, typename StreamType, StreamType &stream_out,typename
DramTypeIn, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint COALESCED,
enable_if_t<std::is_same<SrcType, vx_uint32>::value,int> = 0 >
void vxReadDram(DramTypeIn *dram_in){

			vx_uint32 data[COALESCED];
			vx_uint8 input[4];
			auto aux_dram=&(*dram_in[0]);
			vx_uint32 i =0;
			#pragma ii 1
			//for (auto i = 0; i < WIDTH * HEIGHT/(VEC_NUM*COALESCED) -1; i++){
			while (i < WIDTH * HEIGHT/(VEC_NUM*COALESCED)){
		//	First version		
				UNROLL_INTEL()
				for (vx_uint8 j = 0; j < COALESCED; j++){ 
					data[j]= burst_coalesced::load(&aux_dram[i*COALESCED+ j]);
				}
				for (vx_uint8 j = 0; j < COALESCED; ++j){ 
						stream_out.write(data[j]);	
				}
				i++;
			}
	}

//


/*********************************************************************************************************************/
/** @brief	Transfer data from openCL  streaming interface to Avalon Memory Master
 * @param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit) G
 * @param StreamType  Data type of the streamming
 * @param stream_out  Stream object source to DRAM
 * @param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8) 
 * @param WIDTH       Image width  
 * @param HEIGHT      Image height 
 * @param COALESCED   size of the burst petition to DRAM
 * */
//normal
template<typename DstType, typename StreamType, StreamType &stream_in,typename
DramTypeOut, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint COALESCED= 8, 
enable_if_t<!std::is_same<DstType, vx_uint32>::value,int> = 0 >
void vxWriteDram(DramTypeOut *dram_out){
		vx_image<DstType, VEC_NUM> data[COALESCED];

	for (vx_uint32 i = 0; i < WIDTH * HEIGHT/(VEC_NUM*COALESCED); i++){

		for (vx_uint8 j = 0; j < COALESCED; ++j)
			data[j]=(stream_in.read()); 
   
		UNROLL_INTEL()
		for (vx_uint8 j = 0; j < COALESCED; ++j) 
	    	(*dram_out)[i*COALESCED+j] = data[j];	
	  //  	(*dram_out)[i*COALESCED+j] = stream_in.read();	
		
	} 
}
/////////////////
//write speciallization
template<typename DstType, typename StreamType, StreamType &stream_in,typename
DramTypeOut, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint
COALESCED= 8, enable_if_t<std::is_same<DstType, vx_uint32>::value,int> = 0 >
void vxWriteDram(DramTypeOut *dram_out){
//	DstType data[COALESCED];
	vx_image<DstType, VEC_NUM> data[COALESCED];

	for (vx_uint32 i = 0; i < WIDTH * HEIGHT/(VEC_NUM*COALESCED); i++){

		for (vx_uint8 j = 0; j < COALESCED; ++j)
			data[j]=(stream_in.read()); 
   
		UNROLL_INTEL()
		for (vx_uint8 j = 0; j < COALESCED; ++j){ 
	    	(*dram_out)[i*COALESCED+j] = data[j];	
		}
	} 
}
/*********************************************************************************************************************/
/** @brief	Transfer data from openCL  streaming interface to Avalon Memory Master
 * @param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit) G
 * @param StreamType  Data type of the streamming
 * @param stream_out  Stream object source to DRAM
 * @param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8) 
 * @param WIDTH       Image width  
 * @param HEIGHT      Image height 
 * @param COALESCED   size of the burst petition to DRAM
 * */
template<typename DstType, typename StreamType, StreamType &stream_in,typename DramTypeOut, vx_uint8 VEC_NUM, vx_uint16 WIDTH, uint COALESCED = 8>
void vxDistributionWriteDram(DramTypeOut *dram_out){
	vx_distribution<DstType, VEC_NUM> data[COALESCED];

	for (auto i = 0; i < WIDTH /(VEC_NUM*COALESCED); i++){
		for (vx_uint8 j = 0; j < COALESCED; ++j)
			data[j]=hls_fpga_reg(stream_in.read()); 
   
		UNROLL_INTEL()
		for (vx_uint8 j = 0; j < COALESCED; ++j) 
	    	(*dram_out)[i*COALESCED+j] = data[j];	
	} 
}
/*********************************************************************************************************************/
/** @brief	Copy data from one stream to simultaneous access in graph
 * @param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit) G
 * @param WIDTH       Image width  
 * @param HEIGHT      Image height 
 * @param StreamBase  Stream to copy 
 * @param stream_out  Stream copy
 * */
template<typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, 
	typename StreamType, StreamType &stream_base, StreamType &stream_out0, 
	StreamType &stream_out1, enable_if_t<!std::is_same<SrcType, vx_uint32>::value,int> = 0 >
void SplitStream(){
	vx_image<SrcType, VEC_NUM> data;
	for (vx_uint32 i = 0; i < WIDTH * HEIGHT/(VEC_NUM); i++){
    	//printf("%d\n", i);
		data = stream_base.read();
		stream_out0.write(data);		
		stream_out1.write(data);		
	}
}
//templeate specialization SplitStream
template<typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, 
	typename StreamType, StreamType &stream_base, StreamType &stream_out0, 
	StreamType &stream_out1, enable_if_t<std::is_same<SrcType, vx_uint32>::value,int> = 0 >
void SplitStream(){
	SrcType data;
	for (vx_uint32 i = 0; i < WIDTH * HEIGHT/(VEC_NUM); i++){
    	//printf("%d\n", i);
		data = stream_base.read();
		stream_out0.write(data);		
		stream_out1.write(data);		
	}
}
/*********************************************************************************************************************/
/*********************************************************************************************************************/
/***************CLASS WRAPPER******************************************************************************************/
/** @brief	Transfer task to launch functions
 * @param SrcType     Data type of the input image (unsigned)(8-bit, 16-bit, 32-bit) 
 * @param StreamType  Data type of the streamming
 * @param stream_out  Stream object source to DRAM
 * @param VEC_NUM     Amount of pixels computed in parallel (1, 2, 4, 8) 
 * @param WIDTH       Image width  
 * @param HEIGHT      Image height 
 * @param COALESCED   size of the burst petition to DRAM
 * */

template<typename SrcType, typename StreamType, StreamType &input0, typename DramTypeIn, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint COALESCED>
struct vxDramRead
{
    vxDramRead(DramTypeIn *dram_in){
	//ihc::launch((vxReadDram<SrcType, decltype(input0), input0, DramTypeIn, VEC_NUM, WIDTH, HEIGHT, COALESCED>)); 
	ihc::launch((vxReadDram<SrcType, decltype(input0), input0, DramTypeIn, VEC_NUM, WIDTH, HEIGHT, COALESCED>), dram_in); 
	}
    void vxReleaseNode(){
//	ihc::collect((vxReadDram<SrcType, decltype(input0), input0, DramTypeIn, VEC_NUM, WIDTH, HEIGHT,COALESCED>)); 
	}
};

template<typename SrcType, typename StreamType, StreamType &input0, typename
DramTypeOut, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, uint COALESCED
= 1>
struct vxDramWrite
{
   /*  @see vxWriteDram(DramTypeOut *dram_out)*/
    vxDramWrite(DramTypeOut *dram_in){
	ihc::launch((vxWriteDram<SrcType, decltype(input0), input0, DramTypeOut, VEC_NUM, WIDTH, HEIGHT, COALESCED>), dram_in); 
	}
    void vxReleaseNode(){
	ihc::collect((vxWriteDram<SrcType, decltype(input0), input0, DramTypeOut, VEC_NUM, WIDTH, HEIGHT, COALESCED>)); 
	}
};


template<typename SrcType, typename StreamType, StreamType &input0, typename DramTypeOut, vx_uint8 VEC_NUM, vx_uint16 WIDTH, uint COALESCED = 8>
struct vxDistributionDramWrite
{
   /*  @see vxWriteDram(DramTypeOut *dram_out)*/
    vxDistributionDramWrite(DramTypeOut *dram_in){
	ihc::launch((vxDistributionWriteDram<SrcType, decltype(input0), input0, DramTypeOut, VEC_NUM, WIDTH, COALESCED>), dram_in); 
	}
    void vxReleaseNode(){
	ihc::collect((vxDistributionWriteDram<SrcType, decltype(input0), input0, DramTypeOut, VEC_NUM, WIDTH, COALESCED>)); 
	}
};

template<typename SrcType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, 
	typename StreamType, StreamType &stream_base, StreamType &stream_out0, StreamType &stream_out1>
struct vxSplitStream
{
    vxSplitStream(){
	ihc::launch((SplitStream <SrcType, VEC_NUM, WIDTH, HEIGHT, decltype(stream_base), stream_base, stream_out0, stream_out1>)); 
	}
    void vxReleaseNode(){
//	ihc::collect((SplitStream <SrcType, VEC_NUM, WIDTH, HEIGHT, StreamType, stream_base, stream_out0, stream_out1>)); 
	}
};
#endif
