#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Balls
{
	public:
		static vector<Vec3f> detectBilliardBalls(cv::Mat frame);
		static vector<vector<int>> drawBalls(cv::Mat& originalImg, vector<Vec3f> circles, vector<Point> corners, cv::Mat blurred);
		static Vec3f getColorClass(int ballClass);
	private: 
		static int getBallsClass(cv::Mat mat, int radius, Point center);

};

