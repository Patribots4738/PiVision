#include "stdafx.h"
#include "VisionCore.h"
#include <opencv2\opencv.hpp>

using namespace std;

cv::Scalar upperBound, lowerBound;
float focalLength, actualWidth;

//*******************************************
struct VisionObject {
	float actualWidth;
	float pixelWidth;
	float centerX;
	float centerY;
	float angle;
	float distance;
};
//*******************************************

static void drawSquare(cv::Mat src, cv::Point2f points[], cv::Scalar color) {
	cv::line(src, points[0], points[1], color);
	cv::line(src, points[1], points[2], color);
	cv::line(src, points[2], points[3], color);
	cv::line(src, points[3], points[0], color);
}

void VisionCore::setFocalLength(float length) {
	focalLength = length;
}

void VisionCore::setBounds(cv::Scalar upper, cv::Scalar lower) {
	upperBound = upper;
	lowerBound = lower;
}

VisionCore::VisionCore(double focalLength) {
	setFocalLength(focalLength);
}

cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_ERODE,
	cv::Size(2 * 1 + 1, 2 * 1 + 1),
	cv::Point(1, 1));

cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_ERODE,
	cv::Size(2 * 2 + 1, 2 * 2 + 1),
	cv::Point(2, 2));

VisionObject* DetectObjects(cv::Mat frame){
	cv::Mat src, dst;
	src = frame.clone();
	dst = frame.clone();

	cv::flip(src, src, 1);

	dst = src.clone();

	cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);

	cv::GaussianBlur(dst, dst, cv::Size(5, 5), 2, 2);

	cv::inRange(dst, upperBound, upperBound, dst);

	cv::erode(dst, dst, erodeElement);
	cv::dilate(dst, dst, dilateElement);

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::findContours(dst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	vector<vector<cv::Point>> approxCurve(contours.size()), hull(contours.size());
	vector<VisionObject> vObjects;

	for (int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours.at(i), hull.at(i));
		cv::approxPolyDP(contours.at(i), approxCurve.at(i), 0.1 * cv::arcLength(hull.at(i), false), true);

		//Create our temp VisionObject to assign variables
		VisionObject vObject; //VO => VisionObject

		cv::Moments m = cv::moments(contours.at(i), true);
		cv::Point center(m.m10 / m.m00, m.m01 / m.m00); //Finds the center

		cv::Rect boundingBox = cv::boundingRect(approxCurve.at(i));

		cv::RotatedRect rect = cv::minAreaRect(approxCurve.at(i));

		//Calculate the distance based off of the object width
		double distance = (actualWidth * focalLength) / boundingBox.width;

		//Set the scruct value
		vObject.actualWidth = actualWidth;
		vObject.centerX = center.x;
		vObject.centerY = center.y;
		vObject.distance = distance;
		vObject.pixelWidth = boundingBox.width;
		vObject.angle = rect.angle;

		vObjects.push_back(vObject);
	}

	if(vObjects.empty()){
		return NULL;
	}

	return &vObjects[0];
}