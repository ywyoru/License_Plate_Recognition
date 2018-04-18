#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

/*************************************************************************
* 函数名称：
*   Dilation()
* 参数:
*   unsigned char* lpDIBBits    - 指向源DIB图像指针
*   int  lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int  lHeight      - 源图像高度（象素数）
*   int   nMode		- 膨胀方式：0表示水平方向，1垂直方向，2自定义结构元素。
*	 int   structure[3][3]	- 自定义的3×3结构元素。
* 返回值:
*   BOOL               - 膨胀成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
************************************************************************/
void Dilation(Image* srcImage, int nMode)//int structure[3][3])
{
	int lHeight = srcImage->height;
	int lWidth = srcImage->width;
	unsigned char* lpDIBBits = srcImage->imageData;
	unsigned char*	lpSrc;			// 指向源图像的指针	
	unsigned char*	lpDst;			// 指向缓存图像的指针	
	unsigned char*	lpNewDIBBits;	// 指向缓存DIB图像的指针
	//HLOCAL	hNewDIBBits;
	long i, j, n, m;			//循环变量
	unsigned char pixel;	//像素值

	int structure[15][15] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1,1},
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1 },
	};

	// 暂时分配内存，以保存新图像
	//hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	lpNewDIBBits = (unsigned char *)malloc(sizeof(unsigned char)*lWidth*lHeight);

	// 初始化新分配的内存，设定初始值为255
	lpDst = lpNewDIBBits;
	memset(lpDst, 255, lWidth * lHeight);

	if (nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for (j = 0; j <lHeight; j++)
		{
			for (i = 1; i <lWidth - 1; i++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
				// 的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者左右有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == 255)
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
			}
		}
	}
	else if (nMode == 1)
	{
		//使用垂直方向的结构元素进行膨胀
		for (j = 1; j <lHeight - 1; j++)
		{
			for (i = 0; i <lWidth; i++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
				// 的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果源图像中当前点自身或者上下有一个点不是黑色，
				//则将目标图像中的当前点赋成白色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1)*lWidth);
					if (pixel == 255)
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
			}
		}
	}
	else
	{
		//使用自定义的结构元素进行膨胀
		for (j = 7; j <lHeight - 7; j++)
		{
			for (i = 7; i <lWidth-7; i++)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
				// 的两列像素和最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中的当前点先赋成黑色
				*lpDst = (unsigned char)0;

				//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
				//则将目标图像中的当前点赋成白色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0; m < 15; m++)
				{
					for (n = 0; n < 15; n++)
					{
						if (structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((14 - m) - 7)*lWidth + (n - 7));
						if (pixel == 255)
						{
							*lpDst = (unsigned char)255;
							break;
						}
					}
				}
			}
		}
	}

	// 复制膨胀后的图像

	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	bool flag = SaveImage("result_dilate.bmp", srcImage);
	if (!flag)
	{
		printf("Dilation Fail!");
	}
	free(lpNewDIBBits);
}

/*************************************************************************
* 函数名称：
*   Erosion()
* 参数:
*   unsigned char* lpDIBBits    - 指向源DIB图像指针
*   int  lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int  lHeight      - 源图像高度（象素数）
*   int   nMode		- 腐蚀方式：0表示水平方向，1垂直方向，2自定义结构元素。
*	 int   structure[3][3]	- 自定义的3×3结构元素。
* 返回值:
*   BOOL               - 腐蚀成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
************************************************************************/

void Erosion(Image* srcImage, int nMode)
{
	int lHeight = srcImage->height;
	int lWidth = srcImage->width;
	unsigned char* lpDIBBits = srcImage->imageData;
	unsigned char*	lpSrc;			// 指向源图像的指针	
	unsigned char*	lpDst;			// 指向缓存图像的指针	
	unsigned char*	lpNewDIBBits;	// 指向缓存DIB图像的指针

	long i, j, m, n;			//循环变量	
	unsigned char pixel;	//像素值

	int structure[3][5] = {
		{1,1,1,1,1},
		{1,1,1,1,1},
		{1,1,1,1,1}
	};

	// 暂时分配内存，以保存新图像
	//hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	lpNewDIBBits = (unsigned char *)malloc(sizeof(unsigned char)*lWidth*lHeight);

	// 初始化新分配的内存，设定初始值为255
	lpDst = lpNewDIBBits;
	memset(lpDst, 255, lWidth * lHeight);

	if (nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for (j = 0; j <lHeight; j++)
		{
			for (i = 1; i <lWidth - 1; i++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
				// 的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者左右只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == 0)
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
			}
		}
	}
	else if (nMode == 1)
	{
		//使用垂直方向的结构元素进行腐蚀
		for (j = 1; j <lHeight - 1; j++)
		{
			for (i = 0; i <lWidth; i++)
			{
				// 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
				// 的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中的当前点先赋成白色
				*lpDst = (unsigned char)255;

				//源图像中当前点自身或者上下只要有一个点是黑色，
				//则将目标图像中的当前点赋成黑色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1)*lWidth);
					if (pixel == 0)
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
			}
		}
	}
	else
	{
		//使用自定义的结构元素进行腐蚀
		for (j = 1; j <lHeight - 1; j++)
		{
			for (i = 2; i <lWidth-2; i++)
			{
				// 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
				// 的两列像素和最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = lpDIBBits + lWidth * j + i;
				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = lpNewDIBBits + lWidth * j + i;
				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = *lpSrc;

				//目标图像中的当前点先赋成白色
				*lpDst = 255;

				//原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
				//则将目标图像中的当前点赋成黑色
				//注意在DIB图像中内容是上下倒置的
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 5; n++)
					{
						if (structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2 - m) - 1)*lWidth + (n - 2));
						if (pixel == 0)
						{
							*lpDst = (unsigned char)0;
							break;
						}
					}
				}
			}
		}
	}
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	bool flag = SaveImage("result_erode.bmp", srcImage);
	if (flag)
	{
		printf("save erode ok... \n");
	}
	free(lpNewDIBBits);
}

