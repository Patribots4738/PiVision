#pragma once
#include <opencv2\opencv.hpp>

class VisionCore {
public:
	struct VisionObject;

	VisionCore(double focalLength);
	VisionObject* DetectObjects(cv::Mat frame); //retun a pointer from the array
	void setBounds(cv::Scalar upper, cv::Scalar lower);
	void setFocalLength(float length);
private:
	cv::Scalar upper, lower;
};