#ifdef Xilinx
	#include "img_filter_base.h"
	#include "img_filter_component_xilinx.h"
//     //TODO seprate Xilinx definitios form img_filter_core
#elif Intel
	#include "HLS/hls.h"
	#include "skinTone_component.h"
#endif

#include "definitions.h"
#include "bmp_tools.h"


typedef vx_uint32 filter_u_type;
typedef vx_uint8 filter_s_type;
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
//	std::string input_bmp_filename    = "rabbit.bmp";
	std::string input_bmp_filename    = "face.bmp";
    std::string output_bmp_filename   = "out_test.bmp"; 

	//Avalon master Interfaces
	filter_u_type tmp_in[COLS_FHD*ROWS_FHD/VEC_NUM];
	filter_s_type tmp_out[COLS_FHD*ROWS_FHD];

	#ifdef Xilinx
    
	#elif Intel
	//DramTypeIn0 mm_in(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	//DramTypeOut0 mm_out(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeIn0 mm_in(tmp_in,sizeof(filter_u_type)*COLS_FHD*ROWS_FHD);
	DramTypeOut0 mm_out(tmp_out,sizeof(filter_s_type)*COLS_FHD*ROWS_FHD);
	#endif
	//
	//  load bmp image
	unsigned int* in_img = 0;;
	int rows, cols;
	read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);
	//assert (rows == ROWS_FHD);	//Verifies if the input image is the same as hardware reference	
	filter_u_type data;
	for (int i = 0; i < (ROWS_FHD*COLS_FHD)/VEC_NUM; ++i) {
	    tmp_in[i]= static_cast<filter_u_type>(in_img[i]);
	}

	//calling the component
	skinTone<filter_u_type, filter_s_type, VEC_NUM , COLS_FHD , ROWS_FHD>(mm_in, mm_out);

	unsigned int* out_img = (unsigned int*) malloc(rows * cols * sizeof(unsigned int));
	vx_uint32 data_out;

	for (int i = 0; i < (rows*cols); ++i) {
		//if it is an streaming
		//			data_out = output1.read();
		data_out=tmp_out[i];
		//data_out=tmp_out[i];
			out_img[i] = static_cast<vx_uint32>(tmp_out[i]);
	}
  //save the processed image in a .bmp file
	write_bmp(output_bmp_filename.c_str(), out_img, rows, cols);
}

