#include <stdio.h>
#include <stdlib.h>  
#include "img.h"  

void InteEqualize(Image* bmpImg)//LPSTR lpDIBBits, int width, int height)
{
	// 指向源图像的指针
	unsigned char*	lpSrc;
	//Image* InteEquaImg;
	// 临时变量
	int	i,j,temp;
	int height = bmpImg->height;
	int width = bmpImg->width;
	// 灰度映射表
	unsigned char bMap[256];
	// 灰度计数表
	int	Count[256];
	// 图像每行的字节数
	int	LineBytes;
	// 计算图像每行的字节数
	LineBytes = width;// WIDTHBYTES(width * 8);

	// 重置计数为0
	for (i = 0; i < 256; i++)
	{
		// 清零
		Count[i] = 0;
	}

	// 计算各个灰度值的计数
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			lpSrc = bmpImg->imageData + LineBytes * i + j;
			// 计数加1
			Count[*(lpSrc)]++;
		}
	}

	// 计算灰度映射表
	for (i = 0; i < 256; i++)
	{
		// 初始为0
		temp = 0;

		for (j = 0; j <= i; j++)
		{
			temp += Count[j];
		}

		// 计算对应的新灰度值
		bMap[i] = (unsigned char)(temp * 255 / (height * width));
	}

	// 每行
	for (i = 0; i < height; i++)
	{
		// 每列
		for (j = 0; j < width; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = bmpImg->imageData + LineBytes * (height - 1 - i) + j;

			// 计算新的灰度值
			*lpSrc = bMap[*lpSrc];
		}
	}
	bool flag = SaveImage("result_equalized.bmp", bmpImg);
	if (flag)
	{
		printf("save equalized ok... \n");
	}
	// 返回
	//return InteEquaImg;
}