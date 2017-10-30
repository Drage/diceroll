
#include "ImageTGA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int RAW_RGB_ENCODING = 2;
const int RLE_RGB_ENCODING = 10;

bool TGALoad(ImageTGA *tga, char *filename)
{
	unsigned long fileSize;
	unsigned char* data;
	FILE *file = fopen(filename, "rb");
 
	if (!file)
		return false;
 
	/* get file size */
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
 
	/* allocate memory */
	data = (unsigned char*)malloc(fileSize);
 
	if (data == NULL)
	{
		fclose(file);
		return false;
	}
 
	/* Read the file into memory */
	fread(data, fileSize, 1, file);
	fclose(file);
 
	/* Read header data */
	if(!TGAReadHeader(tga, data))
		return false;
 
	if (tga->encoding == RAW_RGB_ENCODING)
	{
		if (!TGALoadRawData(tga, data))
			return false;
	}
	else if (tga->encoding == RLE_RGB_ENCODING)
	{
		if (!TGALoadRLEData(tga, data))
			return false;
	}
	else
		return false;

	/* swap order of colour bytes */
	TGABGRtoRGB(tga);

	free(data);
	return true;
}

bool TGAReadHeader(ImageTGA *tga, unsigned char* data)
{
	short x1, y1, x2, y2;
 
	if (data == NULL)
		return false;
 
	/* Only read RGB files */
	if (data[1] != 0)
		return false;
 
	tga->encoding = data[2];    
 
	/* Only read Raw and RLE RGB encoded files */
	if (tga->encoding != 2 && tga->encoding != 10)
		return false;
 
	/* Get width & height */
	memcpy(&x1, &data[8], 2);
	memcpy(&y1, &data[10], 2);
	memcpy(&x2, &data[12], 2);
	memcpy(&y2, &data[14], 2);
 
	tga->width = (x2 - x1);
	tga->height = (y2 - y1);
 
	/* Bits per Pixel */
	tga->bpp = data[16];
 
	tga->imageSize = (tga->width * tga->height * (tga->bpp/8));
 
	return true;
}

bool TGALoadRawData(ImageTGA *tga, unsigned char* data)
{
	short offset;
 	tga->image = (unsigned char*)malloc(tga->imageSize);
 
	if (tga->image == NULL)
		return false;
 
	/* Set offset to start of image data (after header) */
	offset = data[0] + 18;
 
	memcpy(tga->image, &data[offset], tga->imageSize);
 
	return true;
}

bool TGALoadRLEData(ImageTGA *tga, unsigned char* data)
{
	short offset, pixelSize;
	unsigned char *cur;
	unsigned long index = 0;
	unsigned char numPixels;
	int i;
 
	/* Set offset to start of image data (after header) */
	offset = data[0] + 18;
 
	/* Get pixel size in bytes */
	pixelSize = tga->bpp/8;
 
	/* Set pointer to the beginning of the image data */
	cur = &data[offset];
 
	tga->image = (unsigned char*)malloc(tga->imageSize);
 
	if (tga->image == NULL)
		return false;
 
	/* Decode RLE */
	while (index < tga->imageSize) 
	{
		if (*cur & 0x80) /* 10000000 */
		{
			/* Get number of repeated pixels */
			numPixels = *cur - 127;
			cur++; 
 
			/* Repeat the next pixel numPixels times */
			for (i = 0; i != numPixels; i++, index += pixelSize)
				memcpy(&tga->image[index], cur, pixelSize);
 
			cur += pixelSize;
		}
		else /* Raw data segment */
		{
			/* Get number of raw pixels */
			numPixels = *cur + 1; 
			cur++;
 
			for (i = 0; i != numPixels; i++, index += pixelSize, cur += pixelSize)
				memcpy(&tga->image[index], cur, pixelSize);
		}
	}
	return true;
}

void TGABGRtoRGB(ImageTGA *tga)
{
	unsigned long numPixels;
	unsigned char *cur;
	unsigned char temp;
	short pixelSize;
	int i;
 
	cur = tga->image;
	numPixels = tga->width * tga->height;
 
	/* Get pixel size in bytes */
	pixelSize = tga->bpp/8;
 
	for (i = 0; i != numPixels; i++) 
	{
		temp = *cur;		/* Get blue value */
		*cur = *(cur + 2);  /* Swap red value into first position */
		*(cur + 2) = temp;  /* Put blue value in last position */
 
		cur += pixelSize;
	}
}
