//image_rw.c文件
#include<stdio.h>
#include<stdlib.h>
#include"image.h"

void image_info(FILE *file)
{
	int times = 3; //输入文件名次数。
	char bmp_name[10]; //文件名
	printf("\nplease enter a file name for reading:");
	do
	{
		if (times<3)
		{
			printf("\nplease enter a file name for reading again:");
		}
		fflush(stdin);
		gets(bmp_name);
		//printf("\n%s",bmp_name);
		file = fopen(bmp_name, "rb+");  //打开一个文件进行读写操作。
		--times;
		if (file == NULL)
		{
			printf("\nerror opening %s for reading! ", bmp_name);
		}
		else
		{
			break;
		}
	} while (times != 0);
	if (times == 0)
	{
		printf("\nsorry, shutdown!");
		exit(1);
	}
	//读取图像信息
	fseek(file, 0L, 0); //读取图像文件类型
	fread(&bmp, sizeof(BMP), 1, file);
	printf("\n bmp tpye: %u", bmp.bfType);
	printf("\n bmp size: %u", bmp.bfSize);
	printf("\n bmp reserved1: %u", bmp.bfReserved1);
	printf("\n bmp reserved2: %u", bmp.bfReserved2);
	printf("\n bmp offBits: %u", bmp.bfOffBits);
	printf("\n bmp bisize: %u", bmp.biSize);
	printf("\n bmp biWidth: %d", bmp.biWidth);
	printf("\n bmp biHeight: %d", bmp.biHeight);
	printf("\n bmp biplans: %u", bmp.biPlanes);
	printf("\n bmp biBitCount: %u", bmp.biBitCount);
	//printf("\n bmp biCompression: %u", bmp.biCompression);
	printf("\n bmp biSizeImage: %u", bmp.biSizeImage);
	printf("\n bmp biXPelsPerMeter: %d", bmp.biXPelsPerMeter);
	printf("\n bmp biYPelsPerMeter: %d", bmp.biYPelsPerMeter);
	printf("\n bmp biClrUsed: %u", bmp.biClrUsed);
	printf("\n bmp biClrImportant: %u\n", bmp.biClrImportant);
	line_byte = (bmp.biWidth*bmp.biBitCount / 8 + 3) / 4 * 4; //获得图像数据每行的数据个数
	//printf("dfsa%u",bmp.line_byte);
	//bmp.imagedata=NULL;
	imagedata = (unsigned char*)malloc(bmp.biSizeImage);
	fseek(file, (long)bmp.bfOffBits, 0);
	fread(imagedata, sizeof(unsigned char), bmp.biSizeImage, file);
	fclose(file);
}
//保存图像
void image_save(FILE *file)
{
	int times = 3; //输入文件名次数。
	char bmp_name[10]; //文件名
	//int i; //记录数据区个数
	printf("\nplease enter a file name for writing:");
	do
	{
		if (times<3)
		{
			printf("\nplease enter a file name for writing again:");
		}
		fflush(stdin);
		gets(bmp_name);
		printf("\n%s", bmp_name);
		file = fopen(bmp_name, "wb+"); //打开一个文件进行读写操作。
		--times;
		if (file == NULL)
		{
			printf("\nerror opening %s for writing", bmp_name);
		}
		else
		{
			break;
		}
	} while (times != 0);
	if (times == 0)
	{
		printf("\nsorry, shutdown!");
		exit(1);
	}
	//写文件头
	printf("\n%s", bmp_name);
	fseek(file, 0L, 0); //图像文件类型
	fwrite(&(bmp.bfType), sizeof(short), 1, file);
	printf("\n bmp tpye: %d", bmp.bfType);
	fseek(file, 2L, 0); //图像文件大小
	fwrite(&(bmp.bfSize), sizeof(int), 1, file);
	printf("\n bmp size: %d", bmp.bfSize);
	fseek(file, 6L, 0); //图像文件保留字1
	fwrite(&(bmp.bfReserved1), sizeof(short), 1, file);
	printf("\n bmp reserved1: %d", bmp.bfReserved1);
	fseek(file, 8L, 0); //图像文件保留字2
	fwrite(&(bmp.bfReserved2), sizeof(short), 1, file);
	printf("\n bmp reserved2: %d", bmp.bfReserved2);
	fseek(file, 10L, 0);//数据区的偏移量
	fwrite(&(bmp.bfOffBits), sizeof(short), 1, file);
	printf("\n bmp offBits: %d", bmp.bfOffBits);
	fseek(file, 14L, 0);//文件头结构大小
	fwrite(&(bmp.biSize), sizeof(int), 1, file);
	printf("\n bmp bisize: %d", bmp.biSize);
	fseek(file, 18L, 0);//图像的宽度
	fwrite(&(bmp.biWidth), sizeof(int), 1, file);
	printf("\n bmp biWidth: %d", bmp.biWidth);
	fseek(file, 22L, 0);//图像的高度
	fwrite(&(bmp.biHeight), sizeof(int), 1, file);
	printf("\n bmp biHeight: %d", bmp.biHeight);
	fseek(file, 24L, 0);//图像的面数
	fwrite(&(bmp.biPlanes), sizeof(short), 1, file);
	printf("\n bmp biplans: %d", bmp.biPlanes);
	fseek(file, 28L, 0);//图像一个像素的字节数
	fwrite(&(bmp.biBitCount), sizeof(short), 1, file);
	printf("\n bmp biBitCount: %d", bmp.biBitCount);
	fseek(file, 30L, 0);//图像压缩信息
	fwrite(&(bmp.biCompression), sizeof(short), 1, file);
	printf("\n bmp biCompression: %d", bmp.biCompression);
	fseek(file, 34L, 0);//图像数据区的大小
	fwrite(&(bmp.biSizeImage), sizeof(int), 1, file);
	printf("\n bmp biSizeImage: %d", bmp.biSizeImage);
	fseek(file, 38L, 0);//水平分辨率
	fwrite(&(bmp.biXPelsPerMeter), sizeof(int), 1, file);
	printf("\n bmp biXPelsPerMeter: %d", bmp.biXPelsPerMeter);
	fseek(file, 42L, 0);//垂直分辨率
	fwrite(&(bmp.biYPelsPerMeter), sizeof(int), 1, file);
	printf("\n bmp biYPelsPerMeter: %d", bmp.biYPelsPerMeter);
	fseek(file, 46L, 0);//颜色索引数
	fwrite(&(bmp.biClrUsed), sizeof(int), 1, file);
	printf("\n bmp biClrUsed: %d", bmp.biClrUsed);
	fseek(file, 50L, 0);//重要颜色索引数
	fwrite(&(bmp.biClrImportant), sizeof(int), 1, file);
	printf("\n bmp biClrImportant: %d\n", bmp.biClrImportant);
	fseek(file, (long)(bmp.bfOffBits), 0);
	fwrite(imagedata, sizeof(unsigned char), bmp.biSizeImage, file);
	fclose(file);

}








