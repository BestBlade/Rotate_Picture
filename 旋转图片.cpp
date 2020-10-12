#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>


#define pi 3.1415926535

using namespace std;
using namespace cv;

Mat rotatePic(Mat img,Point center,float alpha)
{
	int rows = img.rows - 1;
	int cols = img.cols - 1;
	float cosa = cos(alpha * pi / 180);
	float sina = sin(alpha * pi / 180);

	Point x1y1(-center.x, -center.y);
	Point x1y2(-center.x, cols - center.y);
	Point x2y1(rows - center.x, -center.y);
	Point x2y2(rows - center.x, cols - center.y);

	Point x1y1_, x1y2_, x2y1_, x2y2_;

	x1y1_.x = x1y1.x * cosa - x1y1.y * sina;
	x1y1_.y = x1y1.y * cosa + x1y1.x * sina;

	x1y2_.x = x1y2.x * cosa - x1y2.y * sina;
	x1y2_.y = x1y2.y * cosa + x1y2.x * sina;

	x2y1_.x = x2y1.x * cosa - x2y1.y * sina;
	x2y1_.y = x2y1.y * cosa + x2y1.x * sina;

	x2y2_.x = x2y2.x * cosa - x2y2.y * sina;
	x2y2_.y = x2y2.y * cosa + x2y2.x * sina;

	int new_rows = max(abs(x1y1_.x - x2y2_.x), abs(x1y2_.x - x2y1_.x));
	int new_cols = max(abs(x1y1_.y - x2y2_.y), abs(x1y2_.y - x2y1_.y));
	
	Mat res(new_rows, new_cols, img.type());
	Point new_center;

	new_center.x = new_rows / 2;
	new_center.y = new_cols / 2;

	for (int x = 0; x < new_rows; x++) {
		for (int y = 0; y < new_cols; y++) {
			float x0 = (x - new_center.x) * cosa + (y - new_center.y) * sina + center.x;
			float y0 = (y - new_center.y) * cosa - (x - new_center.x) * sina + center.y;

			if (x0 < 0 || x0 > rows || y0 < 0 || y0 > cols) {
				continue;
			}
			else {
				int x1 = x0;
				int x2 = x1 + 1;
				int y1 = y0;
				int y2 = y1 + 1;

				if (img.channels() == 1) {
					float f1 = (img.at<uchar>(x1, y2) - img.at<uchar>(x1, y1)) * float(y0 - y1) + img.at<uchar>(x1, y1);
					float f2 = (img.at<uchar>(x2, y2) - img.at<uchar>(x2, y1)) * float(y0 - y1) + img.at<uchar>(x2, y1);
					float fxy = (f2 - f1) * float(x0 - x1) + f1;
					
					res.at<uchar>(x, y) = fxy;
				}
				else {
					for (int c = 0; c < img.channels(); c++) {
						float f1 = (img.at<Vec3b>(x1, y2)[c] - img.at<Vec3b>(x1, y1)[c]) * float(y0 - y1) + img.at<Vec3b>(x1, y1)[c];
						float f2 = (img.at<Vec3b>(x2, y2)[c] - img.at<Vec3b>(x2, y1)[c]) * float(y0 - y1) + img.at<Vec3b>(x2, y1)[c];
						float fxy = (f2 - f1) * float(x0 - x1) + f1;

						res.at<Vec3b>(x, y)[c] = fxy;
					}
				}
			}
		}
	}

	return res;
}

int main()
{
	Mat img = imread("C://Users//Chrysanthemum//Desktop//1.jpg");

	//cout << oripic.type() << endl;
	int a = img.rows - 1;				/*	行	*/
	int b = img.cols - 1;				/*	列	*/
	Point center(a / 2, b / 2);			/*	先行后列	*/
	float alpha = 50;

	Mat res = rotatePic(img, center, alpha);

	imshow("origin pic", img);
	imshow("result pic", res);
	waitKey();
}