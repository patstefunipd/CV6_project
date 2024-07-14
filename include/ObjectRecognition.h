#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ObjectRecognition
{
	public:
		static Vec3f         getBallsClass(Mat mat, int radius, Point center);
		static vector<Vec3f> detectBilliardBalls(Mat frame);
};

