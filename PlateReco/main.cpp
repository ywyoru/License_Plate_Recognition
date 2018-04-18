//main.cpp
#include <stdio.h>
#include <stdlib.h>  
#include <time.h>
#include "img.h"  


int main()
{	
	char mode_name[20];
	do
	{
		printf("please select the mode:Color Threshold Division or Edge Detection(input number 1 or 2):");		//两种识别模式选择：1是颜色分割，只适用于蓝车牌；2是边缘提取，适用于所有颜色车牌
		gets(mode_name);
		printf("\nLet's start ! :)\n");
		if (*mode_name == '1')		//颜色分割方法
		{
			clock_t start, start_dilation, start_sobel, end, end_dilation, end_sobel;
			char YN[20];
			char img_name[20];
				printf("\nplease input the path of the image:");
				gets(img_name);
				start = clock();
				Image* img = LoadImage(img_name);
				Image* imgHSV = Rgb2Hsv(img);
				Image* imgPlate = PlateFilter(imgHSV);

				/****************************滤波器**********************/
				Gaussian(imgPlate);

				/*************************先膨胀后腐蚀**********************/

				start_dilation = clock();
				for (int i = 0; i < 5; i++)
				{
					Dilation(imgPlate, 0);
					Dilation(imgPlate, 1);
				}
				printf("save dilate ok... \n");
				end_dilation = clock();
				printf("//dilation time = %f\n", (double)(end_dilation - start_dilation) / CLOCKS_PER_SEC);
				Erosion(imgPlate, 1);

				/*************************Sobel边缘函数**********************/
				start_sobel = clock();
				Sobel(imgPlate);
				EdgeErasion(imgPlate, 10);		//去除边缘一圈白色
				end_sobel = clock();
				printf("//sobel time = %f\n", (double)(end_sobel - start_sobel) / CLOCKS_PER_SEC);

				/*************************连通域筛选**********************/
				//第一个参数：车牌最小宽度
				//第二个参数：车牌最小高度
				//第三个参数：车牌最小宽高比
				//第四个参数：车牌最大宽高比
				int width = img->width;
				int height = img->height;
				CRect* plateRectAll = find_connected_region_location(imgPlate, 90, 30, 2.0, 3.8);

				PickRec(img, imgPlate, plateRectAll);
				printf("\nsave Final ok!!!");

				end = clock();
				printf("\n//time_total= %f\n\n", (double)(end - start) / CLOCKS_PER_SEC);
				printf("\n");

		}
		else if (*mode_name == '2')		//边缘提取方法
		{
			clock_t start, start_dilation, start_sobel, end, end_dilation, end_sobel;
			char YN[20];
			char img_name[20];
				printf("please input the path of the image:");
				gets(img_name);
				start = clock();
				Image* img = LoadImage(img_name);
				Image* imgGray = Rgb2Gray(img);

				/***************************边缘函数**********************/
				Sobel(imgGray);

				/***************************滤波器************************/
				medianFilter(imgGray);

				/***************************阈值二值化**********************/
				thresholdSet(imgGray, 150);		//更好的话应该是自适应设定阈值，这里只是大致取了150

				/***************************腐蚀、膨胀**********************/
				start_dilation = clock();
				for (int i = 0; i < 3; i++)
				{
					Dilation(imgGray, 0);
				}
				printf("save dilate ok... \n");
				end_dilation = clock();
				printf("//dilation time = %f\n", (double)(end_dilation - start_dilation) / CLOCKS_PER_SEC);
				Erosion(imgGray, 1);

				start_sobel = clock();
				Sobel(imgGray);
				EdgeErasion(imgGray, 10);		//去除边缘一圈白色
				end_sobel = clock();
				printf("//sobel time = %f\n", (double)(end_sobel - start_sobel) / CLOCKS_PER_SEC);

				/*************************连通域筛选***********************/
				//第一个参数：车牌最小宽度
				//第二个参数：车牌最小高度
				//第三个参数：车牌最小宽高比
				//第四个参数：车牌最大宽高比
				CRect* plateRectAll = find_connected_region_location(imgGray, 90, 30, 2.3, 3.7);
				PickRec(img, imgGray, plateRectAll);
				printf("\nsave Final ok!!!");

				end = clock();
				printf("\n//time_total= %f\n\n", (double)(end - start) / CLOCKS_PER_SEC);
				printf("\n");
		}
	}while (1);
	return 0;
}