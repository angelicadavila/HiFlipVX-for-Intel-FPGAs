//
//
//#define vx_image_in uint8
//
//#define vx_image_out uint8
//
//#define global_t(data_type, hbm_bank_name) \ 
//	__global __attribute((buffer_location(hbm_bank_name))) data_type
//
//void FunctionTemplateName ( global_t(vx_image_in, "HBM0")* Img_in,
//               global_t(vx_image_out, "HBM1")* Img_out,
//                    ) {
//
//}
//

typedef struct vx_image_t{
    uint8 pixel[8]
}vx_image_in

typedef struct vx_image_t{
    int8 pixel[8]
}vx_image_out

#define global_t(data_type, hbm_bank_name) \ 
	__global __attribute((buffer_location(hbm_bank_name))) data_type

void FunctionTemplateName ( global_t(vx_image_in, "HBM0")* Img_in,
               global_t(vx_image_out, "HBM1")* Img_out,
//               global_t(vx_image_out, "HBM2")* Img_out1
		const int rows
                    ) {

}

