#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ObjectRecognition
{
	public:
		static vector<Vec2f> detectPlayingField(Mat greyMat);
		static vector<Vec3f> detectBilliardBalls(Mat frame);
};

