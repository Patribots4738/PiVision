#include "stdafx.h"
#include "VisionCore.h"

using namespace std;

cv::Scalar upperBound, lowerBound;
float focalLength = 458, actualWidth = 3, FOV = 76.6;

//*******************************************

struct VisionCore::VisionObject {
	float actualWidth;
	float pixelWidth;
	cv::Point center;
	float angle;
	float distance;
};

static string serializeVisionObject(VisionCore::VisionObject object){
	string data = "";
	data += to_string(object.actualWidth) + ",";
	data += to_string(object.pixelWidth) + ",";
	data += to_string(object.angle) + ",";
	data += to_string(object.distance) + ",";
	data += to_string(object.center.x) + ",";
	data += to_string(object.center.y) + ",";
	return data;
}
//*******************************************

static void drawSquare(cv::Mat src, cv::Point2f points[], cv::Scalar color) {
	cv::line(src, points[0], points[1], color);
	cv::line(src, points[1], points[2], color);
	cv::line(src, points[2], points[3], color);
	cv::line(src, points[3], points[0], color);
}

static void drawSquare(cv::UMat src, cv::Point2f points[], cv::Scalar color) {
	cv::line(src, points[0], points[1], color);
	cv::line(src, points[1], points[2], color);
	cv::line(src, points[2], points[3], color);
	cv::line(src, points[3], points[0], color);
}

void VisionCore::setFocalLength(float length) {
	focalLength = length;
}

void VisionCore::setFOV(double fov) {
	FOV = fov;
}

void VisionCore::setBounds(cv::Scalar lower, cv::Scalar upper) {
	upperBound = upper;
	lowerBound = lower;
}

VisionCore::VisionCore(double focalLength) {
	setFocalLength(focalLength);
}

cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_ERODE,
	cv::Size(2 * 1 + 1, 2 * 1 + 1),
	cv::Point(2, 2));

cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_ERODE,
	cv::Size(2 * 2 + 1, 2 * 2 + 1),
	cv::Point(4, 4));

VisionCore::VisionObject* VisionCore::DetectObjects(cv::Mat frame){
	cv::flip(frame, frame, 1);

	cv::Mat src, dst;
	src = frame.clone();
	dst = frame.clone();

	/*Convert to the Gue Saturation Value colorspace
		This makes actually finding the correct color a lot easier
		Because when we look for the specific color we don't have to take in to account
		How brightness changes the RGB values it'll only change the V in HSV colorspace
	*/
	cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);

	cv::GaussianBlur(dst, dst, cv::Size(5, 5), 2, 2);

	cv::inRange(dst, lowerBound, upperBound, dst);
	
	/*Here we are just removing noise
		Erode takes the blacks and tries to remove some white
		Dialate is the opposite of erode
	*/
	cv::erode(dst, dst, erodeElement);
	cv::dilate(dst, dst, dilateElement);
	cv::dilate(dst, dst, dilateElement);
	cv::dilate(dst, dst, dilateElement);

	cv::imshow("dst", dst);

	//Store the Width and Width of the Image in a | size - 0 - size | format
	int width = src.size().width - (src.size().width / 2);

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::findContours(dst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	vector<vector<cv::Point> > approxCurve(contours.size()), hull(contours.size());
	vector<VisionCore::VisionObject> vObjects;

	for (int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours.at(i), hull.at(i));
		cv::approxPolyDP(contours.at(i), approxCurve.at(i), 0.1 * cv::arcLength(hull.at(i), false), true);

		//Create our temp VisionObject to assign variables
		VisionCore::VisionObject vObject; //VO => VisionObject

		cv::Moments m = cv::moments(contours.at(i), true);
		cv::Point center(m.m10 / m.m00, m.m01 / m.m00); //Finds the center

		cv::Rect boundingBox = cv::boundingRect(approxCurve.at(i));

		cv::RotatedRect rect = cv::minAreaRect(approxCurve.at(i));

		//Calculate the distance based off of the object width
		double distance = (actualWidth * focalLength) / (rect.size.width);


		// Do this on the RoboRIO!!!!

		cv::Point adjCenter(center.x - (center.x / 2), center.y - (center.y / 2));
		//To find the actual positin on the 2D plane of the Distance we can use the tangent of the FOV
		//Max Horizontal/Veritcal Visibility = distance * tan( FOV / 2 )
		//Objects position is: (Center Position / Max Position) * Max Visibility
		double maxVisibility = distance * tan(FOV / 2);
		cv::Point realPos(((double)adjCenter.x / (double)width) * maxVisibility, ((double)adjCenter.x / (double)width) * maxVisibility);
		
		// ---------------------------------------------------

		//Set the scruct value
		vObject.actualWidth = actualWidth;
		vObject.center = center;
		vObject.distance = distance;
		vObject.pixelWidth = boundingBox.width;
		vObject.angle = rect.angle; //Calculate the screw?

		vObjects.push_back(vObject);

		cv::rectangle(src, boundingBox, cv::Scalar(255, 255, 0));

		cv::Point2f points[4];
		rect.points(points);
		drawSquare(src, points, cv::Scalar(255, 255, 255));

		cv::putText(src, to_string(realPos.x), vObject.center, cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 0, 0));
	}

	cv::imshow("src", src);

	if(vObjects.empty()){
		return NULL;
	}

	return &vObjects[0];
}

VisionCore::VisionObject* VisionCore::DetectObjects(cv::UMat frame) {
	cv::flip(frame, frame, 1);
	
	cv::UMat uDialateElement, uErodeElement;

	dilateElement.copyTo(uDialateElement);
	erodeElement.copyTo(uErodeElement);

	cv::UMat src, dst;
	frame.copyTo(src);
	frame.copyTo(dst);

	cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);

	cv::GaussianBlur(dst, dst, cv::Size(5, 5), 2, 2);

	cv::inRange(dst, upperBound, upperBound, dst);

	cv::erode(dst, dst, uErodeElement);
	cv::dilate(dst, dst, uDialateElement);

	cv::imshow("dst", dst);

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::findContours(dst, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	vector<vector<cv::Point> > approxCurve(contours.size()), hull(contours.size());
	vector<VisionObject> vObjects;

	for (int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours.at(i), hull.at(i));
		cv::approxPolyDP(contours.at(i), approxCurve.at(i), 0.1 * cv::arcLength(hull.at(i), false), true);

		//Create our temp VisionObject to assign variables
		VisionCore::VisionObject vObject; //VO => VisionObject

		cv::Moments m = cv::moments(contours.at(i), true);
		cv::Point center(m.m10 / m.m00, m.m01 / m.m00); //Finds the center

		cv::Rect boundingBox = cv::boundingRect(approxCurve.at(i));

		cv::RotatedRect rect = cv::minAreaRect(approxCurve.at(i));

		//Calculate the distance based off of the object width
		double distance = (actualWidth * focalLength) / rect.size.width;

		//To find the actual positin on the 2D plane of the Distance we can use the tangent of the FOV

		//Set the scruct value
		vObject.actualWidth = actualWidth;
		vObject.center = center;
		vObject.distance = distance;
		vObject.pixelWidth = boundingBox.width;
		vObject.angle = rect.angle;

		cv::Point2f points[4];
		rect.points(points);
		drawSquare(src, points, cv::Scalar(255, 255, 255));

	}

	cv::imshow("src", src);

	if (vObjects.empty()) {
		return NULL;
	}

	return &vObjects[0];
}