//EdgeErasion()去除腐蚀后的一圈白色边缘
void EdgeErasion(Image* srcimage,int threshold)
{
	int h = srcimage->height;
	int w = srcimage->width;
	unsigned char* dstData;
	dstData = (unsigned char*)malloc(sizeof(unsigned char)*w*h);
	memcpy(dstData, srcimage->imageData, w*h);

	int i, j;
	for (j = 0; j < threshold; j++)
		for (i = 0; i < w; i++)
		{
			dstData[j*w + i] = (unsigned char)0;
		}
	for (j = h - threshold; j < h; j++)
		for (i = 0; i < w; i++)
		{
			dstData[j*w + i] = (unsigned char)0;
		}
	for (i = 0; i < threshold; i++)
		for (j = threshold; j < h - threshold; j++)
		{
			dstData[j*w + i] = 0;
		}
	for (i = w - threshold; i < w; i++)
		for (j = threshold; j < h - threshold; j++)
		{
			dstData[j*w + i] = 0;
		}
	memcpy(srcimage->imageData, dstData, w*h);
	bool flag = SaveImage("result_erase.bmp", srcimage);
	if (flag)
	{
		printf("save erase ok... \n");
	}
	free(dstData);
}   

/*
void erode_image(Image * src)
{
	Image * dst;
	dst = (Image*)malloc(sizeof(Image));
	if (src == NULL || dst == NULL)
		return;

	int width = src->width;
	int height = src->height;

	//水平方向的腐蚀  
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 1; j < src->width - 1; j++)
		{
			//  data = ((unsigned char *)(src->imageData + src->width * i))[j];  
			if (((unsigned char *)(src->imageData + src->width * i))[j] == 0)
			{
				((unsigned char *)(dst->imageData + dst->width * i))[j] = 0;
				for (int k = 0; k < 3; k++)
				{
					//  data = ((unsigned char *)(src->imageData + src->width * i))[j + k -1];  
					if (((unsigned char *)(src->imageData + src->width * i))[j + k - 1] == 255)
					{
						((unsigned char *)(dst->imageData + dst->width * i))[j] = 255;
						break;
					}
				}
			}
			else
				((unsigned char *)(dst->imageData + dst->width * i))[j] = 255;
		}
	}
	//垂直方向的腐蚀  
	for (int i = 0; i < dst->width; i++)
	{
		for (int j = 1; j < dst->height - 1; j++)
		{
			//  data = ((unsigned char *)(src->imageData + src->width * i))[j];  
			if (((unsigned char *)(dst->imageData + dst->width * j))[i] == 0)
			{
				((unsigned char *)(src->imageData + src->width * j))[i] = 0;
				for (int k = 0; k < 3; k++)
				{
					//  data = ((unsigned char *)(src->imageData + src->width * i))[j + k -1];  
					if (((unsigned char *)(dst->imageData + dst->width * (j + k - 1)))[i] == 255)
					{
						((unsigned char *)(src->imageData + src->width * j))[i] = 255;
						break;
					}
				}
			}
			else
				((unsigned char *)(src->imageData + src->width * j))[i] = 255;
		}
	}
	memcpy(src->imageData, dst->imageData, src->height * src->width);
	bool flag = SaveImage("result_erode.bmp", dst);
	if (flag)
	{
		printf("save erode ok... \n");
	}
	free(dst);
}

void dilate_image(Image * src)
{
	Image * dst;
	dst = (Image*)malloc(sizeof(Image));
	if (src == NULL || dst == NULL)
		return;

	int width = src->width;
	int height = src->height;

	//水平方向的膨胀  
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 1; j < src->width - 1; j++)
		{
			if (((unsigned char *)(src->imageData + src->width * i))[j] == 255)
			{
				((unsigned char *)(dst->imageData + dst->width * i))[j] = 255;
				for (int k = 0; k < 3; k++)
				{
					if (((unsigned char *)(src->imageData + src->width * i))[j + k - 1] == 0)
					{
						((unsigned char *)(dst->imageData + dst->width * i))[j] = 0;
						break;
					}
				}
			}
			else
				((unsigned char *)(dst->imageData + dst->width * i))[j] = 0;
		}
	}
	//垂直方向的膨胀  
	for (int i = 0; i < dst->width; i++)
	{
		for (int j = 1; j < dst->height - 1; j++)
		{
			//  data = ((unsigned char *)(src->imageData + src->width * i))[j];  
			if (((unsigned char *)(dst->imageData + dst->width * j))[i] == 255)
			{
				((unsigned char *)(src->imageData + src->width * j))[i] = 255;
				for (int k = 0; k < 3; k++)
				{
					//  data = ((unsigned char *)(src->imageData + src->width * i))[j + k -1];  
					if (((unsigned char *)(dst->imageData + dst->width * (j + k - 1)))[i] == 0)
					{
						((unsigned char *)(src->imageData + src->width * j))[i] = 0;
						break;
					}
				}
			}
			else
				((unsigned char *)(src->imageData + src->width * j))[i] = 0;
		}
	}
	memcpy(src->imageData, dst->imageData, src->height * src->width);
	bool flag = SaveImage("result_dilate.bmp", dst);
	if (flag)
	{
		printf("save dilate ok... \n");
	}
	free(dst);
}
*/