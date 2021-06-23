#ifdef Xilinx
	#include "img_filter_base.h"
	#include "img_filter_component_xilinx.h"
//     //TODO seprate Xilinx definitios form img_filter_core
#elif Intel
	#include "HLS/hls.h"
	#include "sobel_tiling6_component.h"
#endif

#include "definitions.h"
#include "bmp_tools.h"


typedef vx_uint8 filter_u_type;
typedef vx_int8 filter_s_type;
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
	std::string input1_bmp_filename    = "in_test.bmp";
	std::string output_bmp_filename   = "out_test.bmp";

	//Avalon master Interfaces
	vx_image<filter_u_type, VEC_NUM> tmp_in[COLS_FHD*ROWS_FHD/VEC_NUM];
	vx_image<filter_s_type, VEC_NUM> tmp_out[COLS_FHD*ROWS_FHD/VEC_NUM];
	vx_image<filter_s_type, VEC_NUM> tmp_out1[COLS_FHD*ROWS_FHD/VEC_NUM];

	#ifdef Xilinx
    
	#elif Intel
	DramTypeIn00 mm_in(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut00 mm_out(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut10 mm_out1(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	DramTypeIn01 mm_in01(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut01 mm_out01(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut11 mm_out11(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	DramTypeIn02 mm_in02(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut02 mm_out02(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut12 mm_out12(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	DramTypeIn03 mm_in03(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut03 mm_out03(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut13 mm_out13(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	DramTypeIn04 mm_in04(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut04 mm_out04(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut14 mm_out14(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	DramTypeIn05 mm_in05(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut05 mm_out05(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut15 mm_out15(tmp_out1,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);

	#endif

	//
	//  load bmp image
	unsigned int* in_img = 0;;
	int rows, cols;
	read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);
	//assert (rows == ROWS_FHD);	//Verifies if the input image is the same as hardware reference	
	vx_image<filter_u_type,VEC_NUM> data;
	for (int i = 0; i < (ROWS_FHD*COLS_FHD)/VEC_NUM; ++i) {
		for (int j = 0; j < VEC_NUM; ++j) {
			data.pixel[j]= static_cast<filter_u_type>(in_img[i*VEC_NUM+j]);
		}
		tmp_in[i]=data;
	}

	//calling the component
	ImgSobelFunc<filter_u_type, filter_s_type, VEC_NUM , COLS_FHD , ROWS_FHD , FILTER_SIZE , SOBEL_BORDER>(mm_in, mm_out, mm_out1,  mm_in01, mm_out01, mm_out11,
	mm_in02, mm_out02, mm_out12,
	mm_in03, mm_out03, mm_out13,
	mm_in04, mm_out04, mm_out14,
	mm_in05, mm_out05, mm_out15);

	unsigned int* out_img = (unsigned int*) malloc(rows * cols * sizeof(unsigned int));
	vx_image<filter_s_type,VEC_NUM> data_out;

	for (int i = 0; i < (rows*cols)/VEC_NUM; ++i) {
		//if it is an streaming
		//			data_out = output1.read();
		data_out=tmp_out[i];
		for (int j = 0; j < VEC_NUM; ++j) {
			out_img[i*VEC_NUM+j] = data_out.pixel[j];
		}

	}
	//save the processed image in a .bmp file
	write_bmp(output_bmp_filename.c_str(), out_img, rows, cols);
}

