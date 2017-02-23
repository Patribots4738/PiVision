#pragma once
#include "opencv2\opencv.hpp"

class VisionObject {
public:
	VisionObject(float width, float pixelWidth, cv::Point center, float angle);
	float actualWidth, measuredWidth, theta;
	cv::Point2f middle;
	float getDistance();
private:
};