#include "VideoHelpers.h"
#include "Table2D.h"
#include "Balls.h"
#include "BallTracker.h"
#include "Testing.h"
#include <opencv2/video/tracking.hpp>
#include "Segmentation.h"

/*
* 
* Function that, given video filename, a videocapture and a mat frame obj, shows the video read
* 
*/
void VideoHelpers::showVideo(cv::VideoCapture cap) {
    Ptr<DISOpticalFlow> disOpticalFlow = DISOpticalFlow::create();
    Mat prevFrame, flow, blurred, mask3d, frame;
    int countFrame = 0;
    cv::Mat greyMat, table;
    cv::Mat elaboratedFrame;
    std::vector<cv::Vec2f> lines;
    std::vector<cv::Vec3f> circles;
   
    for (;;) {
        cap >> frame;
        blurred = frame.clone();
        vector<Mat> channels(3);
        // split img:
        split(blurred, channels);
        Mat b, g, r;

;       cv::cvtColor(frame, greyMat, cv::COLOR_BGR2GRAY);
        bilateralFilter(channels[0], b, 50, 100, 3);
        bilateralFilter(channels[1], g, 50, 100, 3);
        bilateralFilter(channels[2], r, 50, 100, 3);
        channels[0] = b;
        channels[1] = g;
        channels[2] = r;

        merge(channels, blurred);


        //GaussianBlur(greyMat, greyMat, Size(3, 3), 0);
        // DrawItems(ObjectRecognition::detectPlayingField(greyMat), elaboratedFrame, 0);

            //--------------------------------
        vector<Point> corners;
        Mat output;
        elaboratedFrame = frame.clone();
        tie(corners, output) = Table2D::detectBilliardTable(blurred);
        // Ensure table is initialized and has the same type and number of rows as elaboratedFrame
        table = Table2D::creatTable(elaboratedFrame, corners);
        if (table.empty()) {
            table = cv::Mat::zeros(elaboratedFrame.size(), elaboratedFrame.type());
        }

        if (table.rows != elaboratedFrame.rows) {
            cv::resize(table, table, cv::Size(table.cols, elaboratedFrame.rows));
        }

        if (table.type() != elaboratedFrame.type()) {
            cv::cvtColor(table, table, cv::COLOR_BGR2GRAY);
            cv::cvtColor(table, table, cv::COLOR_GRAY2BGR);
        }

        //Mat combinedFrame;

        mask3d = Table2D::TableMask(blurred);
        circles = Balls::detectBilliardBalls(mask3d);
        vector<vector<int>>outputData = Balls::drawBalls(elaboratedFrame, circles, corners, blurred);
        Mat mask2d = Table2D::TableMask(table);

        //cv::hconcat(elaboratedFrame, table, combinedFrame);

    /*    imshow("Detected Billiard Table", mask2d);
        waitKey();*/
        if (countFrame == 0) {
            vector<vector<int>> testData = Testing::importTestData("test/game1_clip1/bounding_boxes/frame_first_bbox.txt");
            Testing::computeMeanAvaragePrecision(outputData, testData);
         /*   Mat mask = Segmentation::segmentImg(mask3d, outputData);
            Mat testMask = imread("test/game1_clip1/masks/frame_first.png");
            Testing::computeMeanIntersectionOverUnion(mask, testMask);*/
        }
        waitKey();
        if (frame.empty())
            break;
        if (!prevFrame.empty()) {
            BallTracker::trackBall(prevFrame, mask3d, flow, disOpticalFlow, elaboratedFrame, circles);
        }
        prevFrame = mask3d.clone();
        imshow("Test", elaboratedFrame);
        waitKey(20);
        countFrame++;
     /*   imshow("Image with balls", elaboratedFrame);
        waitKey(1);*/


        //--------------------------------

        // Concatenate table image and frame side by side
        // cv::Mat combinedFrame;
        // cv::hconcat(elaboratedFrame, table, combinedFrame);

        // cv::imshow(filename, combinedFrame);

        //if (cv::waitKey(20) >= 0)
        //    break;  
    }
}
