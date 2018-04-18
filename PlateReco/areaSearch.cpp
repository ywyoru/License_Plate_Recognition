#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void PlateAreaSearch(Image *bmpImg, int minCount, int maxLen )
{
	int height = bmpImg->height;
	int width = bmpImg->width;
	unsigned char* jumpData;
	jumpData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memcpy(jumpData, bmpImg->imageData, sizeof(unsigned char)*width*height);
#define jumpData(i,j) jumpData[(int)((i)*width)+(int)(j)]
	int n;	//连续的跳变点数
	//int maxLen;	//最大允许跳变点间距（如）
	//int minCount;	//最小允许每行跳变点数（如18）
	int* start;	//某一段跳变点的起始坐标
	int* end; //某一段跳变点的终止坐标
	start = (int*)malloc(sizeof(int)*height);
	end = (int*)malloc(sizeof(int)*height);
	for (int i = 0; i < height;)
	{
		int count = 0;		//每行的跳变点数
		bool flag = 0;		//标志,0表示未找到起始点
		for (int j = 0; j < width; j++)
		{
			if (jumpData(i, j) == 255 && !flag)
			{
				start[i] = end[i] = j;
				flag = 1;
			}
			if (jumpData(i, j) == 255 && (j - start[i]) < maxLen)
			{
				end[i] = j;
				count++;
			}
			if (jumpData(i, j) == 255 && (j - start[i]) >= maxLen && count < minCount)
			{
				start[i] = end[i] = j;
				count = 0;
			}
			if (jumpData(i, j) == 255 && (j - start[i]) >= maxLen && count >= minCount)
				break;
		}
	}
	free(jumpData);
}

/*//////OpenCV
int PlateAreaSearch(Image *pImg_Image)
{
	// 检测是否有值
	if (pImg_Image == NULL)
	{
		return 0;
	}

	Image* imgTest = 0;
	int i = 0, j = 0, k = 0, m = 0;
	bool flag = 0;
	int plate_n = 0;  //上边界
	int plate_s = 0;  //下边界
	int plate_e = 0;  //右边界
	int plate_w = 0;  //左边界

	int *num_h = new int[MAX(pImg_Image->width, pImg_Image->height)];
	if (num_h == NULL)
	{
		printf("\nmemory exhausted!");
		return 0;
	}

	//初始化分配的空间
	for (i = 0; i<pImg_Image->width; i++)
	{
		num_h[i] = 0;
	}

	imgTest = (Image*)malloc(sizeof(Image)); //cvCreateImage(cvSize(pImg_Image->width, pImg_Image->height), IPL_DEPTH_8U, 1);
	//cvCopy(pImg_Image, imgTest);
	memcpy(imgTest, pImg_Image, sizeof(pImg_Image));
	int width = imgTest->width;
	int height = imgTest->height;
	//--水平轮廓细化
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width - 1; i++)
		{
			//CV_IMAGE_ELEM(imgTest, uchar, j, i) = CV_IMAGE_ELEM(imgTest, uchar, j, i + 1) - CV_IMAGE_ELEM(imgTest, uchar, j, i);
			imgTest->imageData[j*width + i] = imgTest->imageData[j*width + i + 1] - imgTest->channels[j*width + i];
			//记录每一行的像素值
			num_h[j] += imgTest->imageData[j*width + i] / 250;  //CV_IMAGE_ELEM(imgTest, uchar, j, i) / 250;
		}
	}

	int temp_1 = 0;  //统计20行中最大的每行数据量
	int temp_max = 0;  //20行最大的数据量 
	int temp_i = 0;  //最大数据量的行

	for (j = 0; j<height - 20; j++)
	{
		temp_1 = 0;
		for (i = 0; i<20; i++)
			temp_1 += num_h[i + j];
		if (temp_1 >= temp_max)
		{
			temp_max = temp_1;
			temp_i = j;
		}
	}

	//找出上行边界行
	k = temp_i;
	while (((num_h[k + 1]>POINT_X) || (num_h[k + 2]>POINT_X) || (num_h[k]>POINT_X)) && k) k--;
	plate_n = k + 1;

	//找出下边界行
	k = temp_i + 10;
	while (((num_h[k - 1]>POINT_X) || (num_h[k - 2]>POINT_X) || (num_h[k]>POINT_X)) && (k<imgTest->height)) k++;
	plate_s = k;

	//没找到水平分割线，设置为默认值
	if (!(plate_n && plate_s
		&& (plate_n<plate_s) && ((plate_s - plate_n)*HIGH_WITH_CAR<imgTest->width*(1 - WITH_X))))
	{
		printf("\n水平分割失败!");
		return 0;
	}
	//找到水平线 
	else
	{
		int  max_count = 0;
		int  plate_length = (imgTest->width - (plate_s - plate_n)*HIGH_WITH_CAR);
		plate_w = imgTest->width*WITH_X - 1;//车牌宽度 默认

		//--垂直方向 轮廓细化
		for (i = 0; i<imgTest->width; i++)
			for (j = 0; j<imgTest->height - 1; j++)
			{
				CV_IMAGE_ELEM(imgTest, uchar, j, i) = CV_IMAGE_ELEM(imgTest, uchar, j + 1, i) - CV_IMAGE_ELEM(imgTest, uchar, j, i);
			}

		for (k = 0; k<plate_length; k++)
		{
			for (i = 0; i<(int)((plate_s - plate_n)*HIGH_WITH_CAR); i++)
				for (j = plate_n; j<plate_s; j++)//两水平线之间 
				{
					num_h[k] = num_h[k] + CV_IMAGE_ELEM(imgTest, uchar, j, (i + k)) / 250;
				}
			if (num_h[k]>max_count)
			{
				max_count = num_h[k];
				plate_w = k;
			}

		}


		CvRect ROI_rect;                 //获得图片感兴趣区域
		ROI_rect.x = plate_w;
		ROI_rect.y = plate_n;
		ROI_rect.width = (plate_s - plate_n)*HIGH_WITH_CAR;
		ROI_rect.height = plate_s - plate_n;

		if ((ROI_rect.width + ROI_rect.x)> pImg_Image->width)
		{
			ROI_rect.width = pImg_Image->width - ROI_rect.x;
			MessageBox("垂直方向分割失败!");
			return 0;
		}

		else
		{
			IplImage *pImg8uROI = NULL;         //感兴趣的图片  
			pImg8uROI = cvCreateImage(cvSize(ROI_rect.width, ROI_rect.height), src->depth, src->nChannels);

			IplImage *pImg8u11 = NULL;        //车牌区域灰度图
			pImg8u11 = cvCreateImage(cvSize(40 * HIGH_WITH_CAR, 40), pImg8uROI->depth, pImg8uROI->nChannels);

			cvSetImageROI(src, ROI_rect);
			cvCopy(src, pImg8uROI, NULL);
			cvResetImageROI(src);

			pImgResize = cvCreateImage(cvSize(40 * HIGH_WITH_CAR, 40), IPL_DEPTH_8U, 1);
			cvResize(pImg8uROI, pImg8u11, CV_INTER_LINEAR); //线性插值

			cvCvtColor(pImg8u11, pImgResize, CV_RGB2GRAY);
			Threshold(pImgResize, pImgResize);

			cvReleaseImage(&pImg8uROI);
			cvReleaseImage(&pImg8u11);
			cvReleaseImage(&imgTest);
		}
	}

	// 释放内存
	delete[]num_h;
	num_h = NULL;
	return 1;
}
*/