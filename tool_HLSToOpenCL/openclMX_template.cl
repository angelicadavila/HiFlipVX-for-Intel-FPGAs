//#define vx_image_in uint
//
//#define vx_image_out int
typedef struct vx_image_t{
    uint8 pixel[8];
}vx_image_in;

typedef struct vx_imageo_t{
    int8 pixel[8];
}vx_image_out;

#define global_t(data_type, hbm_bank_name) \
    __global __attribute((buffer_location(hbm_bank_name))) data_type

void FunctionTemplateName ( 
		    	global_t(vx_image_in, "HBM0")* restrict Img_in00,
                global_t(vx_image_out, "HBM1")*restrict Img_out00,
		    	global_t(vx_image_out, "HBM2")*restrict Img_out10,
                global_t(vx_image_in, "HBM3")* restrict Img_in01,
		    	global_t(vx_image_out, "HBM4")*restrict Img_out01,
                global_t(vx_image_out, "HBM5")*restrict Img_out11,
                global_t(vx_image_in, "HBM6")* restrict Img_in02,
		    	global_t(vx_image_out, "HBM7")*restrict Img_out02,
                global_t(vx_image_out, "HBM8")*restrict Img_out12,
                global_t(vx_image_in, "HBM9")* restrict Img_in03,
		    	global_t(vx_image_out, "HBM10")*restrict Img_out03,
                global_t(vx_image_out, "HBM11")*restrict Img_out13
);


__kernel void exm1( 
		    	global_t(vx_image_in, "HBM0")* restrict Img_in00,
                global_t(vx_image_out, "HBM1")*restrict Img_out00,
		    	global_t(vx_image_out, "HBM2")*restrict Img_out10,
                global_t(vx_image_in, "HBM3")* restrict Img_in01,
		    	global_t(vx_image_out, "HBM4")*restrict Img_out01,
                global_t(vx_image_out, "HBM5")*restrict Img_out11,
                global_t(vx_image_in, "HBM6")* restrict Img_in02,
		    	global_t(vx_image_out, "HBM7")*restrict Img_out02,
                global_t(vx_image_out, "HBM8")*restrict Img_out12,
                global_t(vx_image_in, "HBM9")* restrict Img_in03,
		    	global_t(vx_image_out, "HBM10")*restrict Img_out03,
                global_t(vx_image_out, "HBM11")*restrict Img_out13
) {
    FunctionTemplateName(Img_in00, Img_out00, Img_out10, 
						 Img_in01, Img_out01, Img_out11,
						 Img_in02, Img_out02, Img_out12,
						 Img_in03, Img_out03, Img_out13);
}

