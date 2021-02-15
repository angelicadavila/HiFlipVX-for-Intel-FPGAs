
#define vx_image_in uint8

#define vx_image_out uint8

void FunctionTemplateName ( __global vx_image_in* Img_in,
			  __global vx_image_out* Img_outx
);


__kernel void exm1( __global vx_image_in* restrict Img_in,
                    __global vx_image_out* restrict Img_outx
) {
    FunctionTemplateName (Img_in, Img_outx);
}




//
//typedef struct vx_image_t{
//    uint8 pixel[8];
//}vx_image_in;
//
//typedef struct vx_imageo_t{
//    uint8 pixel[8];
//}vx_image_out;
//
//void FunctionTemplateName ( __global vx_image_in* Img_in,
//			  __global vx_image_out* Img_outx
//);
//
//
//__kernel void exm1( __global vx_image_in* restrict Img_in,
//                    __global vx_image_out* restrict Img_outx
//) {
//    FunctionTemplateName (Img_in, Img_outx);
//}

