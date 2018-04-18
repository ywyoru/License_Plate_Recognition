#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

CRect* find_connected_region_location(Image* img, int xthreashold, int ythreashold, float rateLow, float rateHigh)
{
	unsigned char *src = img->imageData;
	int i, j;
	int x1, y1, x2, y2;		//x1=left, x2=right, y1=top, y2=bottom
	int width;
	int height;
	unsigned char *temp;
	//int queue_count;  
	int head, rear;
	XY_Queue *queue;
	static int direction[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

	width = img->width;
	height = img->height;

	queue = (XY_Queue *)malloc(sizeof(XY_Queue) * width * height);
	temp = (unsigned char *)malloc(width * height * sizeof(unsigned char));
	//bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width * 3 * height);
	img->pre_region = (PreRegion*)malloc(sizeof(PreRegion)*CAN_REGION_NUM);

	if (temp == NULL)
	{
		printf("find_connected_region_location mem alloc fail\n");
	}
	memcpy(temp, src, width * height);

	head = rear = 0;
	img->region_num = 0;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			if (temp[i * width + j] == 255)
			{

				queue[rear].x = j;	//横坐标x为j
				queue[rear].y = i;	//纵坐标y为i
				rear++;
				temp[i * width + j] = 0;

				img->pre_region[img->region_num].x1 = j;
				img->pre_region[img->region_num].x2 = j;
				img->pre_region[img->region_num].y1 = i;
				img->pre_region[img->region_num].y2 = i;

				if (img->region_num > CAN_REGION_NUM)
				{
					printf("over the CAN_REGION_NUM\n");
					
				}

				while (head < rear)
				{
					x1 = queue[head].x;
					y1 = queue[head].y;
					head++;

					if (x1 < img->pre_region[img->region_num].x1)
						img->pre_region[img->region_num].x1 = x1;
					else if (x1 > img->pre_region[img->region_num].x2)
						img->pre_region[img->region_num].x2 = x1;
					if (y1 < img->pre_region[img->region_num].y1)
						img->pre_region[img->region_num].y1 = y1;
					else if (y1 > img->pre_region[img->region_num].y2)
						img->pre_region[img->region_num].y2 = y1;


					for (i = 0; i < 4; i++)
					{
						x2 = x1 + direction[i][0];
						y2 = y1 + direction[i][1];

						if (x2 > 0 && x2 < width && y2 > 0 && y2 < height && temp[y2 * width + x2])
						{
							temp[y2 * width + x2] = 0;
							queue[rear].x = x2;
							queue[rear].y = y2;
							rear++;
						}

					}
				}
				if ((img->pre_region[img->region_num].x2 - img->pre_region[img->region_num].x1 > xthreashold) && (img->pre_region[img->region_num].y2 - img->pre_region[img->region_num].y1 > ythreashold))
				{

					img->pre_region[img->region_num].width = img->pre_region[img->region_num].x2 - img->pre_region[img->region_num].x1 + 1;
					img->pre_region[img->region_num].height = img->pre_region[img->region_num].y2 - img->pre_region[img->region_num].y1 + 1;
					img->pre_region[img->region_num].rate = (float)img->pre_region[img->region_num].width / img->pre_region[img->region_num].height;
					if ((img->pre_region[img->region_num].width < img->width / 2) && (img->pre_region[img->region_num].height < img->height / 2))
						if ((img->pre_region[img->region_num].rate > rateLow) && (img->pre_region[img->region_num].rate < rateHigh))
						{
							if (img->pre_region[img->region_num].x2 + PRE_LOCATION_BIAS > img->width)
								img->pre_region[img->region_num].x2 = img->width;
							else
								img->pre_region[img->region_num].x2 += PRE_LOCATION_BIAS;
							if (img->pre_region[img->region_num].x1 - PRE_LOCATION_BIAS < 0)
								img->pre_region[img->region_num].x1 = 0;
							else
								img->pre_region[img->region_num].x1 -= PRE_LOCATION_BIAS;
							if (img->pre_region[img->region_num].y2 + PRE_LOCATION_BIAS > img->height)
								img->pre_region[img->region_num].y2 = img->height;
							else
								img->pre_region[img->region_num].y2 += PRE_LOCATION_BIAS;
							if (img->pre_region[img->region_num].y1 - PRE_LOCATION_BIAS < 0)
								img->pre_region[img->region_num].y1 = 0;
							else
								img->pre_region[img->region_num].y1 -= PRE_LOCATION_BIAS;

							img->pre_region[img->region_num].width = img->pre_region[img->region_num].x2 - img->pre_region[img->region_num].x1 + 1;
							img->pre_region[img->region_num].height = img->pre_region[img->region_num].y2 - img->pre_region[img->region_num].y1 + 1;

							img->region_num++;
						}
				}
			}

		}
	CRect* PlateRect;
	PlateRect = (CRect*)malloc(sizeof(CRect)*img->region_num);
	printf("\nConnected Region:");
	printf("\nregion_num = %d", img->region_num);
	for ( i = 0; i < img->region_num; i++)
	{
		//printf("\nwidth = %d, height = %d", img->pre_region[i].width, img->pre_region[i].height);
		PlateRect[i].top = img->pre_region[i].y1;
		PlateRect[i].bottom = img->pre_region[i].y2;
		PlateRect[i].left = img->pre_region[i].x1;
		PlateRect[i].right = img->pre_region[i].x2;

	}
	//创建CRect类，把每一个连通域的x1x2y1y2存入这个类
	printf("\n");
	free(temp);
	temp = NULL;
	return PlateRect;
}