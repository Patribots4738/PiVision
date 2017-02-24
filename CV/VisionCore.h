#pragma once
#include "stdafx.h"
#include <opencv2\opencv.hpp>

class VisionCore {
public:
	struct VisionObject;

	VisionCore(double focalLength);
	VisionObject* DetectObjects(cv::Mat frame); //retun a pointer from the array
	VisionObject* DetectObjectsOCL(cv::Mat frame); //retun a pointer from the array
	void setBounds(cv::Scalar upper, cv::Scalar lower);
	void setFocalLength(float length);
	void setFPS(int fps);
private:
	cv::Scalar upper, lower;
};