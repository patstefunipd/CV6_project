#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;

class Testing
{
	public:
		static vector<vector<int>> importTestData(string filename);
		static void computeMeanAvaragePrecision(vector<vector<int>> dataset1, vector<vector<int>> dataset2, Mat img);
		static void computeMeanIntersectionOverUnion();
};

