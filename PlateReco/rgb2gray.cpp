#include <stdio.h>
#include <stdlib.h>  
#include "img.h"  

Image* Rgb2Gray(Image* bmpImg)
{
	Image* grayImg;
	int i, j, temp;
	int height = bmpImg->height;
	int width = bmpImg->width;
	grayImg = (Image*)malloc(sizeof(Image));
	grayImg->height = height;
	grayImg->width = width;
	grayImg->channels = 1;
	grayImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width * height);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			temp = (unsigned char)
				0.114*bmpImg->imageData[(long)i*width * 3 + 3 * j]
				+ 0.588*bmpImg->imageData[(long)i*width * 3 + 3 * j + 1]
				+ 0.299*bmpImg->imageData[(long)i*width * 3 + 3 * j + 2];	//¼ò»¯¹«Ê½Gray = £¨76*R + 150*G + 30*B£©>> 8
			grayImg->imageData[(long)i*width + j] = temp;
		}
  	}
	bool flag = SaveImage("result_gray.bmp", grayImg);
	if (flag)
	{
		printf("save gray ok... \n");
	}
	return grayImg;
}