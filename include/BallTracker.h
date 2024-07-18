#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class BallTracker
{
	public:
		static bool trackBall(Mat prevFrame, Mat greyMat, Mat flow, Ptr<DISOpticalFlow> disOpticalFlow, Mat &img, vector<vector<int>> circles);
};

