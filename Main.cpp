#include <opencv2/opencv.hpp>
#include <iostream>
#include "VideoHelpers.h"
using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{
    string filename = "videos/game1_clip1.mp4";
    if (argc > 0) filename = argv[0];
    VideoCapture capture(filename);
    Mat frame;

    if (!capture.isOpened())
        throw "Error when reading " + filename;

    VideoHelpers::showVideo(filename, capture, frame);
    return 0;
}