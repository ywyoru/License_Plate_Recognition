#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"  

void PickRec(Image* Img, Image* ImgPlate,CRect* rect)
{
	for (int num = 0; num < ImgPlate->region_num; num++)
	{
		int top = rect[num].top;
		int bottom = rect[num].bottom;
		int left = rect[num].left;
		int right = rect[num].right;

		int widthNew = right - left + 1;
		int heightNew = bottom - top + 1;

		Image* dstImage;				//ÍÚµÄ³µÅÆÊä³öÍ¼Ïñ
		dstImage = (Image*)malloc(sizeof(Image));
		dstImage->height = heightNew;
		dstImage->width = widthNew;
		dstImage->channels = 3;
		dstImage->imageData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * widthNew * heightNew);
#define dstData(i,j) dstImage->imageData[(int)((i)*widthNew * 3)+(int)(j)]
#define imgData(i,j) Img->imageData[(int)((i)*Img->width * 3)+(int)(j)]

		for (int i = 0; i < heightNew; i++)
		{
			for (int j = 0; j < widthNew; j++)
			{
				dstData(i, j * 3) = imgData(top + i, (left + j) * 3);
				dstData(i, j * 3 + 1) = imgData(top + i, (left + j) * 3 + 1);
				dstData(i, j * 3 + 2) = imgData(top + i, (left + j) * 3 + 2);
			}
		}
		char *str1 = "result_Final";
		char *str2 = ".bmp";
		char resultName[30];
		sprintf(resultName, "%s%d%s", str1,num,str2);

		bool flag = SaveImage(resultName, dstImage);
		if (!flag)
		{
			printf("\nsave Final fail!");
		}
		free(dstImage);
	}

}