#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"  

//统计每行每列白点数，只能找一个车牌
CRect* location(Image* ImgPlate,int yuzhi)
{
	CRect* rect;
	rect = (CRect*)malloc(sizeof(CRect));

	int HL, HH, VL, VH;

	int height = ImgPlate->height;
	int width = ImgPlate->width;
	int i, j, n, maxnum, flag = 0;
	//struct HSV *hsv;
	int tempCount[2000] = { 0 };
	//hsv = (struct HSV *)malloc(sizeof(struct HSV)*width*height);
	//hsvzation(image, hsv, width, height);
	unsigned char *tempData;
	tempData = (unsigned char *)malloc(sizeof(unsigned char)*height*width);
	memcpy(tempData, ImgPlate->imageData, height*width);

	//int lineBytes=(width*24+31)/32*4;
	
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			if (tempData[i*width + j] == 255)
			{
				tempCount[i]++;			//统计每一行的白色点数

			}
		}
	}
	edgesob8(tempData, width, height);

	maxnum = tempCount[0];
	for (i = 1; i<height; i++)
	{

		if (tempCount[i]>maxnum)
			maxnum = tempCount[i];		//找到白点数最多的那行
	}
	maxnum = maxnum / 3;
	for (i = 0; i<height; i++)
	{
		if (flag == 0)
		{
			if (tempCount[i]>maxnum)
			{
				HL = i - yuzhi;

				flag = 1;
			}
		}
		if (flag == 1)
		{

			if (tempCount[i] == 0)
			{
				HH = i + yuzhi;

				break;
			}
		}
	}

	memset(tempCount, 0x00, sizeof(int)*width);
	for (i = 0; i<width; i++)
	{
		for (j = 0; j<height; j++)
		{
			if (tempData[j*width + i] == 255)
			{
				tempCount[i]++;

			}
		}
	}
	flag = 0;
	maxnum = tempCount[0];


	for (i = 1; i<width; i++)
	{

		if (tempCount[i]>maxnum)
			maxnum = tempCount[i];
	}
	maxnum = maxnum / 3;
	for (i = 0; i<width; i++)
	{
		if (tempCount[i]>maxnum)
		{
			VL = i - yuzhi;

			break;
		}
	}

	for (i = width; i>VL; i--)
	{
		if (tempCount[i]>maxnum)
		{
			VH = i + yuzhi;

			break;
		}

	}
	printf("\nbottom = %d, top = %d, left = %d, right = %d", HH, HL, VL, VH);
	free(tempData);
	tempData = NULL;
	rect->top = HL;
	rect->bottom = HH;
	rect->left = VL;
	rect->right = VH;
	return rect;
}

void edgesob8(unsigned char *image, int width, int height)//8邻域
{

	//int sum1,sum2,sum;double gray;
	int i, j, logNum;
	//int p[8];
	unsigned char *tempCount;
	tempCount = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memset(tempCount, 0x00, sizeof(unsigned char)*width*height);
	for (i = 3; i<height - 2; i++)
		for (j = 3; j<width - 2; j++)
		{
			logNum = 16 * image[i*width + j] - image[(i - 2)*width + j] - image[(i - 1)*width + j - 1] - 2 * image[(i - 1)*width + j] - image[(i - 1)*width + j + 1] - image[i*width + j - 2] - 2 * image[i*width + j - 1] - 2 * image[i*width + j + 1] - image[i*width + j + 2] - image[(i + 1)*width + j - 1] - 2 * image[(i + 1)*width + j] - image[(i + 1)*width + j + 1] - image[(i + 2)*width + j];
			if (logNum > 0)

				tempCount[i*width + j] = 255;

			else

				tempCount[i*width + j] = 0;
		}
	memcpy(image, tempCount, width*height);
	free(tempCount);
	tempCount = NULL;

}




//只能找一个车牌
void TopBottom(Image* bmpImg, int* iTop, int* iBottom)  //LPSTR lpDIB, LONG width, LONG height,
{
	int height = bmpImg->height;
	int width = bmpImg->width;

	unsigned char* lpDIBBits;                   //指向DIB的象素的指针

	int LineBytes;                   // 图像每行的字节数
	unsigned char * lpSrc;             //指向原图像象素点的指针

	unsigned char pixel;				//像素值

	int i, j;

	// 计算结果
	int* iResult;

	int pzBottom, pzTop;

	bool findPZ = false;

	// 找到DIB图像象素起始位置
	lpDIBBits = bmpImg->imageData;//::FindDIBBits(lpDIB);

	// 计算图像每行的字节数
	LineBytes = width;// WIDTHBYTES(width * 8);

	iResult = new int[height];

	for (i = 0; i<height; i++)
		iResult[i] = 0;

	for (j = height / 7; j<height*0.8; j++)
	{
		iResult[j] = 0;
		for (i = 0; i<height; i++)
		{
			lpSrc = (unsigned char*)lpDIBBits + LineBytes*j + i;
			pixel = (unsigned char)(*lpSrc);
			if (pixel == 255)
			{
				iResult[j]++;
			}
		}
		if ((!findPZ) && iResult[j]>12)
		{
			pzBottom = j;
			findPZ = true;
			continue;
		}
		if (findPZ&&iResult[j]<12)
		{
			pzTop = j;
			break;
		}

	}
	if (pzTop - pzBottom < 40)
	{
		pzTop = pzTop + 55;
		pzBottom = pzBottom - 20;   //微量调整
	}
	*iBottom = height - pzBottom;
	//*iBottom=pzBottom;
	*iTop = height - pzTop;
	//*iTop=pzTop;
	//return true;
	printf("\ntop = %d,bottom = %d", *iTop,*iBottom);
}
void LeftRight(Image* bmpImg,int* iLeft, int* iRight)//LPSTR lpDIB, LONG width, LONG height,
{
	int height = bmpImg->height;
	int width = bmpImg->width;

	unsigned char* lpDIBBits;                   //指向DIB的象素的指针

	int LineBytes;                   // 图像每行的字节数
	unsigned char * lpSrc;             //指向原图像象素点的指针

	unsigned char pixel;				//像素值

	int i, j;

	// 计算结果
	int* iResult;

	int pzLeft, pzRight;

	bool findPZ = false;

	// 找到DIB图像象素起始位置
	lpDIBBits = bmpImg->imageData;//::FindDIBBits(lpDIB);

	// 计算图像每行的字节数
	LineBytes = width;// WIDTHBYTES(width * 8);

	iResult = new int[width];

	for (i = 0; i<width; i++)
		iResult[i] = 0;

	for (i = width / 7; i<width*0.8; i++)
	{
		iResult[i] = 0;
		for (j = 0; j<height; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + LineBytes*j + i;
			pixel = (unsigned char)(*lpSrc);
			if (pixel == 255)
			{
				iResult[i]++;
			}
		}
		if ((!findPZ) && iResult[i]>12)
		{
			pzLeft = i;
			findPZ = true;
			continue;
		}

		pzRight = pzLeft + 150;
	}

	*iLeft = pzLeft - 10;
	*iRight = pzRight + 10;
	printf("\nleft = %d,right = %d", *iLeft, *iRight);
}