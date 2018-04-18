#include <stdio.h>
#include <stdlib.h>  
#include "img.h"  

Image* PlateFilter(Image* imgHSV)
{
	int h = imgHSV->height;
	int w = imgHSV->width;

	Image* dst;
	dst = (Image*)malloc(sizeof(Image));
	dst->height = h;
	dst->width = w;
	dst->channels = 1;
	dst->imageData = (unsigned char*)malloc(sizeof(unsigned char)*w * h);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0;  x < w; x++)
		{
			int bH=imgHSV->imageData[y*w * 3 + x * 3 + 0];  // H component
			int bS=imgHSV->imageData[y*w * 3 + x * 3 + 1];  // S component
			int bV=imgHSV->imageData[y*w * 3 + x * 3 + 2];  // V component

			if (   bH>=100 && bH<=230 && bS>=80 && bV>=65)	//二值化，符合该HSV范围的被设定为白色 (or bH>=100 && bH<=230 && bS>=70 && bV>=65)
				dst->imageData[(long)y*w + x] = 255;
			else
				dst->imageData[(long)y*w + x] = 0;
		}
	}
	bool flag = SaveImage("result_PlateFilter.bmp", dst);
	if (flag)
	{
		printf("save PlateFilter ok... \n");
	}
	return dst;
}