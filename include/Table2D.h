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

    static tuple<Mat, Mat> creatTable(const Mat originalFrame , const std::vector<cv::Point> cornerPoints, double width, double height);
    static std::tuple<std::vector<Point>, Mat> detectBilliardTable(Mat& frame);
    static tuple<Scalar, Scalar> GetClothColor(const Mat& hsv, int search_width = 45);
    static Mat TableMask(const Mat frame);
    static std::tuple<int, int> CalculateWidthHeight(std::vector<cv::Point> cornerPoints);
    static Mat k_means(Mat image);

    class Indexer {
        public:
            static int getIndexOfMax(const Mat& hist) {
                double maxVal = 0;
                Point maxLoc;
                minMaxLoc(hist, 0, &maxVal, 0, &maxLoc);
                return maxLoc.y;
            }
    };
};
