// Copyright (C) 2013-2019 Altera Corporation, San Jose, California, USA. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
// whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// This agreement shall be governed in all respects by the laws of the State of California and
// by the laws of the United States of America.

// bmp_tools.cpp

#include "bmp_tools.h"
#include <stdio.h>
#include <stdlib.h>

// Bitmap header size in bytes
static const int BMP_HEADER_SIZE = 54;

void read_bmp (const char* filename, unsigned int** img_data, int& height, int& width) {
  FILE *fp;
#if defined(_WIN32) || defined(_WIN64)
  if (fopen_s(&fp, filename, "rb") !=0) {
#else
  if ((fp=fopen(filename,"rb"))==NULL) {
#endif
    printf("can't open file %s for binary reading\n", filename);
    exit(1);
  }

  // Skip over unimportant parts of header
  for (int i=0; i<18; i++) fgetc(fp); 
  
  //read width and height
  fread(&width, sizeof(int), 1, fp);
  fread(&height, sizeof(int), 1, fp);
  if (height < 0 || width < 0) {
    printf("error got height %d, width %d\n", height, width);
    exit(1);
  }
  printf("image size is %d (width) by %d (height) pixels\n", width, height);
  *img_data = (unsigned int*) malloc(width * height * sizeof(unsigned int));

  // Skip remaining part of header
  for (int i=0; i<28; i++) fgetc(fp);
  
  // BMP: Each line must be a multiple of 4 bytes
  int padding = (4-((width*3)&3))&3;
  int idx=0;      
  // Color order is BGR, read across bottom row, then repeat going up rows
  for (int i=0; i< height; ++i) {
	  for (int j=0; j< width; ++j) {
      unsigned char b = fgetc(fp); //B
      unsigned char g = fgetc(fp); //G
      unsigned char r = fgetc(fp); //R
      (*img_data)[idx] = (((unsigned int ) r << 16) | ((unsigned int ) g << 8) | ((unsigned int ) b << 0));
      idx++;
	  }
	  // Discard the padding bytes
	  for (int j=0; j< padding; ++j) {
	  	unsigned char b = fgetc(fp);
	  }
  }
  fclose(fp);
}

void write_bmp (const char* filename, unsigned int* img_data, int& height, int& width) {
  FILE *fp;
  unsigned int file_size = width * height * 3 + BMP_HEADER_SIZE;
  unsigned char header[BMP_HEADER_SIZE] = { 0x42, 0x4d,         // BMP & DIB
                                        (static_cast<unsigned char>(file_size & 0xff)), (static_cast<unsigned char>((file_size >> 8) & 0xff)), (static_cast<unsigned char>((file_size >> 16) & 0xff)), (static_cast<unsigned char>((file_size >> 24) & 0xff)), // size of file in bytes
                                        0x00, 0x00, 0x00, 0x00, // reserved
                                        0x36, 0x00, 0x00, 0x00, // offset of start of image data
                                        0x28, 0x00, 0x00, 0x00, // size of the DIB header
                                        (static_cast<unsigned char>(width & 0xff)), (static_cast<unsigned char>((width >> 8) & 0xff)), (static_cast<unsigned char>((width >> 16) & 0xff)), (static_cast<unsigned char>((width >> 24) & 0xff)),     // width in pixels
                                        (static_cast<unsigned char>(height & 0xff)), (static_cast<unsigned char>((height >> 8) & 0xff)), (static_cast<unsigned char>((height >> 16) & 0xff)), (static_cast<unsigned char>((height >> 24) & 0xff)), // height in pixels
                                        0x01, 0x00, // number of color planes
                                        0x18, 0x00, // number of bits per pixel
                                        0x00, 0x00, 0x00, 0x00, // no compression - BI_RGB
                                        0x00, 0x00, 0x00, 0x00, // image size - dummy 0 for BI_RGB
                                        0x13, 0x0b, 0x00, 0x00, // horizontal ppm
                                        0x13, 0x0b, 0x00, 0x00, // vertical ppm
                                        0x00, 0x00, 0x00, 0x00, // default 2^n colors in palatte
                                        0x00, 0x00, 0x00, 0x00  // every color is important
                                      };
  // Open file for write
#if defined(_WIN32) || defined(_WIN64)
  if (fopen_s(&fp, filename, "wb") !=0) {
#else
  if ((fp=fopen(filename,"wb"))==NULL) {
#endif
    printf("can't open file %s for binary writing\n", filename); 
    exit(1);
  }

  // Write header
  fwrite(header, 1, BMP_HEADER_SIZE, fp);

  // Write data: Line size must be a multiple of 4 bytes
  int padding = (4-((width*3)&3))&3;
  int idx = 0;
  for (int i = 0; i <  height; ++i) {
    unsigned char p[3];
    for (int j = 0; j < width ; ++j) {
      // written in B, G, R order
      p[0] = (img_data[idx] >>  0) & 0xff; //B
      p[1] = (img_data[idx] >>  8) & 0xff; //G
      p[2] = (img_data[idx] >>  16) & 0xff; //R
      idx++;
      fwrite(p, 1, 3, fp);
    }
    // Pad to multiple of 4 bytes
    if (padding) {
    	p[0]=p[1]=p[2]=0;
    	fwrite(p,1,padding,fp);
    }
  }
  fclose(fp);
}
