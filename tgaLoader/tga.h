#ifndef __TGA_H__
#define __TGA_H__

#include "texture.h"

typedef struct
{
	unsigned char Header[12];								// TGA File Header
} TGAHeader;

typedef struct
{
	unsigned char		header[6];							// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;						// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;							// Used To Store The Image Size When Setting Aside Ram
	unsigned int		temp;									// Temporary Variable
	unsigned int		type;
	unsigned int		Height;								//Height of Image
	unsigned int		Width;								//Width ofImage
	unsigned int		Bpp;									// Bits Per Pixel
} TGA;

TGAHeader tgaheader;									// TGA header
TGA tga;												   // TGA image data

unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

#endif
