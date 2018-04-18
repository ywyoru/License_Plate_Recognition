#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void grayStretch(Image* src, int nMin, int nMax)
{
	int height = src->height;
	int width = src->width;
	unsigned char* dstData;
	dstData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	int low_value = nMin;    //拉伸后像素的最小值   
	int high_value = nMax;   //拉伸后像素的最大值  

	float rate = 0;          //图像的拉伸率  

	float stretch_p[256], stretch_p1[256], stretch_num[256];
	//清空三个数组,初始化填充数组元素为0  
	memset(stretch_p, 0, sizeof(stretch_p));
	memset(stretch_p1, 0, sizeof(stretch_p1));
	memset(stretch_num, 0, sizeof(stretch_num));
	//统计图像各个灰度级出现的次数  
	unsigned char* srcData = (unsigned char*)src->imageData;
	int nHeight = src->height;
	int nWidth = src->width;
	int i, j;
	unsigned char nVal = 0;
	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			nVal = srcData[i*nWidth + j];
			stretch_num[nVal]++;
		}
	}
	//统计各个灰度级出现的概率  
	for (i = 0; i<256; i++)
	{
		stretch_p[i] = stretch_num[i] / (nHeight*nWidth);
	}
	//统计各个灰度级的概率和  
	for (i = 0; i<256; i++)
	{
		for (j = 0; j <= i; j++)
		{
			stretch_p1[i] += stretch_p[j];
		}
	}
	//计算两个阈值点的值  
	for (i = 0; i<256; i++)
	{
		if (stretch_p1[i]<0.1)     //low_value取值接近于10%的总像素的灰度值  
		{
			low_value = i;
		}
		if (stretch_p1[i]>0.9)     //high_value取值接近于90%的总像素的灰度值  
		{
			high_value = i;
			break;
		}
	}
	rate = (float)255 / (high_value - low_value + 1);
	//进行灰度拉伸  
	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			nVal = srcData[i*nWidth + j];
			if (nVal<low_value)
			{
				dstData[i*nWidth + j] = 0;
			}
			else if (nVal>high_value)
			{
				dstData[i*nWidth + j] = 255;
			}
			else
			{
				dstData[i*nWidth + j] = (unsigned char)((nVal - low_value)*rate + 0.5);
				if (dstData[i*nWidth + j]>255)
				{
					dstData[i*nWidth + j] = 255;
				}
			}
		}
	}
	memcpy(src->imageData, dstData, width * height);
	bool flag = SaveImage("result_grayStretch.bmp", src);
	if (flag)
	{
		printf("save grayStretch ok... \n");
	}
	// 释放内存
	free(dstData);
}