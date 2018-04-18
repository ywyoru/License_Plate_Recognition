//img.h
#ifndef IMG_H  
#define IMG_H  

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)                                                                                            
#define MIN(X,Y) ((X)>(Y)?(Y):(X))
#define MAX(X,Y) ((X)<(Y)?(Y):(X))
#define CAN_REGION_NUM 8
#define PRE_LOCATION_BIAS 5

typedef struct
{
	//BitMapFileHeader
	//unsigned short    bfType;  
	unsigned long    bfSize;
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;
	//BitMapInfoHeader
	unsigned long  biSize;
	long   biWidth;
	long   biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
}BMP;

typedef struct
{
	//RgbQuad 
	unsigned char rgbBlue; //该颜色的蓝色分量  
	unsigned char rgbGreen; //该颜色的绿色分量  
	unsigned char rgbRed; //该颜色的红色分量  
	unsigned char rgbReserved; //保留值 
}BMPQuad;

typedef struct
{
	int x1, x2;
	int y1, y2;
	int width;
	int height;
	float rate;		//宽高比
}PreRegion;			//连通域

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;
	int region_num;				//车牌数
	PreRegion* pre_region;		//连通域指针
}Image;

typedef struct
{
	int x;
	int y;
}XY_Queue;

typedef struct
{
	int top;
	int bottom;
	int left;
	int right;
}CRect;


Image* LoadImage(char* path);
bool SaveImage(char* path, Image* bmpImg);

Image* Rgb2Gray(Image* bmpImg);
Image* Rgb2Hsv(const Image* imgRGB);
Image* PlateFilter(Image* imgHSV);

/***************************************滤波器****************************************/
//void _medianfilter(Image* bmpImg);
void InteEqualize(Image* bmpImg);							//直方图均衡化
void grayStretch(Image* src, int nMin, int nMax);			//灰度拉伸
void Gaussian(Image* bmpImg);								//高斯模糊
void medianFilter(Image* bmpImg);							//中值滤波

/***************************************边缘检测**************************************/
void horizonGrad(Image* bmpImg, int threshold);				//水平梯度
void JumpPoint(Image* bmpImg);								//水平跳变点
void Robert(Image* bmpImg);									//Robert边缘检测算子
void Sobel(Image* bmpImg);									//Sobel边缘检测算子
void SobelVertical(Image* bmpImg);							//垂直Sobel边缘检测算子
void edgesob8(unsigned char *image, int width, int height); //Sobel(8邻域)
void Canny(Image* bmpImg);									//Canny边缘检测算子

/***************************************膨胀腐蚀**************************************/
void Erosion(Image* srcImage, int nMode);     				//腐蚀
void Dilation(Image* srcImage, int nMode);					//膨胀
void EdgeErasion(Image* srcimage, int threshold);			//去除膨胀后的四边

/***************************************二值化****************************************/
int otsu(Image *image);										//otsu前景背景阈值算法
void threshold(Image* bmpImg);								//阈值为W(x,y)=0.5*(max f(x+m,y+n)+min f(x+m,y+n))
void thresholdSet(Image* bmpImg, int threshold);				//手动设置二值化阈值
void quickAdaptiveThreshold(Image* bmpImg);					//A fast adaptive image binarization method (Wellner 1993)
//void ThresholdTrans(Image* bmpImg);
/************************************寻找车牌边缘**************************************/
int TraceEdge(int w, int h, int widthStep, int nThrLow, unsigned char* pResult, int *pMag);
//void location(Image* ImgPlate, int yuzhi, int *HL, int *HH, int *VL, int *VH);
CRect* location(Image* ImgPlate, int yuzhi);					//定位车牌矩形（一个），内部用到sob8检测边缘
void TopBottom(Image* bmpImg, int* iTop, int* iBottom);		//寻找上下边界
void LeftRight(Image* bmpImg, int* iLeft, int* iRight);		//寻找左右边界
void Hough(Image* srcImg, int yuzhi);						//Hough变换
CRect* find_connected_region_location(Image* img, int xthreashold, int ythreashold, float rateLow, float rateHigh);		//寻找连通域函数

/***********************************挖出车牌并保存**************************************/
void PickRec(Image* Img, Image* ImgPlate, CRect* rect);						//最终定位车牌并保存

#endif  
