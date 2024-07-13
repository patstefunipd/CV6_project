#include "Table2D.h"


// Initialize static members
int Table2D::width = 540; 
int Table2D::height = 560; 


Mat Table2D::creatTable(const Mat originalFrame , const vector<Point> cornerPoints) {
    // Create a new image with specified width, height, and 3 color channels
    Mat img = Mat::zeros(height, width, CV_8UC3);

    Point tl, tr, br, bl;
    if (cornerPoints.size() == 4) {
        tl = cornerPoints[0];
        tr = cornerPoints[1];
        bl = cornerPoints[2];
        br = cornerPoints[3];
    } else {
        // Handle error: not enough points
        cerr << "Error: cornerPoints vector does not contain exactly 4 points." << endl;
        return img;
    }
    
    // Set the background color
    img.setTo(Scalar(10, 180, 0)); // BGR color

    // Draw the filled rectangle representing the table area
    rectangle(img, Point(0, height / 5), Point(width, height), Scalar(10, 180, 0), FILLED);

    // Convert the image to RGB color space
    cvtColor(img, img, COLOR_BGR2RGB);

    // Crop and overlay the original frame onto the table area
    if (!originalFrame.empty()) {
        // Create a mask for the table area
        Mat mask = Mat::zeros(originalFrame.size(), CV_8UC1);
        vector<Point> tableContour = {tl, tr, br, bl}; // Using the instance-specific corners
        vector<vector<Point>> drawContoursVec = {tableContour};
        drawContours(mask, drawContoursVec, -1, Scalar(255), FILLED);

        // Apply the mask to extract the table region from the original frame
        Mat tableRegion;
        originalFrame.copyTo(tableRegion, mask);

        // Resize the table region to fit into the table image
        resize(tableRegion, tableRegion, Size(width, height));

        // Overlay the table region onto the table image
        tableRegion.copyTo(img(Rect(0, 0, tableRegion.cols, tableRegion.rows)));
    }

    return img;
}



std::tuple<std::vector<Point>, Mat> Table2D::detectBilliardTable(Mat& frame) {
    // Convert to HSV color space
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Blur the image to reduce noise
    GaussianBlur(hsv, hsv, Size(9, 9), 2, 2);

    // Compute the hue bounds dynamically
    Scalar lower_bound, upper_bound;
    tie(lower_bound, upper_bound) = GetClothColor(hsv, 10); // Using hue range of 10

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
        cout << "Top-left: " << tl << endl;
        cout << "Top-right: " << tr << endl;
        cout << "Bottom-left: " << bl << endl;
        cout << "Bottom-right: " << br << endl;

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
