#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <math.h>
#include "img.h"  

void Canny(Image* bmpImg)
{
	////////第二步：高斯滤波 
	///////
	double nSigma = 0.2;
	int nWindowSize = 1 + 2 * ceil(3 * nSigma);//通过sigma得到窗口大小
	int nCenter = nWindowSize / 2;
	int nWidth = bmpImg->width;
	int nHeight = bmpImg->height;
	int widthStep = nWidth;// WIDTHBYTES(nWidth * 8);
	unsigned char * pCanny;
	pCanny = (unsigned char*)malloc(sizeof(unsigned char)*nWidth*nHeight);//cvCreateImage(cvGetSize(ColorImage), ColorImage->depth, 1);
	//生成二维滤波核
	double *pKernel_2 = new double[nWindowSize*nWindowSize];
	double d_sum = 0.0;
	for (int i = 0; i<nWindowSize; i++)
	{
		for (int j = 0; j<nWindowSize; j++)
		{
			double n_Disx = i - nCenter;//水平方向距离中心像素距离
			double n_Disy = j - nCenter;
			pKernel_2[j*nWindowSize + i] = exp(-0.5*(n_Disx*n_Disx + n_Disy*n_Disy) / (nSigma*nSigma)) / (2.0*3.1415926)*nSigma*nSigma;
			d_sum = d_sum + pKernel_2[j*nWindowSize + i];
		}
	}
	for (int i = 0; i<nWindowSize; i++)//归一化处理
	{
		for (int j = 0; j<nWindowSize; j++)
		{
			pKernel_2[j*nWindowSize + i] = pKernel_2[j*nWindowSize + i] / d_sum;
		}
	}
	//输出模板
	for (int i = 0; i<nWindowSize*nWindowSize; i++)
	{
		if (i % (nWindowSize) == 0)
		{
			printf("\n");
		}
		printf("%.10f ", pKernel_2[i]);
	}
	//滤波处理
	for (int s = 0; s<nWidth; s++)
	{
		for (int t = 0; t<nHeight; t++)
		{
			double dFilter = 0;
			double dSum = 0;
			//当前坐标（s,t）
			//获取8邻域
			for (int x = -nCenter; x <= nCenter; x++)
			{
				for (int y = -nCenter; y <= nCenter; y++)
				{
					if ((x + s >= 0) && (x + s<nWidth) && (y + t >= 0) && (y + t<nHeight))//判断是否越界
					{
						double currentvalue = (double)bmpImg->imageData[(y + t)*widthStep + x + s];
						dFilter += currentvalue*pKernel_2[x + nCenter + (y + nCenter)*nCenter];
						dSum += pKernel_2[x + nCenter + (y + nCenter)*nCenter];
					}
				}
			}
			pCanny[t*widthStep + s] = (unsigned char)(dFilter / dSum);
		}
	}
//////////////////////////////////////////////////////////
////////第三步：计算梯度值和方向 
//////////////////同样可以用不同的检测器////////////////加上把图像放到0-255之间//////  
/////    P[i,j]=(S[i+1,j]-S[i,j]+S[i+1,j+1]-S[i,j+1])/2     /////  
/////    Q[i,j]=(S[i,j]-S[i,j+1]+S[i+1,j]-S[i+1,j+1])/2     /////  
///////////////////////////////////////////////////////////////// 
   double *P=new double[nWidth*nHeight];
   double *Q=new double[nWidth*nHeight];
   int *M=new int[nWidth*nHeight];
   //IplImage * M;//梯度结果
   //M=cvCreateImage(cvGetSize(ColorImage),ColorImage->depth,1);
   double *Theta=new double[nWidth*nHeight];
   int nwidthstep = nWidth;//pCanny->widthStep;
   for(int iw=0;iw<nWidth-1;iw++)
   {
       for (int jh=0;jh<nHeight-1;jh++)
       {
           P[jh*nWidth+iw]=(double)(pCanny[MIN(iw+1,nWidth-1)+jh*nwidthstep]-pCanny[iw+jh*nwidthstep]+
           pCanny[MIN(iw+1,nWidth-1)+MIN(jh+1,nHeight-1)*nwidthstep]-pCanny[iw+MIN(jh+1,nHeight-1)*nwidthstep])/2;
           Q[jh*nWidth+iw]=(double)(pCanny[iw+jh*nwidthstep]-pCanny[iw+MIN(jh+1,nHeight-1)*nwidthstep]+
           pCanny[MIN(iw+1,nWidth-1)+jh*nwidthstep]-pCanny[MIN(iw+1,nWidth-1)+MIN(jh+1,nHeight-1)*nwidthstep])/2;
        }
    }
//计算幅值和方向
    for(int iw=0;iw<nWidth-1;iw++)
    {
        for (int jh=0;jh<nHeight-1;jh++)
        {
           M[jh*nWidth+iw]=(int)sqrt(P[jh*nWidth+iw]*P[jh*nWidth+iw]+Q[jh*nWidth+iw]*Q[jh*nWidth+iw]+0.5);
           Theta[jh*nWidth+iw]=atan2(Q[jh*nWidth+iw],P[jh*nWidth+iw])*180/3.1415;
           if (Theta[jh*nWidth+iw]<0)
           {
               Theta[jh*nWidth+iw]=360+Theta[jh*nWidth+iw];
           }
        }
    }

	////////第四步：非极大值抑制
	//注意事项 权重的选取，也就是离得近权重大
	///////////////////////////////////////////////////////////////// 
	unsigned char * N;//非极大值抑制结果
	N = (unsigned char*)malloc(sizeof(unsigned char)*nWidth*nHeight);//(cvGetSize(ColorImage), ColorImage->depth, 1);
	unsigned char * OpencvCannyimg;//非极大值抑制结果
	OpencvCannyimg = (unsigned char*)malloc(sizeof(unsigned char)*nWidth*nHeight);// cvCreateImage(cvGetSize(ColorImage), ColorImage->depth, 1);
	int g1 = 0, g2 = 0, g3 = 0, g4 = 0;                            //用于进行插值，得到亚像素点坐标值   
	double dTmp1 = 0.0, dTmp2 = 0.0;                           //保存两个亚像素点插值得到的灰度数据 
	double dWeight = 0.0;                                    //插值的权重  

	for (int i = 1; i<nWidth - 1; i++)
	{
		for (int j = 1; j<nHeight - 1; j++)
		{
			//如果当前点梯度为0，该点就不是边缘点
			if (M[i + j*nWidth] == 0)
			{
				N[i + j*nwidthstep] = 0;
			}
			else
			{
				////////首先判断属于那种情况，然后根据情况插值///////  
				////////////////////第一种情况///////////////////////  
				/////////       g1  g2                  /////////////  
				/////////           C                   /////////////  
				/////////           g3  g4              /////////////  
				/////////////////////////////////////////////////////  
				if ((Theta[i + j*nWidth] >= 90 && Theta[i + j*nWidth]<135) || (Theta[i + j*nWidth] >= 270 && Theta[i + j*nWidth]<315))
				{
					g1 = M[i - 1 + (j - 1)*nWidth];
					g2 = M[i + (j - 1)*nWidth];
					g3 = M[i + (j + 1)*nWidth];
					g4 = M[i + 1 + (j + 1)*nWidth];
					dWeight = fabs(P[i + j*nWidth]) / fabs(Q[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
					////////////////////第二种情况///////////////////////  
					/////////       g1                      /////////////  
					/////////       g2  C   g3              /////////////  
					/////////               g4              /////////////  
					/////////////////////////////////////////////////////  
				}
				else if ((Theta[i + j*nWidth] >= 135 && Theta[i + j*nWidth]<180) || (Theta[i + j*nWidth] >= 315 && Theta[i + j*nWidth]<360))
				{
					g1 = M[i - 1 + (j - 1)*nWidth];
					g2 = M[i - 1 + (j)*nWidth];
					g3 = M[i + 1 + (j)*nWidth];
					g4 = M[i + 1 + (j + 1)*nWidth];
					dWeight = fabs(Q[i + j*nWidth]) / fabs(P[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
					////////////////////第三种情况///////////////////////  
					/////////           g1  g2              /////////////  
					/////////           C                   /////////////  
					/////////       g4  g3                  /////////////  
					/////////////////////////////////////////////////////  
				}
				else if ((Theta[i + j*nWidth] >= 45 && Theta[i + j*nWidth]<90) || (Theta[i + j*nWidth] >= 225 && Theta[i + j*nWidth]<270))
				{
					g1 = M[i + 1 + (j - 1)*nWidth];
					g2 = M[i + (j - 1)*nWidth];
					g3 = M[i + (j + 1)*nWidth];
					g4 = M[i - 1 + (j + 1)*nWidth];
					dWeight = fabs(P[i + j*nWidth]) / fabs(Q[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
					////////////////////第四种情况///////////////////////  
					/////////               g1              /////////////  
					/////////       g4  C   g2              /////////////  
					/////////       g3                      /////////////  
					/////////////////////////////////////////////////////  
				}
				else if ((Theta[i + j*nWidth] >= 0 && Theta[i + j*nWidth]<45) || (Theta[i + j*nWidth] >= 180 && Theta[i + j*nWidth]<225))
				{
					g1 = M[i + 1 + (j - 1)*nWidth];
					g2 = M[i + 1 + (j)*nWidth];
					g3 = M[i - 1 + (j)*nWidth];
					g4 = M[i - 1 + (j + 1)*nWidth];
					dWeight = fabs(Q[i + j*nWidth]) / fabs(P[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;

				}

			}

			if ((M[i + j*nWidth] >= dTmp1) && (M[i + j*nWidth] >= dTmp2))
			{
				N[i + j*nwidthstep] = 200;

			}
			else  N[i + j*nwidthstep] = 0;

		}
	}

	///////第五步：双阈值的选取
	//注意事项  注意是梯度幅值的阈值  
	///////////////////////////////////////////////////////////////// 
	int nHist[1024];//直方图
	int nEdgeNum;//所有边缘点的数目
	int nMaxMag = 0;//最大梯度的幅值
	for (int k = 0; k<1024; k++)
	{
		nHist[k] = 0;
	}
	for (int wx = 0; wx<nWidth; wx++)
	{
		for (int hy = 0; hy<nHeight; hy++)
		{
			if ((unsigned char)N[wx + hy*widthStep] == 200)
			{
				int Mindex = M[wx + hy*nWidth];
				nHist[M[wx + hy*nWidth]]++;//获取了梯度直方图

			}
		}
	}
	nEdgeNum = 0;
	for (int index = 1; index<1024; index++)
	{
		if (nHist[index] != 0)
		{
			nMaxMag = index;
		}
		nEdgeNum += nHist[index];//经过non-maximum suppression后有多少边缘点像素  
	}
	//计算两个阈值 注意是梯度的阈值
	int nThrHigh;
	int nThrLow;
	double dRateHigh = 0.7;
	double dRateLow = 0.5;
	int nHightcount = (int)(dRateHigh*nEdgeNum + 0.5);
	int count = 1;
	nEdgeNum = nHist[1];
	while ((nEdgeNum <= nHightcount) && (count<nMaxMag - 1))
	{
		count++;
		nEdgeNum += nHist[count];
	}
	nThrHigh = count;
	nThrLow = (int)(nThrHigh*dRateLow + 0.5);
	printf("\n直方图的长度 %d  ", nMaxMag);
	printf("\n梯度的阈值幅值大 %d 小阈值 %d  ", nThrHigh, nThrLow);

	//////////////////////////////////////////////////////////
	////////第六步：边缘检测
	/////////////////////////////////////////////////////////////////

	for (int is = 1; is<nWidth - 1; is++)
	{
		for (int jt = 1; jt<nHeight - 1; jt++)
		{
			//CvScalar s=cvGet2D(N,jt,is);
			//int currentvalue=s.val[0];
			int currentvalue = (unsigned char)(N[is + jt*widthStep]);
			if ((currentvalue == 200) && (M[is + jt*nWidth] >= nThrHigh))
				//是非最大抑制后的点且 梯度幅值要大于高阈值
			{
				N[is + jt*nwidthstep] = 255;
				//邻域点判断
				TraceEdge(is, jt, nwidthstep,nThrLow, N, M);
			}
		}
	}
	for (int si = 1; si<nWidth - 1; si++)
	{
		for (int tj = 1; tj<nHeight - 1; tj++)
		{
			if ((unsigned char)N[si + tj*nwidthstep] != 255)
			{
				N[si + tj*nwidthstep] = 0;
			}
		}

	}

	memcpy(bmpImg->imageData, N, nWidth * nHeight);
	bool flag = SaveImage("result_canny.bmp", bmpImg);
	if (flag)
	{
		printf("\nsave canny ok...");
	}
	// 释放内存
	//LocalUnlock(hNewDIBBits);
	free(pCanny);
	free(N);
}

int TraceEdge(int w, int h, int widthStep,int nThrLow, unsigned char* pResult, int *pMag)
{
	int n, m;
	char flag = 0;
	int currentvalue = (unsigned char)pResult[w + h*widthStep];
	if (currentvalue == 0)
	{
		pResult[w + h*widthStep] = 255;
		flag = 0;
		for (n = -1; n <= 1; n++)
		{
			for (m = -1; m <= 1; m++)
			{
				if (n == 0 && m == 0) continue;
				int curgrayvalue = (unsigned char)pResult[w + n + (h + m)*widthStep];
				int curgrdvalue = pMag[w + n + (h + m)*(w + h*widthStep)];//pMag[w + n + (h + m)*pResult->width];
				if (curgrayvalue == 200 && curgrdvalue>nThrLow)
					if (TraceEdge(w + n, h + m, widthStep,nThrLow, pResult, pMag))
					{
						flag = 1;
						break;
					}
			}
			if (flag) break;
		}
		return(1);
	}
	return(0);
}