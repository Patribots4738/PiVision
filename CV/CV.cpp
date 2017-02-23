// CV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;

int drawSquare(cv::Mat src, cv::Point2f points[], cv::Scalar color) {
	cv::line(src, points[0], points[1], color);
	cv::line(src, points[1], points[2], color);
	cv::line(src, points[2], points[3], color);
	cv::line(src, points[3], points[0], color);
	return 0;
}

int main()
{
	cv::VideoCapture cap(0);
	int lowerH = 17, lowerS = 157, lowerV = 132, upperH = 35, upperS = 255, upperV = 255;
	float focalLength = 505, targetWidth = 3; 
    return 0;
}