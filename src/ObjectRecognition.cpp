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
				if (currRGB[0] < 50 && currRGB[1] < 50 && currRGB[2] < 50) {
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
	if (countBlackPixels > 30) {
		return Vec3b(0, 0, 0);
	}
	if (countWhitePixels > countColouredPixels && countWhitePixels > 120) {
		return Vec3b(255, 255, 255);
	}
	//is striped
	if (countWhitePixels > 30) {
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
	HoughCircles(greyMat, circles, HOUGH_GRADIENT, 1, 10, 14, 15.5, 5, 12);
	return circles;
}
