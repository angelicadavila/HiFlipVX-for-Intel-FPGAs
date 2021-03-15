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
    std::string output_bmp_filename   = "out_test.bmp"; 

	//Avalon master Interfaces
	vx_image<filter_u_type, VEC_NUM> tmp_in[COLS_FHD*ROWS_FHD/VEC_NUM];
	vx_image<filter_s_type, VEC_NUM> tmp_out[COLS_FHD*ROWS_FHD/VEC_NUM];

	#ifdef Xilinx
    
	#elif Intel
	DramTypeIn0 mm_in(tmp_in,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
	DramTypeOut0 mm_out(tmp_out,sizeof(vx_image<filter_u_type, VEC_NUM>)*COLS_FHD*ROWS_FHD);
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

	//set all the constant values
//	const vx_uint8 thresValue95 = 95;
//    vx_threshold<vx_uint8> thresh95 = vxCreateThreshold<vx_uint8>(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//	vxSetThresholdAttribute<vx_uint8>(thresh95, VX_THRESHOLD_THRESHOLD_VALUE, &thresValue95, sizeof(vx_uint8));
//
//	vx_uint8 thresValue40 = 40;
//	vx_threshold<vx_uint8> thresh40 = vxCreateThreshold<vx_uint8>(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//	vxSetThresholdAttribute<vx_uint8>(thresh40, VX_THRESHOLD_THRESHOLD_VALUE, &thresValue40, sizeof(vx_uint8));
//
//	vx_uint8 thresValue20 = 20;
//	vx_threshold<vx_uint8> thresh20 = vxCreateThreshold<vx_uint8>(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//	vxSetThresholdAttribute<vx_uint8>(thresh20, VX_THRESHOLD_THRESHOLD_VALUE, &thresValue20, sizeof(vx_uint8));
//
//	vx_uint8 thresValue15 = 15;
//	vx_threshold<vx_uint8> thresh15 = vxCreateThreshold<vx_uint8>(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//	vxSetThresholdAttribute<vx_uint8>(thresh15, VX_THRESHOLD_THRESHOLD_VALUE, &thresValue15, sizeof(vx_uint8));
//
//	vx_uint8 thresValue0 = 0;
//	vx_threshold<vx_uint8> thresh0 = vxCreateThreshold<vx_uint8>(VX_THRESHOLD_TYPE_BINARY, VX_TYPE_UINT8);
//	vxSetThresholdAttribute<vx_uint8>(thresh0, VX_THRESHOLD_THRESHOLD_VALUE, &thresValue0, sizeof(vx_uint8));

	//calling the component
	skinTone<filter_u_type, filter_s_type, VEC_NUM , COLS_FHD , ROWS_FHD>(mm_in, mm_out);

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

