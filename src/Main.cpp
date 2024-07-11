#include <opencv2/opencv.hpp>
#include <iostream>
#include "VideoHelpers.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <video_file_path>" << endl;
        return -1;
    }

    string filename = argv[1];
    cv::VideoCapture capture(filename);
    cv::Mat frame;

    if (!capture.isOpened()) {
        cerr << "Error: Could not open video file " << filename << endl;
        return -1;
    }

    VideoHelpers::showVideo(filename, capture, frame);

    return 0;
}
