#include "ObjectRecognition.h"

Vec3f ObjectRecognition::getBallsClass(Mat img, int radius, Point center) {
	Vec3b currRGB;
	int countBlackPixels     = 0;
	int countWhitePixels     = 0;
	int countWhiteInColoured = 0;
	int countColouredPixels  = 0;

	for (int i = center.y - radius; i < center.y + radius; i++) {
		for (int j = center.x - radius; j < center.x + radius; j++) {
			if (i >= 0 && i < img.rows && j >= 0 && j < img.cols) {
					currRGB = img.at<Vec3b>(i, j);
					if (currRGB[0] < 40 && currRGB[1] < 40 && currRGB[2] < 40) {
						countBlackPixels++;
					}
					else if (currRGB[0] < 90 || currRGB[1] < 90 || currRGB[2] < 90) {
						countColouredPixels++;
						//img.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					}
					else {
						countWhitePixels++;
						//img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
					}
			}
		}
	}
	//imshow("Threshold", img);
	//waitKey();

	countBlackPixels /= radius ^ 2;
	countWhitePixels /= radius ^ 2;
	countColouredPixels /= radius ^ 2;

	if (countBlackPixels > 2) {
		return Vec3b(0, 0, 0);
	}
	if (countWhitePixels > countColouredPixels && countWhitePixels > 30) {
		return Vec3b(255, 255, 255);
	}
	//is striped
	if (countWhitePixels > 4) {
		return Vec3b(255, 0, 0);
	}
	return Vec3b(0,0,255);
}

/**
* 
 * function that recognizes the balls in the billiard field 
 * using HoughCircles
 * 
*/
vector<Vec3f> ObjectRecognition::detectBilliardBalls(Mat greyMat) {
	vector<Vec3f> circles;
	Mat canny;
	//Canny(greyMat, canny, 80, 40);
	//imshow("canny", canny);
	//waitKey();
	HoughCircles(greyMat, circles, HOUGH_GRADIENT, 0.9, 12, 14, 15.2, 5, 21);
	return circles;
}
