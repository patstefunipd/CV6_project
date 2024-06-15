#include "ball_detector.hpp"

BallDetector::BallDetector() {
    class_labels = {"cue_ball", "eight_ball", "solid_ball", "striped_ball"};
}

std::vector<DetectedObject> BallDetector::detectBalls(const cv::Mat& image_frame) {
    std::vector<DetectedObject> detected_objects;

    // Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(image_frame, gray, cv::COLOR_BGR2GRAY);

    // Use Gaussian blur to reduce noise and improve detection accuracy
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    // Use HoughCircles to detect circles in the image
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 8, 200, 20, 10, 30);

    // Iterate over the detected circles and create bounding boxes
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Vec3f circle = circles[i];
        int center_x = cvRound(circle[0]);
        int center_y = cvRound(circle[1]);
        int radius = cvRound(circle[2]);

        // Define the bounding box around the detected circle
        int x = center_x - radius;
        int y = center_y - radius;
        int width = 2 * radius;
        int height = 2 * radius;

        // Assuming all detected circles are balls, label them generically
        DetectedObject detected_obj = {x, y, width, height, "ball"};
        detected_objects.push_back(detected_obj);
    }

    return detected_objects;
}

