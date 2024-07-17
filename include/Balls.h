#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Balls
{
	public:
		static vector<Vec3f> detectBilliardBalls(Mat frame);
		static void projectBalls(vector<Vec3f> circles);
		static vector<vector<int>> drawBalls(Mat& originalImg, vector<Vec3f> circles, vector<Point> corners, Mat blurred);
	private: 
		static int getBallsClass(Mat mat, int radius, Point center);

};

