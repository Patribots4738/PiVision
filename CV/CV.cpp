#include "stdafx.h"
#include <iostream>
#include "opencv2\opencv.hpp"
#include "VisionCore.h"

using namespace std;

int main()
{
	cv::VideoCapture cap(0);
	int lowerH = 17, lowerS = 157, lowerV = 132, upperH = 35, upperS = 255, upperV = 255;
	float focalLength = 505, targetWidth = 3;

	cv::Mat frame;
	VisionCore vcore(focalLength);

	while(true){
		vcore.setBounds(cv::Scalar(upperH, upperS, upperV), cv::Scalar(lowerH, lowerS, lowerV));
		cap >> frame;
		VisionCore::VisionObject *obj = vcore.DetectObjects(frame);
	}

    return 0;
}