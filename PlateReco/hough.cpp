#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void Hough(Image* srcImg,int yuzhi)
{
	int height = srcImg->height;
	int width = srcImg->width;

	unsigned char* dstData;
	dstData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	//memcpy(dstData, srcImg->imageData, sizeof(unsigned char)*width*height);

#define pi 3.1416
#define mp sqrt((float)height*height+width*width)
#define ma 180
	int i, j, k, p;
	int npp[ma][1000];
	for (i = 1; i < height; i++) //height原图高度

		for (j = 1; j < width; j++) //width 原图宽度

		{

			if (srcImg->imageData[i*width + j] == 255) //对边缘检测后的数据（存在lpDIBBits中）进行hough变化

			{

				for (k = 1; k < ma; k++) //ma=180

				{

					p = (int)(i*cos(pi*k / 180) + j*sin(pi*k / 180));//p hough变换中距离参数

					p = (int)(p / 2 + mp / 2); //对p值优化防止为负

					npp[k][p] = npp[k][p]++; //npp对变换域中对应重复出现的点累加

				}

			}

		}

	int kmax = 0; //最长直线的角度

	int pmax = 0; //最长直线的距离

	int n = 0; //这一部分为寻找最长直线

	for (i = 1; i < ma; i++) //ma=180

		for (j = 1; j<mp; j++) //mp为原图对角线距离

		{

			if (npp[i][j]>yuzhi) //找出最长直线 yuzhi为中间变量用于比较

			{

				yuzhi = npp[i][j];

				kmax = i; //记录最长直线的角度

				pmax = j; //记录最长直线的距离

			}

		}

	memset(dstData, (unsigned char)0, sizeof(unsigned char)*width*height);//原图中坐标符合kmax和pmax的值的集合即是最长的直线

	for (i = 1; i < height; i++) 

		for (j = 1; j < width; j++)

		{

			if (srcImg->imageData[i*width + j] == 255)

			{

				p = (int)(i*cos(pi*kmax / 180) + j*sin(pi*kmax / 180));//pi=3.1415926

				p = (int)(p / 2 + mp / 2); //mp为原图对角线距离

				if (p == pmax)

					dstData[ i*width + j] = 255; //存储图像数据 放在temp数组中

			}

		}
	memcpy(srcImg->imageData, dstData, width * height);
	bool flag = SaveImage("result_hough.bmp", srcImg);
	if (flag)
	{
		printf("save jumpPoint ok... \n");
	}
	// 释放内存
	free(dstData);
}