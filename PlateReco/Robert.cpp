#include <stdio.h>
#include <stdlib.h>  
#include <math.h>
#include <string.h>
#include "img.h"  

void Robert(Image* bmpImg)
{
	// 指向源图像的指针
	unsigned char*	lpSrc;

	// 指向缓存图像的指针
	unsigned char*	lpDst;

	// 指向缓存DIB图像的指针
	unsigned char*	lpNewDIBBits;
	//void*	hNewDIBBits;

	//循环变量
	long i, j;

	//像素值
	double result;
	unsigned char pixel[4];

	int height = bmpImg->height;
	int width = bmpImg->width;

	// 暂时分配内存，以保存新图像
	//hNewDIBBits = (void*)malloc(sizeof(unsigned char)*width*height);  // LocalAlloc(LHND, lWidth * lHeight);

	/*if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		printf("分配内存失败");
	}
	*/

	// 锁定内存
	//lpNewDIBBits = (char *)LocalLock(hNewDIBBits);
	lpNewDIBBits = (unsigned char*)malloc(sizeof(unsigned char)*width*height);

	// 初始化新分配的内存，设定初始值为255
	lpDst = lpNewDIBBits;
	memset(lpDst, (unsigned char)255, width * height);


	//使用水平方向的结构元素进行腐蚀
	for (j = height - 1; j > 0; j--)
	{
		for (i = 0; i <width - 1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			lpSrc = bmpImg->imageData + width * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			lpDst = lpNewDIBBits + width * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0] = (unsigned char)*lpSrc;
			pixel[1] = (unsigned char)*(lpSrc + 1);
			pixel[2] = (unsigned char)*(lpSrc - width);
			pixel[3] = (unsigned char)*(lpSrc - width + 1);

			//计算目标图像中的当前点
			result = sqrt((float)(pixel[0] - pixel[3])*(pixel[0] - pixel[3]) + \
				(pixel[1] - pixel[2])*(pixel[1] - pixel[2]));
			*lpDst = (unsigned char)result;

		}
	}

	// 复制腐蚀后的图像
	memcpy(bmpImg->imageData, lpNewDIBBits, width * height);
	bool flag = SaveImage("result_Robert.bmp", bmpImg);
	if (flag)
	{
		printf("save Robert ok... \n");
	}
	// 释放内存
	//LocalUnlock(hNewDIBBits);
	free(lpNewDIBBits);
}
