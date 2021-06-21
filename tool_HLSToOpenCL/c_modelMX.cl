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

void FunctionTemplateName ( 
		    	global_t(vx_image_in, "HBM0")* Img_in00,
                global_t(vx_image_out, "HBM1")* Img_out00,
		    	global_t(vx_image_out, "HBM2")* Img_out01,

                global_t(vx_image_in, "HBM3")* Img_in01,
		    	global_t(vx_image_out, "HBM4")* Img_out01,
                global_t(vx_image_out, "HBM5")* Img_out11,

                global_t(vx_image_in, "HBM6")* Img_in02,
		    	global_t(vx_image_out, "HBM7")* Img_out02,
                global_t(vx_image_out, "HBM8")* Img_out12,

                global_t(vx_image_in, "HBM9")* Img_in03,
		    	global_t(vx_image_out, "HBM10")* Img_out03,
                global_t(vx_image_out, "HBM11")* Img_out13
                    ) {

}

