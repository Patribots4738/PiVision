#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "VisionObject.h"

//This class should prob be a struct but whatever...

float actualWidth, measuredWidth, theta;
cv::Point2f middle;

VisionObject::VisionObject(float width, float pixelWidth, cv::Point center, float angle){
	actualWidth = width;
	measuredWidth = pixelWidth;
	middle = center;
	theta = angle;
}

float VisionObject::getDistance(){
	return 0;
}