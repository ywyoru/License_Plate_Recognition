#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

/**************************************************
*   Sobel算子边缘检测
*   parameter: srcData - 原始图像数据指针
*              dstData - 存储处理后的图像
*              lWidth,lHeight - 图像的宽和高
*              dLineBites - 单行图像的字节数(4的倍数)
**************************************************/
void Sobel(Image* bmpImg) //LPBYTE srcData, LPBYTE dstData, LONG lWidth, LONG lHeight, DWORD dLineBites)
{
	int lHeight = bmpImg->height;
	int lWidth = bmpImg->width;
	int dLineBites = lWidth;//WIDTHBYTES(lWidth * 8);
	unsigned char* dstData;
	dstData = (unsigned char*)malloc(sizeof(unsigned char)*lWidth*lHeight);
	//memcpy(dstData, bmpImg->imageData, sizeof(unsigned char)*lWidth*lHeight);
	int KERNEL[4][9] = { { -1, 0, 1, -2, 0, 2, -1, 0, 1 },
	{ -1, -2, -1, 0, 0, 0, 1, 2, 1 },
	{ -2, -1, 0, -1, 0, 1, 0, 1, 2 },
	{ 0, -1, -2, 1, 0, -1, 2, 1, 0 } };
	int i, j, k, kernel_sum, total_sum = 0, nMax = 0;
	unsigned char* pTmpSrcData = NULL;
	unsigned char* pTmpDstData = NULL;
	for (i = 1; i < lHeight - 1; i++)
	{
		pTmpSrcData = bmpImg->imageData + dLineBites * i;
		pTmpDstData = dstData + dLineBites * i;
		for (j = 1; j < lWidth - 1; ++j)
		{
			total_sum = 0;
			nMax = 0;
			for (k = 0; k < 4; k++)
			{
				kernel_sum = *(pTmpSrcData + j - dLineBites - 1) * KERNEL[k][0] +
					*(pTmpSrcData + j - dLineBites) * KERNEL[k][1] +
					*(pTmpSrcData + j - dLineBites + 1) * KERNEL[k][2] +
					*(pTmpSrcData + j - 1) * KERNEL[k][3] +
					*(pTmpSrcData + j) * KERNEL[k][4] +
					*(pTmpSrcData + j + 1) * KERNEL[k][5] +
					*(pTmpSrcData + j + dLineBites - 1) * KERNEL[k][6] +
					*(pTmpSrcData + j + dLineBites) * KERNEL[k][7] +
					*(pTmpSrcData + j + dLineBites + 1) * KERNEL[k][8];
				kernel_sum = abs(kernel_sum);
				//total_sum += abs(kernel_sum);
				if (nMax < kernel_sum)
					nMax = kernel_sum;

			}
			if (nMax > 255)
				nMax = 255;
			*(pTmpDstData + j) = (unsigned char)nMax;
		}
	}
	memcpy(bmpImg->imageData, dstData, lWidth * lHeight);
	bool flag = SaveImage("result_Sobel.bmp", bmpImg);
	if (flag)
	{
		printf("save Sobel ok... \n");
	}
	// 释放内存
	free(dstData);
}


void SobelHorizontal(Image* bmpImg) //LPBYTE srcData, LPBYTE dstData, LONG lWidth, LONG lHeight, DWORD dLineBites)
{
	int ImageHeight = bmpImg->height;
	int ImageWidth = bmpImg->width;
	unsigned char* SobelImage;
	SobelImage = (unsigned char*)malloc(sizeof(unsigned char)*ImageWidth*ImageHeight);
	//memcpy(SobelImage, bmpImg->imageData, sizeof(unsigned char)*ImageWidth*ImageHeight);
#define SobelImage(i,j) SobelImage[(int)((i)*ImageWidth)+(int)(j)]
#define ImageData(i,j) bmpImg->imageData[(int)((i)*ImageWidth)+(int)(j)]

	int i, j, d, k, sum;
	int x, y;
	int temp[3][3] = { 
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1 };

	///////////对图象进行模板处理///////////
	for (k = 0; k<ImageHeight - 2; k++)
	{
		for (d = 0; d<ImageWidth - 2; d++)

		{
			sum = 0;
			for (x = 0; x<3; x++)
			{
				for (y = 0; y<3; y++)
				{
					sum += ImageData(k + x,d + y) * temp[x][y];
				}
			}
			SobelImage(k + 1,d + 1) = sum;
		}
	}
	memcpy(bmpImg->imageData, SobelImage, ImageWidth * ImageHeight);
	bool flag = SaveImage("result_SobelHorizontal.bmp", bmpImg);
	if (flag)
	{
		printf("save SobelHorizontal ok... \n");
	}
	// 释放内存
	free(SobelImage);
}

void SobelVertical(Image* bmpImg) //LPBYTE srcData, LPBYTE dstData, LONG lWidth, LONG lHeight, DWORD dLineBites)
{
	int ImageHeight = bmpImg->height;
	int ImageWidth = bmpImg->width;
	unsigned char* SobelImage;
	SobelImage = (unsigned char*)malloc(sizeof(unsigned char)*ImageWidth*ImageHeight);
	//memcpy(SobelImage, bmpImg->imageData, sizeof(unsigned char)*ImageWidth*ImageHeight);
#define SobelImage(i,j) SobelImage[(int)((i)*ImageWidth)+(int)(j)]
#define ImageData(i,j) bmpImg->imageData[(int)((i)*ImageWidth)+(int)(j)]

	int i, j, d, k, sum;
	int x, y;
	int temp[3][3] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1 };

	///////////对图象进行模板处理///////////
	for (k = 0; k<ImageHeight - 2; k++)
	{
		for (d = 0; d<ImageWidth - 2; d++)

		{
			sum = 0;
			for (x = 0; x<3; x++)
			{
				for (y = 0; y<3; y++)
				{
					sum += ImageData(k + x, d + y) * temp[x][y];
				}
			}
			SobelImage(k + 1, d + 1) = sum;
		}
	}
	memcpy(bmpImg->imageData, SobelImage, ImageWidth * ImageHeight);
	bool flag = SaveImage("result_SobelVertical.bmp", bmpImg);
	if (flag)
	{
		printf("save SobelVertical ok... \n");
	}
	// 释放内存
	free(SobelImage);
}