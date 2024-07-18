#include <opencv2/opencv.hpp>
#include <iostream>
#include "VideoHelpers.h"
#include "FrameProcessing.h"
#include "Testing.h"
#include "Segmentation.h"
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    string filename = "videos/game1_clip1.mp4";
    if (argc > 0) filename = argv[1];

    //PROCESSING PART
    //VideoHelpers::showVideo(filename);

    //TESTING PART
    string path = "test/game";
    string firstFramePath = "frames/frame_first.png";
    string lastFramePath = "frames/frame_first.png";
    string filenametest= "";
    string firstBboxPath = "bounding_boxes/frame_first_bbox.txt";
    string lastBboxPath = "bounding_boxes/frame_last_bbox.txt";
    string firstSegMask = "masks/frame_first.png";
    string lastSegMask = "masks/frame_last.png";
    vector<vector<int>> outputData, testData;
    Mat outputSegMask, testSegMask;
    Mat testImg = Mat::zeros(Size(5, 5), CV_8UC3);
    for (int game = 1; game < 4 ; game++) {
        for (int clip = 1; clip < 4; clip++) {
            if (empty(testImg) || (game > 1 && clip > 2)) break;
            filenametest = path + to_string(game) + "_clip" + to_string(clip) + "/";
            //FRAME FIRST
            cout << endl << filenametest << " first" << endl;
            testImg = imread(filenametest + firstFramePath);
            testData = Testing::importTestData(filenametest + firstBboxPath);
            tie(outputData, outputSegMask) = FrameProcessing::processFrame(testImg);
            Testing::computeMeanAvaragePrecision(outputData, testData);
            testSegMask = imread(filenametest + firstSegMask);
            //Testing::computeMeanIntersectionOverUnion(testImg, outputSegMask);

            //FRAME LAST
            cout << endl << filenametest << " last" << endl;
            testData = Testing::importTestData(filenametest + lastBboxPath);
            testImg = imread(filenametest + lastFramePath);
            tie(outputData, outputSegMask) = FrameProcessing::processFrame(testImg);
            Testing::computeMeanAvaragePrecision(outputData, testData);
            testSegMask = imread(filenametest + lastSegMask);
            //Testing::computeMeanIntersectionOverUnion(outputSegMask, outputSegMask);
        }
    }
    return 0;
}