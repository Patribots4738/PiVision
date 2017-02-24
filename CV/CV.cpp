#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;

int main()
{
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FPS, 60);

	int lowerH = 17, lowerS = 157, lowerV = 132, upperH = 35, upperS = 255, upperV = 255;
	float focalLength = 505, targetWidth = 3;

	cv::Mat frame;
	VisionCore vcore(focalLength);
	cap >> frame;

	cv::imshow("dst", frame);

	while(true){
		cv::createTrackbar("upperH", "dst", &upperH, 255);
		cv::createTrackbar("upperS", "dst", &upperS, 255);
		cv::createTrackbar("upperV", "dst", &upperV, 255);

		cv::createTrackbar("lowerH", "dst", &lowerH, 255);
		cv::createTrackbar("lowerS", "dst", &lowerS, 255);
		cv::createTrackbar("lowerV", "dst", &lowerV, 255);

		vcore.setBounds(cv::Scalar(lowerH, lowerS, lowerV), cv::Scalar(upperH, upperS, upperV));
		cap >> frame;
<<<<<<< HEAD

		
		vcore.VisionCore::DetectObjects(frame);

		cv::waitKey(1);
=======
		vcore.DetectObjects(frame);
>>>>>>> parent of beb047b... Removed unused files, also fixed error that was preventing compiling
	}

    return 0;
}