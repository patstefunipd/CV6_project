#include "MiniMap.h"
#include "Balls.h"

//PATRIZIA STEFANI

void MiniMap::drawHoles(Mat& miniMap, int threshold) {
    circle(miniMap, Point(threshold, threshold), 15, Scalar(105, 105, 105), -1);
    circle(miniMap, Point(miniMap.cols / 2, threshold), 15, Scalar(105, 105, 105), -1);
    circle(miniMap, Point(miniMap.cols - threshold, threshold), 15, Scalar(105, 105, 105), -1);
    circle(miniMap, Point(miniMap.cols / 2, miniMap.rows - threshold), 15, Scalar(105, 105, 105), -1);
    circle(miniMap, Point(threshold, miniMap.rows - threshold), 15, Scalar(105, 105, 105), -1);
    circle(miniMap, Point(miniMap.cols - threshold, miniMap.rows - threshold), 15, Scalar(105, 105, 105), -1);
}

void MiniMap::drawBoundaries(Mat& miniMap, int threshold) {
    line(miniMap, Point(threshold / 2 + 6, threshold / 2), Point(miniMap.cols - 6, threshold / 2), Scalar(0, 0, 0), 6);
    line(miniMap, Point(threshold / 2, threshold / 2 + 6), Point(threshold / 2, miniMap.rows - 6), Scalar(0, 0, 0), 6);
    line(miniMap, Point(miniMap.cols - threshold / 2, threshold / 2 + 6), Point(miniMap.cols - threshold / 2, miniMap.rows - 6), Scalar(0, 0, 0), 6);
    line(miniMap, Point(threshold / 2, miniMap.rows - threshold / 2), Point(miniMap.cols - threshold / 2, miniMap.rows - threshold / 2), Scalar(0, 0, 0), 6);
}

void MiniMap::drawField(Mat& miniMap) {
    for (int x = 0; x < miniMap.cols; x++) {
        for (int y = 0; y < miniMap.rows; y++) {
            miniMap.at<Vec3b>(y, x) = Vec3f(0, 255, 0);
        }
    }
}

Mat MiniMap::concatMiniMapToImg(Mat original, Mat miniMap) {
    Mat result, resizedMiniMap;
    int newCols = miniMap.cols;
    int newRows = miniMap.rows;
    if (newCols > original.cols/4) {
        newCols = original.cols / 4;
    }
    if (newRows > original.rows / 4) {
        newRows = original.rows / 4;
    }
  resize(miniMap, resizedMiniMap, Size(newCols, newRows), INTER_LINEAR);
  cv::Mat indicator(original, cv::Rect(0, 0, newCols, newRows));
  resizedMiniMap.copyTo(indicator);
  return original;
}

Mat MiniMap::buildMiniMap(Mat original, Mat perspectiveMat, vector<vector<int>> balls, int width, int height) {
    Mat miniMap = Mat::zeros(height, width, CV_8UC3);
    vector<Point2f> projectedPoint2D;
    vector<Point2f> points;
    int threshold = 5;

    drawField(miniMap);
    drawBoundaries(miniMap, threshold);
    drawHoles(miniMap, threshold);

    for (int i = 0; i < balls.size(); i++) {
        threshold = balls[i][3] / 2 - 1;
        Point2f tl = Point2f(balls[i][0] + threshold, balls[i][1] + threshold);
        Point2f tr = Point2f(balls[i][0] + balls[i][2] - threshold, balls[i][1] + threshold);
        Point2f bl = Point2f(balls[i][0] + threshold, balls[i][1] + balls[i][3] - threshold);
        Point2f br = Point2f(balls[i][0] + balls[i][2] - threshold, balls[i][1] + balls[i][3] - threshold);

        points.push_back(tl);
        points.push_back(tr);
        points.push_back(bl);
        points.push_back(br);


        cv::perspectiveTransform(points, projectedPoint2D, perspectiveMat);
        if (!projectedPoint2D.empty()) {
            cv::Point projectedPoint(static_cast<int>(projectedPoint2D[0].x), static_cast<int>(projectedPoint2D[0].y));
            cv::circle(miniMap, projectedPoint, 7, Balls::getColorClass(balls[i][4]), -1);
        }
        points.clear();
        projectedPoint2D.clear();
    }
    original = concatMiniMapToImg(original,miniMap);
    imshow("orig", original);
    waitKey();
    return original;
}