#pragma once
#include "stdafx.h"


class VisionCore {
public:
	float focalLength, actualHeight, FOV;

	struct VisionObject;

	void setBounds(cv::Scalar upper, cv::Scalar lower);
	void setFocalLength(float length);
	void setFOV(double fov);
	VisionCore(double focalLength);

	VisionObject* DetectObjects(cv::Mat frame); //retun a pointer from the array
	VisionObject* DetectObjects(cv::UMat frame); //retun a pointer from the array
private:
	cv::Scalar upperBound, lowerBound;
};