#ifdef Xilinx
	#include "img_filter_base.h"
	#include "img_filter_component_xilinx.h"
//     //TODO seprate Xilinx definitios form img_filter_core
#elif Intel
	#include "HLS/hls.h"
	#include "component_intel.h"
#endif

#include "definitions.h"
#include "bmp_tools.h"


typedef vx_uint32 filter_u_type;
typedef vx_uint32 filter_s_type;
typedef vx_image<filter_u_type, VEC_NUM, vx_streamIn_e> filter_u_image;

int main(){
#ifdef Xilinx
#pragma HLS INTERFACE axis port=input
#pragma HLS INTERFACE axis port=output1
#pragma HLS INTERFACE axis port=output2

#pragma HLS data_pack variable=input
#pragma HLS data_pack variable=output1
#pragma HLS data_pack variable=output2
#endif
	//Input ppm image -only for CPU test
	//std::string input_bmp_filename    = "test.bmp";
	std::string input_bmp_filename    = "rabbit.bmp";
//	std::string input_bmp_filename    = "./../figure/sky4k.bmp";
	std::string input1_bmp_filename    = "in_test.bmp";
	std::string output_bmp_filename   = "out_test.bmp";

	//Avalon master Interfaces
	//vx_image_t<filter_u_type, VEC_NUM> tmp_in[COLS_FHD*ROWS_FHD/VEC_NUM];
	//vx_image_t<filter_s_type, VEC_NUM> tmp_out[COLS_FHD*ROWS_FHD/VEC_NUM];
	auto SIZE = COLS_FHD *ROWS_FHD;
	vx_image<vx_uint8, VEC_NUM> tmp_in[SIZE/VEC_NUM];
	vx_image<vx_uint8,VEC_NUM> tmp_out[SIZE/VEC_NUM];

	#ifdef Xilinx
    
	#elif Intel
	//DramTypeIn0 mm_in(tmp_in,sizeof(vx_image<vx_uint32,VEC_NUM>)*SIZE/VEC_NUM);
	DramTypeIn0 mm_in(tmp_in,sizeof(vx_uint8)*SIZE);
	DramTypeOut0 mm_out(tmp_out,sizeof(vx_uint8)*SIZE);
	//DramTypeOut0 mm_out(tmp_out,sizeof(vx_uint32)*SIZE);
	#endif
	//  load bmp image
	unsigned int* in_img = 0;;
	int rows = ROWS_FHD;
    int cols = COLS_FHD;
//	read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);
	//assert (rows == ROWS_FHD);	//Verifies if the input image is the same as hardware reference	
	vx_image<vx_uint8,VEC_NUM> data;
	for (int i = 0; i < (ROWS_FHD*COLS_FHD)/ VEC_NUM; ++i) {
		for (int j = 0; j < VEC_NUM; ++j) {
//			data.pixel[j]= static_cast<filter_u_type>(in_img[i*VEC_NUM+j]);
			data.pixel[j]= static_cast<vx_uint8>(0xaa);;
 
		}
		tmp_in[i]=data;
//			tmp_in[i]= static_cast<filter_u_type>(0x0f);
	}

	//calling the component
	ImgAutoContrast<filter_u_type, filter_s_type, VEC_NUM , COLS_FHD , ROWS_FHD , FILTER_SIZE , SOBEL_BORDER>(mm_in, mm_out);
//
//	unsigned int* out_img = (unsigned int*) malloc(ROWS_FHD * COLS_FHD * sizeof(unsigned int));
//	vx_image<filter_s_type,VEC_NUM> data_out;
//
//	for (int i = 0; i < (ROWS_FHD * COLS_FHD)/VEC_NUM; ++i) {
//		//if it is an streaming
//		//			data_out = output1.read();
//		data_out=tmp_out[i];
//		for (int j = 0; j < VEC_NUM; ++j) {
//			out_img[i*VEC_NUM+j] = data_out.pixel[j];
//		}
//
//	}
//	//save the processed image in a .bmp file
//	write_bmp(output_bmp_filename.c_str(), out_img, rows, cols);
}

