#pragma once
#include "stdafx.h"


class VisionCore {
public:
	float focalLength, actualHeight;

	struct VisionObject;

	void setBounds(cv::Scalar upper, cv::Scalar lower);
	void setFocalLength(float length);
	VisionCore(double focalLength);

	VisionObject* DetectObjects(cv::Mat frame); //retun a pointer from the array
	VisionObject* DetectObjectsOCL(cv::Mat frame); //retun a pointer from the array
private:
	cv::Scalar upperBound, lowerBound;
};