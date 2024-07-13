#include "Table2D.h"


// Initialize static members
int Table2D::width = 540; 
int Table2D::height = 560; 


// Mat Table2D::creatTable(const Mat originalFrame , const Point tl, const Point tr, const Point br, const Point bl) {
//     // Create a new image with specified width, height, and 3 color channels
//     Mat img = Mat::zeros(height, width, CV_8UC3);
    
//     // Set the background color
//     img.setTo(Scalar(10, 180, 0)); // BGR color

//     // Draw the filled rectangle representing the table area
//     rectangle(img, Point(0, height / 5), Point(width, height), Scalar(10, 180, 0), FILLED);

//     // Convert the image to RGB color space
//     cvtColor(img, img, COLOR_BGR2RGB);

//     // Crop and overlay the original frame onto the table area
//     if (!originalFrame.empty()) {
//         // Create a mask for the table area
//         Mat mask = Mat::zeros(originalFrame.size(), CV_8UC1);
//         vector<Point> tableContour = {tl, tr, br, bl}; // Using the instance-specific corners
//         vector<vector<Point>> drawContoursVec = {tableContour};
//         drawContours(mask, drawContoursVec, -1, Scalar(255), FILLED);

//         // Apply the mask to extract the table region from the original frame
//         Mat tableRegion;
//         originalFrame.copyTo(tableRegion, mask);

//         // Resize the table region to fit into the table image
//         resize(tableRegion, tableRegion, Size(width, height));

//         // Overlay the table region onto the table image
//         tableRegion.copyTo(img(Rect(0, 0, tableRegion.cols, tableRegion.rows)));
//     }

//     return img;
// }



Mat Table2D::detectBilliardTable(Mat& frame) {

    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    GaussianBlur(hsv, hsv, Size(9, 9), 2, 2);

    Scalar lower_bound, upper_bound;
    tie(lower_bound, upper_bound) = GetClothColor(hsv, 10);

    Mat mask;
    inRange(hsv, lower_bound, upper_bound, mask);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);
    morphologyEx(mask, mask, MORPH_OPEN, kernel);

    Mat edges;
    Canny(mask, edges, 50, 150);

    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<Point> tableContour;
    double maxArea = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            tableContour = contours[i];
        }
    }

    Mat output = frame.clone();
    if (!tableContour.empty()) {
        vector<vector<Point>> drawContoursVec;
        drawContoursVec.push_back(tableContour);
        drawContours(output, drawContoursVec, -1, Scalar(0, 255, 0), 2);

        Rect boundingBox = boundingRect(tableContour);
        rectangle(output, boundingBox.tl(), boundingBox.br(), Scalar(255, 0, 0), 2);
    }

    return output;
}





tuple<Scalar, Scalar> Table2D::GetClothColor(const Mat& hsv, int search_width) {
    Mat hist;

    // Compute the histogram of the H channel
    int histSize = 180;
    float h_range[] = {0, 180};
    const float* h_ranges[] = {h_range};
    int channels[] = {0};
    calcHist(&hsv, 1, channels, Mat(), hist, 1, &histSize, h_ranges, true, false);
    int h_max = Table2D::Indexer::getIndexOfMax(hist);

    // Compute the histogram of the S channel
    histSize = 256;
    float s_range[] = {0, 256};
    const float* s_ranges[] = {s_range};
    channels[0] = 1;
    calcHist(&hsv, 1, channels, Mat(), hist, 1, &histSize, s_ranges, true, false);
    int s_max = Table2D::Indexer::getIndexOfMax(hist);

    // Compute the histogram of the V channel
    histSize = 256;
    float v_range[] = {0, 256};
    const float* v_ranges[] = {v_range};
    channels[0] = 2;
    calcHist(&hsv, 1, channels, Mat(), hist, 1, &histSize, v_ranges, true, false);
    int v_max = Table2D::Indexer::getIndexOfMax(hist);

    // Define the range of the color in HSV
    Scalar lower_color(h_max - search_width, s_max - search_width, v_max - search_width);
    Scalar upper_color(h_max + search_width, s_max + search_width, v_max + search_width);

    return make_tuple(lower_color, upper_color);
}
