#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include "img.h"  

/**
** method to remove noise from the corrupted image by median value
* @param corrupted input grayscale binary array with corrupted info
* @param smooth output data for smooth result, the memory need to be allocated outside of the function
* @param width width of the input grayscale image
* @param height height of the input grayscale image
*/
void medianFilter(Image* bmpImg)//unsigned char* corrupted, unsigned char* smooth, int width, int height)
{
	int height = bmpImg->height;
	int width = bmpImg->width;
	unsigned char* corrupted = bmpImg->imageData;
	unsigned char* smooth;
	smooth = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	memcpy(smooth, bmpImg->imageData, width*height*sizeof(unsigned char));
	for (int j = 1; j<height - 1; j++)
	{
		for (int i = 1; i<width - 1; i++)
		{
			int k = 0;
			unsigned char window[9];
			for (int jj = j - 1; jj < j + 2; ++jj)
				for (int ii = i - 1; ii < i + 2; ++ii)
					window[k++] = corrupted[jj * width + ii];
			//   Order elements (only half of them)  
			for (int m = 0; m < 5; ++m)
			{
				int min = m;
				for (int n = m + 1; n < 9; ++n)
					if (window[n] < window[min])
						min = n;
				//   Put found minimum element in its place  
				unsigned char temp = window[m];
				window[m] = window[min];
				window[min] = temp;
			}
			smooth[j*width + i] = window[4];
		}
	}
	memcpy(bmpImg->imageData, smooth, width * height);
	bool flag = SaveImage("result_median.bmp", bmpImg);
	if (flag)
	{
		printf("save median ok... \n");
	}
	// ÊÍ·ÅÄÚ´æ
	//LocalUnlock(hNewDIBBits);
	free(smooth);
}
