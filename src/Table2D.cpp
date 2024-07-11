#include "Table2D.h"


// Initialize static members
int Table2D::width = 280; 
int Table2D::height = 560; 

int width = 280, height = 560;
Mat Table2D::creatTable() {
	// Create a new image with specified width, height, and 3 color channels
    Mat img = Mat::zeros(height, width, CV_8UC3);
    
    img.setTo(Scalar(10, 180, 0)); // BGR color

    circle(img, Point(width / 2, height / 5), width / 6, Scalar(50, 255, 50), FILLED);

    rectangle(img, Point(0, height / 5), Point(width, height), Scalar(10, 180, 0), FILLED);

    line(img, Point(0, height / 5), Point(width, height / 5), Scalar(50, 255, 50));

    cvtColor(img, img, COLOR_BGR2RGB);

    return img;
}
