#include "ObjectRecognition.h"

/**
* 
*/
vector<Vec2f> ObjectRecognition::detectPlayingField(Mat greyMat) {
	Mat canny;
	Canny(greyMat, canny, 70, 200, 3);
	vector<Vec2f> lines;
	HoughLines(canny, lines, 1, CV_PI / 180, 150, 0, 0);
	return lines;
}

/**
* 
 * function that recognizes the balls in the billiard field 
 * using HoughCircles
 * 
*/
vector<Vec3f> ObjectRecognition::detectBilliardBalls(Mat greyMat) {
	Mat canny;
	//for (int i = 0; i <= 100; i += 10) {
	//	for (int j = 0; j <= 100; j += 10) {
	//		Canny(greyMat, canny, i, j);
	//		imshow("canny " + to_string(i) + "," + to_string(j), canny);
	//		waitKey(0);
	//	}
	//}
	vector<Vec3f> circles;
	HoughCircles(greyMat, circles, HOUGH_GRADIENT, 1, 10, 14, 15.5, 5, 12);
	return circles;
}
