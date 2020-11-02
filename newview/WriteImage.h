//
// Created by MQ on 2020/8/27.
//

#ifndef LEARNOPENGL_WRITEIMAGE_H
#define LEARNOPENGL_WRITEIMAGE_H

#include<iostream>
#include<stdint.h>
#include<fstream>
#include <float.h> //for float_MAX
#include <tiffio.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;

//
//struct image_32 {
//    u32 Height ;
//    u32 Width ;
//    u32* Pixels ;
//} ;
//
//struct bitmap_header {
//    u16 FileType ;
//    u32 FileSize ;
//    u16 Reserved1;
//    u16 Reserved2;
//    u32 BitmapOffset;
//    u32 Size ;//also need to set the size of the pixels. Since the header is 50 bytes check wikipedia.
//    u32 Width ;
//    u32 Height ;
//    u16 Planes ;
//    u16 BitsPerPixel;
//    u32 Compression ;
//    u32 SizeOfBitmap ;//writing bottom part first. Very Important.
//    u32 HorzResolution ;
//    u32 VertResolution;
//    u32 ColorsUsed ;
//    u32 ColorsImportant;
//};


void writeTiff(const float* data,u32 height,u32 width,std::string path)
{
    TIFF* tiff = TIFFOpen(path.c_str(),"w+") ;
    TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,width) ;
    TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,height) ;
    TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,1) ;
    TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,32) ;
    TIFFSetField(tiff,TIFFTAG_SAMPLEFORMAT,SAMPLEFORMAT_COMPLEXIEEEFP) ;

    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
//   Some other essential fields to set that you do not have to understand for now.
    float* linebuffer = (float*)malloc(sizeof(float)*width) ;

    TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,sizeof(float)*width) ;
    for(u32 r =0 ;r<height;r++)
    {
        memcpy(linebuffer,&data[(height-r-1)*width],sizeof(float)*width) ;
        for(int i = 0; i<1024;i++)
        {
            linebuffer[i] = 1-linebuffer[i] ;
        }
        TIFFWriteScanline(tiff, (unsigned char *) linebuffer, r, 0);
    }

    (void) TIFFClose(tiff);
    free(linebuffer) ;
}


void writeRGBATIFF(const unsigned char* data,int WIDTH ,int HEIGHT,std::string path)
{
    TIFF* tiff = TIFFOpen(path.c_str(),"w+") ;
    TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,WIDTH) ;
    TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,HEIGHT) ;
    TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,4) ;
    TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,8) ;
    TIFFSetField( tiff, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
    TIFFSetField( tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField( tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.
//   Som  e other essential fields to set that you do not have to understand for now.
    int linesize = sizeof(unsigned char)*4*WIDTH ;
    unsigned char* linebuffer = (unsigned char*)malloc(linesize) ;

    TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,linesize) ;

    for(u32 r =0 ;r<HEIGHT;r++)
    {
        memcpy(linebuffer,&data[(HEIGHT-r-1)*WIDTH*4],linesize) ;
        TIFFWriteScanline(tiff, linebuffer, r, 0);
    }
    (void) TIFFClose(tiff);
    free(linebuffer) ;
}
#endif //LEARNOPENGL_WRITEIMAGE_H
