#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdio.h>										// Standard I/O header

typedef	struct
{
	unsigned char	* imageData;									// Image Data (Up To 32 Bits)
	unsigned int	bpp;											// Image Color Depth In Bits Per Pixel
	unsigned int	width;											// Image Width
	unsigned int	height;											// Image Height
	unsigned int	texID;											// Texture ID Used To Select A Texture
	unsigned int	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;

int LoadUncompressedTGA(Texture *, const char *, FILE *);	// Load an Uncompressed file
int LoadCompressedTGA(Texture *, const char *, FILE *);		// Load a Compressed file
int LoadTGA(Texture * texture, const char *filename);

Texture *loadGLTexture(const char *filename);

#endif
