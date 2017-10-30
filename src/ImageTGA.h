
#ifndef IMAGETGA_H
#define IMAGETGA_H

#include "Boolean.h"

extern const int RAW_RGB_ENCODING;
extern const int RLE_RGB_ENCODING;

/**
 * @brief	Stores Targa (TGA) image data. 
 * @details	Only supports raw RGB and RLE compressed RBG. 
 * 			Supports 24 or 32 bit images with alpha channel.
 * @author	Matt Drage
 * @date	11/03/2012
 */
struct ImageTGA
{
	short int width, height, bpp;
	unsigned long imageSize;
	char encoding;
	unsigned char *image;
};
typedef struct ImageTGA ImageTGA;

/**
 * @brief	Loads a TGA image file.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	tga 	The tga object to store the data in.
 * @param 	file	The file to read.
 * @return	true if it succeeds, false if it fails.
 */
bool TGALoad(ImageTGA *tga, char *file);

/**
 * @brief	Reads the header metadata of a TGA image.
 * @details	Used internally by TGALoad function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	tga 	The tga image to store info in.
 * @param 	data	The header data to read from.
 * @return	true if it succeeds, false if it fails.
 */
bool TGAReadHeader(ImageTGA *tga, unsigned char* data);

/**
 * @brief	Loads raw RGB data from a TGA file.
 * @details	Used internally by TGALoad function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	tga 	The tga to store the data in.
 * @param 	data	The data to read.
 * @return	true if it succeeds, false if it fails.
 */
bool TGALoadRawData(ImageTGA *tga, unsigned char* data);

/**
 * @brief	Loads run length encoded data from a TGA file.
 * @details	Used internally by TGALoad function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	tga 	The tga to store the data in.
 * @param 	data	The data to read.
 * @return	true if it succeeds, false if it fails.
 */
 bool TGALoadRLEData(ImageTGA *tga, unsigned char* data);

/**
 * @brief	Convers TGA image data from BGR to RGB ordering.
 * @details	Used internally by TGALoad function.
 * @author	Matt Drage
 * @date	11/03/2012
 * @param 	tga		The tga to convert.
 */
void TGABGRtoRGB(ImageTGA *tga);

#endif
