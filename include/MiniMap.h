#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
class MiniMap
{
	private:
		static void drawHoles(cv::Mat& minimap, int threshold);
		static void drawBoundaries(cv::Mat& minimap, int threshold);
		static void drawField(cv::Mat& minimap);
	public:
		static Mat concatMiniMapToImg(cv::Mat original, cv::Mat miniMap);
		static Mat buildMiniMap(Mat original, Mat perspectiveMat, vector<vector<int>> balls, int width, int height);
};