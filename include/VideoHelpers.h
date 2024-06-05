#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <typeinfo>
using namespace std;
using namespace cv;

class VideoHelpers
{
	public:
		static void showVideo(string filename, VideoCapture cap, Mat frame);
};

