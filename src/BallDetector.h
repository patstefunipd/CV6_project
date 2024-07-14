#ifndef BALLDETECTOR_H
#define BALLDETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>

struct Ball {
    cv::Rect boundingBox;
    int categoryID; 
};

class BallDetector {
public:
    std::vector<Ball> detectBalls(const cv::Mat& frame, const cv::Mat& playingField);
};

#endif // BALLDETECTOR_H
