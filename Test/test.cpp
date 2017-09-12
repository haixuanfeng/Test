#include <iostream>  
#include<Windows.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include<vector>

using namespace cv;
using namespace std;

IplImage *src;
IplImage *pImg8u = NULL;             //灰度图  
IplImage *pImg8uSmooth = NULL;       //高斯滤波后的图  
IplImage *pImgCanny = NULL;          //二值化的图
vector<IplImage> numbers;

void findNumber(IplImage *);

int main() {
	
	src = cvLoadImage("hh.png", -1);
	CvSize size = cvSize((*src).width, (*src).height);
	pImg8uSmooth = cvCreateImage(size, IPL_DEPTH_8U, 1);
	pImg8u = cvCreateImage(size, IPL_DEPTH_8U, 1);
	pImgCanny = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvCvtColor(src, pImg8u, CV_RGB2GRAY);                //灰度化  
	cvSmooth(pImg8u, pImg8uSmooth, CV_GAUSSIAN, 3, 0, 0);   //高斯滤波  
	cvCanny(pImg8uSmooth, pImgCanny, 100, 200, 3);         //二值化     
	//cvCanny(pImg8uSmooth, pImgCanny, 100, 200, 3);
	cvDilate(pImgCanny, pImgCanny, 0, 1);
	cvErode(pImgCanny, pImgCanny, 0, 1);
	//cvNamedWindow("cvcanny", 1);
	//cvShowImage("cvcanny", pImg8uSmooth);
	
	findNumber(pImg8uSmooth);
	//cvShowImage("cvcanny", pImg8uSmooth);
	//cvWaitKey(0);
	
}

void findNumber(IplImage * img) {
	int width = img->width;
	int height = img->height;
	int step = img->widthStep;
	int channels = img->nChannels;
	uchar *data = (uchar*)img->imageData;
	cout << width;
		for (int i = 0;i != height; ++i)
		{
			for (int j = 0;j != width; ++j)
			{
				
					
					if (data[i*step + j*channels]<220)
						data[i*step + j*channels] = 0;//255-data[i*step+j*channels+k];  
					else
						data[i*step + j*channels] = 255;//255-data[i*step+j*channels+k];  
					
				
			}
		}

		for (int i = 0;i < height;i++)
		{
			int couter = 0;
			for (int j = 0;j < width;j++)
			{
				if (data[i*step + j*channels] == 255)
					couter++;
				else
				{
					if (couter>160&&couter<200)
					{
						bool yes = true;
						for (int k = 1; k < 10; k++)
						{
							for (int z = couter;z > 0;z--)
							{
								if (data[(i + k)*step + j - couter] != 255)
									yes = false;
							}


						}

						if (yes)
						{
							IplImage * img1;
							img1 = cvCreateImage(CvSize(couter, 100), IPL_DEPTH_8U, 1);
							CvRect ROI_rect1;
							ROI_rect1.x = j - couter;
							ROI_rect1.y = i;
							ROI_rect1.width = couter;
							ROI_rect1.height = 100;
							cvSetImageROI(img, ROI_rect1);
							cvCopy(img, img1, NULL); //获取第1个字符  
							cvResetImageROI(img);
							cvNamedWindow("cvcanny", 1);
							cvShowImage("cvcanny", img1);
							waitKey(0);
							
						}

					}
					else
					{
						couter = 0;
					}
				}

			}
		}
}