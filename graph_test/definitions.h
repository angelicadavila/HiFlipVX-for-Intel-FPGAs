//// Conversion from Xilinx to intel follow this rules:
//https://www.intel.com/content/www/us/en/programmable/documentation/mtr1422491996806.html#mtr1422491925947 to intel follow this rules:

// Image Size
#ifndef SRC_IMG_CONSTANTS_H_
#define SRC_IMG_CONSTANTS_H_ 

const vx_uint16 COLS_FHD = 1920*2;
const vx_uint16 ROWS_FHD =  1080*2;
//const vx_uint16 COLS_FHD = 1920;
//const vx_uint16 ROWS_FHD =  1080;
//const vx_uint16 COLS_FHD = 680;
//const vx_uint16 ROWS_FHD = 480;
//const vx_uint16 COLS_FHD = 680;
//const vx_uint16 ROWS_FHD = 480;
//const vx_uint16 COLS_FHD = 528;
//const vx_uint16 ROWS_FHD =  352;
const auto PIXELS_FHD = COLS_FHD*ROWS_FHD;
//const vx_uint16 COLS_HD = 1280;
//const vx_uint16 ROWS_HD = 720;
//const auto PIXELS_HD = COLS_HD*ROWS_HD;

// Vectorization
//const vx_uint8 VEC_NUM =1;
//autoc
//const vx_uint8 VEC_NUM = 1;
const vx_uint8 VEC_NUM = 8;
const vx_uint16 VEC_PIX = static_cast<vx_uint16>(PIXELS_FHD / VEC_NUM);

// Defines
const vx_uint8 FILTER_SIZE = 3;
#define SEPARABLE_FILTER   (vx_bool)vx_true_e
#define CONVOLVE_FILTER_SIZE 3
#define BOX_BORDER      VX_BORDER_CONSTANT
#define CONVOLVE_BORDER VX_BORDER_CONSTANT
#define DILATE_BORDER   VX_BORDER_CONSTANT
#define ERODE_BORDER    VX_BORDER_CONSTANT
#define GAUSSIAN_BORDER VX_BORDER_CONSTANT
#define MEDIAN_BORDER   VX_BORDER_REPLICATE
#define SCHARR_BORDER   VX_BORDER_CONSTANT
#define SOBEL_BORDER    VX_BORDER_CONSTANT

//histogram
#define HIST_BINS   256
#define HIST_OFFSET 0
#define HIST_RANGE  256            // (1 << 8), (1 << 16)
//#define HIST_RANGE  (1 << 8)            // (1 << 8), (1 << 16)
//autoc
#define EXTRACT_CHANNELY 2  
#define EXTRACT_CHANNELU 1  
#define EXTRACT_CHANNELV 0  

//RGB
#define VX_CHANNEL_R 2  
#define VX_CHANNEL_G 1  
#define VX_CHANNEL_B 0  

#define COMB_CHANNEL 3  


#define EXTRACT_TYPE    VX_DF_IMAGE_NV12


#endif

