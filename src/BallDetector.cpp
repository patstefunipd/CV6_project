#include "BallDetector.h"
using namespace cv;
using namespace std;

vector<Ball> BallDetector::detectBalls(const Mat& frame, const Mat& playingField) {
    
    vector<Ball> balls;

    
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 8, 200, 20, 0, 0);

    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        Rect boundingBox(center.x - radius, center.y - radius, radius * 2, radius * 2);
        balls.push_back({boundingBox, 3}); 
    }

    return balls;
}
