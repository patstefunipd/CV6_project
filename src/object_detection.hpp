#pragma once

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

// Structure representing a detected object's bounding box
struct DetectedObject {
    int x, y, width, height; // Bounding box coordinates and dimensions
    std::string label;       // Label for the detected object
};

// Class for detecting balls in an image using basic image processing
class BallDetector {
public:
    // Constructor to initialize the detector
    BallDetector();

    // Method to perform detection on an input image frame
    std::vector<DetectedObject> detectBalls(const cv::Mat& image_frame);

private:
    std::vector<std::string> class_labels; // List of class names
};

