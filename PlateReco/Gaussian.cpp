#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void Gaussian(Image* bmpImg)
{
	unsigned char* corrupted = bmpImg->imageData;
	int height = bmpImg->height;
	int width = bmpImg->width;
	unsigned char* smooth;
	smooth = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	int templates[25] = { 
		1, 4, 7, 4, 1,
		4, 16, 26, 16, 4,
		7, 26, 41, 26, 7,
		4, 16, 26, 16, 4,
		1, 4, 7, 4, 1 };
	memcpy(smooth, corrupted, width*height*sizeof(unsigned char));
	for (int j = 2; j<height - 2; j++)
	{
		for (int i = 2; i<width - 2; i++)
		{
			int sum = 0;
			int index = 0;
			for (int m = j - 2; m<j + 3; m++)
			{
				for (int n = i - 2; n<i + 3; n++)
				{
					sum += corrupted[m*width + n] * templates[index++];
				}
			}
			sum /= 273;
			if (sum > 255)
				sum = 255;
			smooth[j*width + i] = sum;
		}
	}
	memcpy(bmpImg->imageData, smooth, width * height);
	bool flag = SaveImage("result_Gaussian.bmp", bmpImg);
	if (flag)
	{
		printf("save Gaussian ok... \n");
	}
	// 释放内存
	free(smooth);
}

/*
void Gaussian(Image* bmpImg)
{
	double nSigma = 0.2;
	int nWindowSize = 1 + 2 * ceil(3 * nSigma);//通过sigma得到窗口大小
	int nCenter = nWindowSize / 2;
	int nWidth = bmpImg->width;
	int nHeight = bmpImg->height;
	int widthStep = WIDTHBYTES(nWidth * 8);
	unsigned char * pGaussian;
	pGaussian = (unsigned char*)malloc(sizeof(unsigned char)*nWidth*nHeight);//cvCreateImage(cvGetSize(ColorImage), ColorImage->depth, 1);
	//生成二维滤波核
	double *pKernel_2 = new double[nWindowSize*nWindowSize];
	double d_sum = 0.0;
	for (int i = 0; i < nWindowSize; i++)
	{
		for (int j = 0; j < nWindowSize; j++)
		{
			double n_Disx = i - nCenter;//水平方向距离中心像素距离
			double n_Disy = j - nCenter;
			pKernel_2[j*nWindowSize + i] = exp(-0.5*(n_Disx*n_Disx + n_Disy*n_Disy) / (nSigma*nSigma)) / (2.0*3.1415926)*nSigma*nSigma;
			d_sum = d_sum + pKernel_2[j*nWindowSize + i];
		}
	}
	for (int i = 0; i < nWindowSize; i++)//归一化处理
	{
		for (int j = 0; j < nWindowSize; j++)
		{
			pKernel_2[j*nWindowSize + i] = pKernel_2[j*nWindowSize + i] / d_sum;
		}
	}
	//输出模板
	for (int i = 0; i < nWindowSize*nWindowSize; i++)
	{
		if (i % (nWindowSize) == 0)
		{
			printf("\n");
		}
		printf("%.10f ", pKernel_2[i]);
	}
	printf("\n");
	//滤波处理
	for (int s = 0; s < nWidth; s++)
	{
		for (int t = 0; t < nHeight; t++)
		{
			double dFilter = 0;
			double dSum = 0;
			//当前坐标（s,t）
			//获取8邻域
			for (int x = -nCenter; x <= nCenter; x++)
			{
				for (int y = -nCenter; y <= nCenter; y++)
				{
					if ((x + s >= 0) && (x + s < nWidth) && (y + t >= 0) && (y + t < nHeight))//判断是否越界
					{
						double currentvalue = (double)bmpImg->imageData[(y + t)*widthStep + x + s];
						dFilter += currentvalue*pKernel_2[x + nCenter + (y + nCenter)*nCenter];
						dSum += pKernel_2[x + nCenter + (y + nCenter)*nCenter];
					}
				}
			}
			pGaussian[t*widthStep + s] = (unsigned char)(dFilter / dSum);
		}
	}
	memcpy(bmpImg->imageData, pGaussian, nWidth * nHeight);
	bool flag = SaveImage("result_Gaussian.bmp", bmpImg);
	if (flag)
	{
		printf("save Gaussian ok... \n");
	}
	// 释放内存
	//LocalUnlock(hNewDIBBits);
	free(pGaussian);
}
*/