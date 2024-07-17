#include "BallTracker.h"

bool BallTracker::trackBall(Mat prevFrame, Mat frame, Mat flow, Ptr<DISOpticalFlow> disOpticalFlow, Mat &img, vector<Vec3f> circles) {
    disOpticalFlow->calc(prevFrame, frame, flow);
    int threshold = 10;
    bool result = false;
    for (int i = 0; i < circles.size(); i++) {
        cv::Point center = cv::Point(circles[i][0], circles[i][1]);
        int radius = circles[i][2];
        Point2f flowAtPoint = flow.at<Point2f>(center.y, center.x);
        if (norm(flowAtPoint) > 2) {
            Point beginningOfFlow = Point(center.x + flowAtPoint.x, center.y + flowAtPoint.y);
            Point2f lineEnd = beginningOfFlow;
            bool isColliding = false;
            int max = 100;
            while (!isColliding && max > 0) {
                for (int collidingBalls = 0; collidingBalls < circles.size(); collidingBalls++) {
                    if (!isColliding) {
                        lineEnd.x += flowAtPoint.x * 3;
                        lineEnd.y += flowAtPoint.y * 3;
                    }
                    if (collidingBalls != i) {
                        cv::Point centerColl = cv::Point(circles[i][0], circles[i][1]);
                        int radiusColl = circles[i][2];
                        if (lineEnd.x > centerColl.x - radiusColl - threshold && lineEnd.x < centerColl.x + radiusColl + threshold &&
                            lineEnd.y > centerColl.y - radiusColl - threshold && lineEnd.y < centerColl.y + radiusColl + threshold) {
                            isColliding = true;
                            cout << "it is colliding" << endl;
                        }
                        if (isColliding) {
                            break;
                        }
                    }
                    max--;
                }
            }
            line(img, Point(center.x, center.y), lineEnd , Scalar(0,0,255), 2);
            result = true;
        }
    }
    return result;
}