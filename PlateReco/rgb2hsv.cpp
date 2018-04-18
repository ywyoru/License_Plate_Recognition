#include <stdio.h>
#include <stdlib.h>  
#include "img.h"  

Image* Rgb2Hsv(const Image* imgRGB)
{
	float fR, fG, fB;
	float fH, fS, fV;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	// Create a blank HSV image
	//if (!imageHSV || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
	//printf("ERROR in convertImageRGBtoHSV()! Bad input image.\n");
	//exit(1);
	//}

	int h = imgRGB->height;  // Pixel height.
	int w = imgRGB->width;  // Pixel width.
	//int rowSizeRGB = imageRGB->widthStep; // Size of row in bytes, including extra padding.
	//char *imRGB = imageRGB->imageData; // Pointer to the start of the image pixels.
	//int rowSizeHSV = imageHSV->widthStep; // Size of row in bytes, including extra padding.
	//char *imHSV = imageHSV->imageData; // Pointer to the start of the image pixels.

	Image* imgHSV;
	imgHSV = (Image*)malloc(sizeof(Image));
	imgHSV->height = h;
	imgHSV->width = w;
	imgHSV->channels = 3;
	imgHSV->imageData = (unsigned char*)malloc(sizeof(unsigned char)*3*w * h);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			//uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			int bB = imgRGB->imageData[y*w * 3 + x * 3]; //*(uchar*)(pRGB+0); // Blue component
			int bG = imgRGB->imageData[y*w * 3 + x * 3+1]; //*(uchar*)(pRGB+1); // Green component
			int bR = imgRGB->imageData[y*w * 3 + x * 3+2]; //*(uchar*)(pRGB+2); // Red component

			// Convert from 8-bit integers to floats.
			fR = bR * BYTE_TO_FLOAT;
			fG = bG * BYTE_TO_FLOAT;
			fB = bB * BYTE_TO_FLOAT;

			// Convert from RGB to HSV, using float ranges 0.0 to 1.0.
			float fDelta;
			float fMin, fMax;
			int iMax;
			// Get the min and max, but use integer comparisons for slight speedup.
			if (bB < bG) {
				if (bB < bR) {
					fMin = fB;
					if (bR > bG) {
						iMax = bR;
						fMax = fR;
					}
					else {
						iMax = bG;
						fMax = fG;
					}
				}
				else {
					fMin = fR;
					fMax = fG;
					iMax = bG;
				}
			}
			else {
				if (bG < bR) {
					fMin = fG;
					if (bB > bR) {
						fMax = fB;
						iMax = bB;
					}
					else {
						fMax = fR;
						iMax = bR;
					}
				}
				else {
					fMin = fR;
					fMax = fB;
					iMax = bB;
				}
			}
			fDelta = fMax - fMin;
			fV = fMax;    // Value (Brightness).
			if (iMax != 0) {   // Make sure its not pure black.
				fS = fDelta / fMax;  // Saturation.
				float ANGLE_TO_UNIT = 1.0f / (6.0f * fDelta); // Make the Hues between 0.0 to 1.0 instead of 6.0
				if (iMax == bR) {  // between yellow and magenta.
					fH = (fG - fB) * ANGLE_TO_UNIT;
				}
				else if (iMax == bG) {  // between cyan and yellow.
					fH = (2.0f / 6.0f) + (fB - fR) * ANGLE_TO_UNIT;
				}
				else {    // between magenta and cyan.
					fH = (4.0f / 6.0f) + (fR - fG) * ANGLE_TO_UNIT;
				}
				// Wrap outlier Hues around the circle.
				if (fH < 0.0f)
					fH += 1.0f;
				if (fH >= 1.0f)
					fH -= 1.0f;
			}
			else {
				// color is pure Black.
				fS = 0;
				fH = 0; // undefined hue
			}

			// Convert from floats to 8-bit integers.
			int bH = (int)(0.5f + fH * 255.0f);
			int bS = (int)(0.5f + fS * 255.0f);
			int bV = (int)(0.5f + fV * 255.0f);

			// Clip the values to make sure it fits within the 8bits.
			if (bH > 255)
				bH = 255;
			if (bH < 0)
				bH = 0;
			if (bS > 255)
				bS = 255;
			if (bS < 0)
				bS = 0;
			if (bV > 255)
				bV = 255;
			if (bV < 0)
				bV = 0;

			// Set the HSV pixel components.
			imgHSV->imageData[y*w * 3 + x * 3 + 0] = bH;  // H component
			imgHSV->imageData[y*w * 3 + x * 3 + 1] = bS;  // S component
			imgHSV->imageData[y*w * 3 + x * 3 + 2] = bV;  // V component
		}
	}
	bool flag = SaveImage("result_hsv.bmp", imgHSV);
	if (flag)
	{
		printf("save hsv ok... \n");
	}
	return imgHSV;
}

/*
void Rgb2Hsv(unsigned char R, unsigned char G, unsigned char B, float& H, float& S, float&V)
{
	// r,g,b values are from 0 to 1  
	// h = [0,360], s = [0,1], v = [0,1]  
	// if s == 0, then h = -1 (undefined)  
	float min, max, delta, tmp;
	tmp = R>G ? G : R;
	min = tmp>B ? B : tmp;
	tmp = R>G ? R : G;
	max = tmp>B ? tmp : B;
	V = max; // v  
	delta = max - min;
	if (max != 0)
		S = delta / max; // s  
	else
	{
		// r = g = b = 0 // s = 0, v is undefined  
		S = 0;
		H = 0;
		return;
	}
	if (delta == 0){
		H = 0;
		return;
	}
	else if (R == max){
		if (G >= B)
			H = (G - B) / delta; // between yellow & magenta  
		else
			H = (G - B) / delta + 6.0;
	}
	else if (G == max)
		H = 2.0 + (B - R) / delta; // between cyan & yellow  
	else if (B == max)
		H = 4.0 + (R - G) / delta; // between magenta & cyan  
	H *= 60.0; // degrees  
}
*/