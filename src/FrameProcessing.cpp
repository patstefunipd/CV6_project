#include "FrameProcessing.h"
#include "Table2D.h"
#include "Balls.h"
#include "Testing.h"
#include "Segmentation.h"
#include "MiniMap.h"

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

tuple<Mat, Mat, Mat, int, int> FrameProcessing::projectTable(Mat img, Mat blurred, vector<Point> corners){
    Mat output, table, projectionMatrix;
    Mat elaboratedFrame = img.clone();
    int width, height;
    // Ensure table is initialized and has the same type and number of rows as elaboratedFrame
    tie(width, height) = Table2D::CalculateWidthHeight(corners);
    tie(table, projectionMatrix) = Table2D::creatTable(elaboratedFrame, corners, width, height);
    if (table.empty()) {
        table = cv::Mat::zeros(elaboratedFrame.size(), elaboratedFrame.type());
    }

    //if (table.rows != elaboratedFrame.rows) {
    //    cv::resize(table, table, cv::Size(table.cols, elaboratedFrame.rows));
    //}

    if (table.type() != elaboratedFrame.type()) {
        cv::cvtColor(table, table, cv::COLOR_BGR2GRAY);
        cv::cvtColor(table, table, cv::COLOR_GRAY2BGR);
    }

    //Mat combinedFrame;

   
    Mat mask2d = Table2D::TableMask(table);

    //cv::hconcat(elaboratedFrame, table, combinedFrame);

/*    imshow("Detected Billiard Table", mask2d);
    waitKey();*/
    return make_tuple(mask2d, projectionMatrix, table, width, height);
}

vector<vector<int>> FrameProcessing::processBalls(Mat img, Mat blurred, Mat mask3d, vector<Point> corners) {
    vector<Vec3f> circles;
    circles = Balls::detectBilliardBalls(mask3d);
    vector<vector<int>>outputData = Balls::drawBalls(img, circles, corners, blurred);
    return outputData;
}

tuple<vector<vector<int>>, Mat, Mat, Mat> FrameProcessing::processFrame(Mat img) {
    vector<Point> corners;
    Mat projMatrix, proj2d, output, topView;
    int width, height;
    Mat blurred = blurImage(img);
    tie(corners, output) = Table2D::detectBilliardTable(img);
    tie(proj2d, projMatrix, topView, width, height) = projectTable(img, blurred, corners);
    Mat mask3d = Table2D::TableMask(blurred);
    vector<vector<int>> balls = processBalls(img, blurred, mask3d, corners);
    proj2d = MiniMap::buildMiniMap(img, projMatrix, balls, width, height);
    Mat segmentMask = Segmentation::segmentImg(mask3d, balls);
    return make_tuple(balls, segmentMask, mask3d, proj2d);
}