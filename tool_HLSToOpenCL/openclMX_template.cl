//
//#define vx_image_in uint8
//
//#define vx_image_out uint8
//
//#define global_t(data_type, hbm_bank_name) \
//    __global __attribute((buffer_location(hbm_bank_name))) data_type
//
//void FunctionTemplateName ( global_t (vx_image_in, "HBM1")* Img_in,
//			  global_t (vx_image_out, "HBM1")* Img_outx
//);
//
//
//__kernel void exm1( global_t(vx_image_in, "HBM0")* restrict Img_in,
//                    global_t(vx_image_out, "HBM1")* restrict Img_outx
//) {
//    FunctionTemplateName (Img_in, Img_outx);
//}
//
//

typedef struct vx_image_t{
    uint8 pixel[8];
}vx_image_in;

typedef struct vx_imageo_t{
    uint8 pixel[8];
}vx_image_out;

#define global_t(data_type, hbm_bank_name) \
    __global __attribute((buffer_location(hbm_bank_name))) data_type

void FunctionTemplateName ( global_t (vx_image_in, "HBM0")* Img_in,
			  global_t (vx_image_out, "HBM1")* Img_outx,
			  global_t (vx_image_out, "HBM2")* Img_outx1
);


__kernel void exm1( global_t(vx_image_in, "HBM0")* restrict Img_in,
                    global_t(vx_image_out, "HBM1")* restrict Img_outx,
                    global_t(vx_image_out, "HBM2")* restrict Img_outx1
) {
    FunctionTemplateName (Img_in, Img_outx, Img_outx1);
}

