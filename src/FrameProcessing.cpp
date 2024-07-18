#include "FrameProcessing.h"
#include "Table2D.h"
#include "Balls.h"
#include "Testing.h"
#include "Segmentation.h"

Mat FrameProcessing::blurImage(Mat img) {
    Mat blurred = img.clone();
    vector<Mat> channels(3);
    split(blurred, channels);
    Mat b, g, r;
    bilateralFilter(channels[0], b, 50, 100, 3);
    bilateralFilter(channels[1], g, 50, 100, 3);
    bilateralFilter(channels[2], r, 50, 100, 3);
    channels[0] = b;
    channels[1] = g;
    channels[2] = r;
    merge(channels, blurred);
    return blurred;
}

Mat FrameProcessing::projectTable(Mat img, Mat blurred, vector<Point> corners){
    Mat output, table;
    Mat elaboratedFrame = img.clone();
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

   
    Mat mask2d = Table2D::TableMask(table);

    //cv::hconcat(elaboratedFrame, table, combinedFrame);

/*    imshow("Detected Billiard Table", mask2d);
    waitKey();*/
    return mask2d;
}

vector<vector<int>> FrameProcessing::processBalls(Mat img, Mat blurred, Mat mask3d, vector<Point> corners) {
    vector<Vec3f> circles;
    circles = Balls::detectBilliardBalls(mask3d);
    vector<vector<int>>outputData = Balls::drawBalls(img, circles, corners, blurred);
    return outputData;
}

tuple<vector<vector<int>>, Mat> FrameProcessing::processFrame(Mat img) {
    vector<Point> corners;
    Mat output;
    Mat blurred = blurImage(img);
    tie(corners, output) = Table2D::detectBilliardTable(img);
    Mat proj2d  = projectTable(img, blurred, corners);
    Mat mask3d = Table2D::TableMask(blurred);
    vector<vector<int>> balls = processBalls(img, blurred, mask3d, corners);
    Mat segmentMask = Segmentation::segmentImg(mask3d, balls);
    return make_tuple(balls, segmentMask);
}