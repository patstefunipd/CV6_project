#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Table2D
{	
	private:
        static int width;
        static int height;
	public:
		static Mat creatTable();
};