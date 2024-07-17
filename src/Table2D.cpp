#include "Table2D.h"
#include "Balls.h"

// Initialize static members
int Table2D::width = 540; 
int Table2D::height = 560; 


Mat Table2D::creatTable(const Mat originalFrame , const std::vector<cv::Point> cornerPoints) {
    // Ensure cornerPoints contains exactly 4 points
    if (cornerPoints.size() != 4) {
        throw std::invalid_argument("cornerPoints must contain exactly 4 points.");
    }

    // Convert cornerPoints from cv::Point to cv::Point2f
    std::vector<cv::Point2f> pts1;
    for (const auto &point : cornerPoints) {
        pts1.emplace_back(static_cast<float>(point.x), static_cast<float>(point.y));
        circle(originalFrame, Point(point.x, point.y), 10, Scalar(0,0,255), 2);
    }

    // Define the points of the output image
    std::vector<cv::Point2f> pts2 = {
        cv::Point2f(0.0f, 0.0f),
        cv::Point2f(static_cast<float>(width), 0.0f),
        cv::Point2f(0.0f, static_cast<float>(height)),
        cv::Point2f(static_cast<float>(width), static_cast<float>(height))
    };

    // Getting perspective transform matrix by 4 points of each image
    cv::Mat matrix = cv::getPerspectiveTransform(pts1, pts2);

    // Applying perspective warp
    cv::Mat transformed;
    cv::warpPerspective(originalFrame, transformed, matrix, cv::Size(width, height));

    return transformed;
}



std::tuple<std::vector<Point>, Mat> Table2D::detectBilliardTable(Mat& frame) {
    // Convert to HSV color space
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Blur the image to reduce noise
    GaussianBlur(hsv, hsv, Size(9, 9), 2, 2);

    // Compute the hue bounds dynamically
    Scalar lower_bound, upper_bound;
    tie(lower_bound, upper_bound) = GetClothColor(hsv, 10); 

    // Create a mask for the dominant hue
    Mat mask;
    inRange(hsv, lower_bound, upper_bound, mask);

    // Apply morphological operations to clean up the mask
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);
    morphologyEx(mask, mask, MORPH_OPEN, kernel);

    // Detect edges using Canny on the mask
    Mat edges;
    Canny(mask, edges, 50, 150);

    // Find contours
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Find the largest contour
    vector<Point> tableContour;
    double maxArea = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            tableContour = contours[i];
        }
    }

    // Draw the detected table contour on the frame
    Mat output = frame.clone();
    std::vector<Point> cornerPoints;
    if (!tableContour.empty()) {
        vector<vector<Point>> drawContoursVec;
        drawContoursVec.push_back(tableContour);
        drawContours(output, drawContoursVec, -1, Scalar(0, 255, 0), 2);

        // Optionally, you can draw a bounding box around the detected contour
        Rect boundingBox = boundingRect(tableContour);
        rectangle(output, boundingBox.tl(), boundingBox.br(), Scalar(255, 0, 0), 2);

        // Extract corner points
        Point tl = boundingBox.tl(); // Top-left corner
        Point br = boundingBox.br(); // Bottom-right corner
        Point tr = Point(boundingBox.x + boundingBox.width, boundingBox.y); // Top-right corner
        Point bl = Point(boundingBox.x, boundingBox.y + boundingBox.height); // Bottom-left corner

        // Print or use the corner points as needed
        // cout << "Top-left: " << tl << endl;
        // cout << "Top-right: " << tr << endl;
        // cout << "Bottom-left: " << bl << endl;
        // cout << "Bottom-right: " << br << endl;

        // Store corner points in the vector
        cornerPoints.push_back(tl);
        cornerPoints.push_back(tr);
        cornerPoints.push_back(bl);
        cornerPoints.push_back(br);        
    }
    return make_tuple(cornerPoints, output);


}

std::tuple<cv::Scalar, cv::Scalar> Table2D::GetClothColor(const cv::Mat& hsv, int search_width) {
    cv::Mat hist;

    // Compute the histogram of the H channel
    int histSize = 180;
    float h_range[] = {0, 180};
    const float* h_ranges[] = {h_range};
    int channels[] = {0};
    cv::calcHist(&hsv, 1, channels, cv::Mat(), hist, 1, &histSize, h_ranges, true, false);
    int h_max = Table2D::Indexer::getIndexOfMax(hist);

    // Compute the histogram of the S channel
    histSize = 256;
    float s_range[] = {0, 256};
    const float* s_ranges[] = {s_range};
    channels[0] = 1;
    cv::calcHist(&hsv, 1, channels, cv::Mat(), hist, 1, &histSize, s_ranges, true, false);
    int s_max = Table2D::Indexer::getIndexOfMax(hist);

    // Compute the histogram of the V channel
    histSize = 256;
    float v_range[] = {0, 256};
    const float* v_ranges[] = {v_range};
    channels[0] = 2;
    cv::calcHist(&hsv, 1, channels, cv::Mat(), hist, 1, &histSize, v_ranges, true, false);
    int v_max = Table2D::Indexer::getIndexOfMax(hist);

    // Define the range of the color in HSV
    cv::Scalar lower_color(h_max - search_width, s_max - search_width, v_max - search_width);
    cv::Scalar upper_color(h_max + search_width, s_max + search_width, v_max + search_width);

    return std::make_tuple(lower_color, upper_color);
}


Mat Table2D::TableMask(const Mat frame) {

    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    GaussianBlur(hsv, hsv, Size(7, 7), 2, 2);

    Scalar lower_bound, upper_bound;
    tie(lower_bound, upper_bound) = GetClothColor(hsv, 30); 

    Mat mask;
    inRange(hsv, lower_bound, upper_bound, mask);


    Mat kernel = Mat::ones(Size(5, 5), CV_8U);
    Mat mask_closing;
    morphologyEx(mask, mask_closing, MORPH_CLOSE, kernel);

    Mat mask_inv;
    threshold(mask_closing, mask_inv, 5, 255, THRESH_BINARY_INV);

    Mat masked_img;
    bitwise_and(frame, frame, masked_img, mask_inv);

    //morphologyEx(mask_inv, mask_inv, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    //erode(mask_inv, mask_inv, getStructuringElement(MORPH_ELLIPSE, Size(7,7)), Point(-1, -1), 1, 1, 1);
    //Mat kernel2 = Mat::ones(Size(13, 13), CV_8U);
    //dilate(mask_inv, mask_inv, kernel);


    //imshow("Table Mask", mask_closing);
    //imshow("Masked Objects", masked_img);

    return mask_inv;
}