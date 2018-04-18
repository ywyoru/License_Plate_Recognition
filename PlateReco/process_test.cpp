#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"img.h"
//灰度化

/*
Image* image_gray(Image* bmpImg)
{
	int i, j;
	Image* bmpImg_gray;
	unsigned char* imageData_post;
	unsigned char* imageData = bmpImg->imageData;
	unsigned char tmp;
	int height = bmpImg->height;
	int width = bmpImg->width;
	imageData_post = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	for (i = 0; i<height; i++)
	{
		for (j = 0; j< width; j++)
		{
			tmp = 
				0.11*(*(imageData + i* width + j * 3 + 0)) 
				+ 0.59*(*(imageData + i*width + j * 3 + 1))
				+ 0.3*(*(imageData + i*width + j * 3 + 2));
			imageData_post[i*width + j * 3 + 0] = tmp;
			imageData_post[i*width + j * 3 + 1] = tmp;
			imageData_post[i*width + j * 3 + 2] = tmp;
		}
	}
	bmpImg_gray = (Image*)malloc(sizeof(Image));
	bmpImg_gray->height = height;
	bmpImg_gray->width = width;
	bmpImg_gray->channels = 1;
	bmpImg_gray->imageData = imageData_post;
	return bmpImg_gray;
}
*/

Image* image_gray(Image* bmpImg)
{

	//修改信息头  
	bitmapInfoHeader->biBitCount = 8;
	bitmapInfoHeader->biSizeImage = (bitmapInfoHeader->biHeight)*destImageLineByteCount;

	//修改文件头  
	bitmapFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	bitmapFileHeader->bfSize = bitmapFileHeader->bfOffBits + bitmapInfoHeader->biSizeImage;


	//写数据  

	BYTE** newImageData = new BYTE*[bitmapInfoHeader->biHeight];

	for (int i = 0; i < bitmapInfoHeader->biHeight; i++)
	{
		newImageData[i] = new BYTE[destImageLineByteCount];
	}

	for (int i = 0; i < bitmapInfoHeader->biHeight; i++)
	{
		for (int j = 0; j < destImageLineByteCount; j++)
		{
			newImageData[i][j] = (int)((float)oldImageData[i][j * 3] * 0.114 +
				(float)oldImageData[i][j * 3 + 1] * 0.587 + (float)oldImageData[i][3 * j + 2] * 0.299);
		}
	}
}