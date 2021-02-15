//************************Xilinxs Templates *******************************""
//Template definitons to generates the hadwawre. Check definitons in img_filter_base.h

template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE>
void ImgBox(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE	
	HwBox<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output);
}


template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE>
void ImgConvolve(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM], const DataType conv[KERN_SIZE][KERN_SIZE]) {
#pragma HLS INLINE		
	HwConvolve<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output, conv);
}


template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE>
void ImgDilate(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwDilate<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output);
}


template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE>
void ImgErode(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwErode<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output);
}


template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE, vx_bool SEPARABLE>
void ImgGaussian(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwGaussian<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE, SEPARABLE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output);
}


template<typename DataType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE>
void ImgMedian(DataType input[(HEIGHT*WIDTH) / VEC_NUM], DataType output[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwMedian<DataType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<DataType, VEC_NUM> *)input, (vx_image<DataType, VEC_NUM> *)output);
}


template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_border_e BORDER_TYPE>
void ImgScharr3x3(SrcType input[(HEIGHT*WIDTH) / VEC_NUM], DstType output1[(HEIGHT*WIDTH) / VEC_NUM], DstType output2[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwScharr3x3<SrcType, DstType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, BORDER_TYPE>((vx_image<SrcType, VEC_NUM> *)input, (vx_image<DstType, VEC_NUM> *)output1, (vx_image<DstType, VEC_NUM> *)output2);
}



template <typename SrcType, typename DstType, vx_uint8 VEC_NUM, vx_uint16 WIDTH, vx_uint16 HEIGHT, vx_uint8 KERN_SIZE, vx_border_e BORDER_TYPE>
void ImgSobel(SrcType input[(HEIGHT*WIDTH) / VEC_NUM], DstType output1[(HEIGHT*WIDTH) / VEC_NUM], DstType output2[(HEIGHT*WIDTH) / VEC_NUM]) {
#pragma HLS INLINE		
	HwSobel<SrcType, DstType, VECTOR_PIXELS(HEIGHT, WIDTH, VEC_NUM), VEC_NUM, WIDTH, HEIGHT, KERN_SIZE, BORDER_TYPE>((vx_image<SrcType, VEC_NUM> *)input, (vx_image<DstType, VEC_NUM> *)output1, (vx_image<DstType, VEC_NUM> *)output2);
}

