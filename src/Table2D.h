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

    // static Mat creatTable(const Mat originalFrame , const Point tl, const Point tr, const Point br, const Point bl);
    static Mat detectBilliardTable(Mat& frame);
    static tuple<Scalar, Scalar> GetClothColor(const Mat& hsv, int search_width = 45);

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
