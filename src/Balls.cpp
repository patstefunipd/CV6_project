#include "Balls.h"
#include "Table2D.h"

void projectBalls(vector<Vec3f> circles) {
	// Define the points of the output image
	//std::vector<cv::Point2f> pts2 = {
	//	cv::Point2f(0.0f, 0.0f),
	//	cv::Point2f(static_cast<float>(540), 0.0f),
	//	cv::Point2f(0.0f, static_cast<float>(560)),
	//	cv::Point2f(static_cast<float>(540), static_cast<float>(560))
	//};
	//Mat output(Size(540, 560), CV_8UC3, Scalar(0,255,0));
	//Point2f points;
	//for (int i = 0; i < circles.size(); i++) {;

	//	// Getting perspective transform matrix by 4 points of each image
	//	cv::Mat matrix = cv::getPerspectiveTransform(pts1, pts2);

	//	// Applying perspective warp
	//	cv::Mat transformed;
	//	cv::warpPerspective(output, transformed, matrix, cv::Size(width, height));
	//}
}



int Balls::getBallsClass(Mat img, int radius, Point center) {
	Vec3b currRGB;
	Mat ball = Mat::zeros(Size(radius * 2, radius * 2), CV_8UC3);
	int countBlackPixels = 0;
	int countWhitePixels = 0;
	int countWhiteInColoured = 0;
	int countColouredPixels = 0;
	int countBallPixelX = 0;
	int countBallPixelY = 0;

	for (int i = center.y - radius; i < center.y + radius; i++) {
		if (i == center.y - radius) countBallPixelY = 0;
		for (int j = center.x - radius; j < center.x + radius; j++) {
			if (i >= 0 && i < img.rows && j >= 0 && j < img.cols) {
				currRGB = img.at<Vec3b>(i, j);
				ball.at<Vec3b>(countBallPixelY, countBallPixelX) = currRGB;

				if (currRGB[0] < 70 && currRGB[1] < 70 && currRGB[2] < 70) {
					countBlackPixels++;
				}
				else if (currRGB[0] < 120 || currRGB[1] < 120 || currRGB[2] < 120) {
					countColouredPixels++;
					//img.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				}
				else {
					countWhitePixels++;
					//img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				}
			}
			countBallPixelX++;
		}
		countBallPixelX = 0;
		countBallPixelY++;
	}

	countBlackPixels /= radius ^ 2;
	countColouredPixels /= radius ^ 2;
	countWhitePixels /= radius ^ 2;

	if (countBlackPixels > 4) {
		return 2;
	}
	if (countWhitePixels > 10) {
		return 1;
		//countWhitePixels > countColouredPixels && 
	}
	Mat grey;
	cvtColor(ball, grey, COLOR_BGR2GRAY, CV_8UC1);
	threshold(grey, grey, 0, 255, THRESH_BINARY | THRESH_OTSU);
	countWhitePixels = 0;
	countBlackPixels = 0;
	//erode(grey, grey, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	/*imshow("ball", ball);
	imshow("fsfsa", grey);
	waitKey();*/

	for (int i = 0; i < grey.rows; i++) {
		for (int j = 0; j < grey.cols; j++) {
			if (((i - grey.rows / 2) ^ 2 + (j - grey.rows / 2) ^ 2) < grey.rows/2) {
				if (grey.at<uchar>(i, j) == 255) {
					countWhitePixels++;;
				}
				else {
					countBlackPixels++;
				}
			}
		}
	}
	countBlackPixels /= radius ^ 2;
	countWhitePixels /= radius ^ 2;

	//is striped
	if (countWhitePixels > 2 && countColouredPixels < 50) {
		return 4;
	}
	return 3;
}

/**
*
 * function that recognizes the balls in the billiard field
 * using HoughCircles
 *
*/
vector<Vec3f> Balls::detectBilliardBalls(Mat greyMat) {
	vector<Vec3f> circles;
	//Mat canny;
	//Canny(greyMat, canny, 0.2, 0.1);
	//imshow("canny", canny);
	//waitKey();
	Mat result;
	HoughCircles(greyMat, circles, HOUGH_GRADIENT, 1, 10, 1, 10, 6, 21);
	return circles;
}

vector<vector<int>> Balls::drawBalls(Mat& img, vector<Vec3f> circles, vector<Point> corners, Mat blurred) {
	vector<vector<int>> data;
    for (size_t i = 0; i < circles.size(); i++) {
			vector<int> boxAndLabel;
            cv::Point center = cv::Point(circles[i][0], circles[i][1]);
            int radius = circles[i][2];
            int categoryId = getBallsClass(blurred, radius, center);
			Vec3f color;
			switch (categoryId) {
				case 1: 
					color = Vec3f(255, 255, 255);
					break;
				case 2:
					color = Vec3f(0, 0, 0);
					break;
				case 3:
					color = Vec3f(0, 0, 255);
					break;
				case 4:
					color = Vec3f(255, 0, 0);
					break;
			}
        /*    if (center.y - radius >= 0 && center.y + radius < img.rows && center.x - radius >= 0 && center.x + radius < img.cols) {
                if (center.y - radius > corners[0].y && center.x + radius > corners[0].x &&
                    center.y - radius < corners[3].y && center.x + radius < corners[3].x) {*/
                        cv::rectangle(img, Point(center.x - radius, center.y - radius), Point(center.x + radius, center.y + radius), color, 2, cv::LINE_AA);
						boxAndLabel.emplace_back(center.x - radius);
						boxAndLabel.emplace_back(center.y - radius);
						boxAndLabel.emplace_back(2 * radius);
						boxAndLabel.emplace_back(2 * radius);
						boxAndLabel.emplace_back(categoryId);
   /*             }
            }*/
			data.emplace_back(boxAndLabel);
        }
	return data;
}