#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Segmentation
{
	public:
		static Mat segmentImg(Mat img, vector<vector<int>> ballClasses);
};

