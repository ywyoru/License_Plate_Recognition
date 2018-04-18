#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void horizonGrad(Image* bmpImg,int threshold)
{
	int height = bmpImg->height;
	int width = bmpImg->width;
	unsigned char* gradData;
	gradData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memcpy(gradData, bmpImg->imageData, sizeof(unsigned char)*width*height);
#define gradData(i,j) gradData[(int)((i)*width)+(int)(j)]
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width-1; j++)
		{
			gradData(i, j) = abs(bmpImg->imageData[i*width+j] - bmpImg->imageData[i*width+j + 1]);
			if (gradData(i, j) > threshold)
				gradData(i, j) = 255;
			else
				gradData(i, j) = 0;
		}
	memcpy(bmpImg->imageData, gradData, width * height);
	bool flag = SaveImage("result_grad.bmp", bmpImg);
	if (flag)
	{
		printf("save grad ok... \n");
	}
	// 释放内存
	free(gradData);
}

void JumpPoint(Image* bmpImg)
{
	int height = bmpImg->height;
	int width = bmpImg->width;
	unsigned char* jumpData;
	jumpData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memcpy(jumpData, bmpImg->imageData, sizeof(unsigned char)*width*height);
#define jumpData(i,j) jumpData[(int)((i)*width)+(int)(j)]
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width-1; j++)
		{
			jumpData(i, j) = abs(bmpImg->imageData[i*width + j] - bmpImg->imageData[i*width + j + 1]);
		}
	memcpy(bmpImg->imageData, jumpData, width * height);
	bool flag = SaveImage("result_jumpPoint.bmp", bmpImg);
	if (flag)
	{
		printf("save jumpPoint ok... \n");
	}
	// 释放内存
	free(jumpData);
}