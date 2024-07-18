#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class FrameProcessing
{
	public:
		static Mat blurImage(Mat img);
		static Mat projectTable(Mat img, Mat blurred, vector<Point> corners);
		static vector<vector<int>> processBalls(Mat img, Mat blurred, Mat mask3d, vector<Point> corners);
		static tuple<vector<vector<int>>, Mat> processFrame(Mat img);
};

