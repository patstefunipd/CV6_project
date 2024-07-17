#include <opencv2/opencv.hpp>
#include <iostream>
#include "VideoHelpers.h"
#include "Table2D.h"
#include "Balls.h"
#include "BallTracker.h"
#include "Testing.h"
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    string filename = "videos/game1_clip1.mp4";
    if (argc > 0) filename = argv[1];
    //COMPUTE FIELD STUFF
    VideoCapture capture(filename);
    Mat frame, frame2;
    if (!capture.isOpened())
        throw "Error when reading " + filename;
    if (!capture.isOpened())
        throw "Error when reading " + filename;
    VideoHelpers::showVideo(capture);
    return 0;
}