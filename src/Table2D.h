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

    static Mat creatTable(const Mat originalFrame , const std::vector<cv::Point> cornerPoints);
    static std::tuple<std::vector<Point>, Mat> detectBilliardTable(Mat& frame);
    static tuple<Scalar, Scalar> GetClothColor(const Mat& hsv, int search_width = 45);
    static Mat TableMask(const Mat frame);

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
