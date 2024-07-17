#include "Segmentation.h"

Mat Segmentation::segmentImg(Mat img, vector<vector<int>> ballClasses) {
	Mat mask = Mat::zeros(Size(img.cols, img.rows), CV_8U);
	bool isBall = false;
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			for (int ballInd = 0; ballInd < ballClasses.size(); ballInd++) {
				if (x >= ballClasses[ballInd][0] && x <= ballClasses[ballInd][0] + ballClasses[ballInd][2] &&
					y >= ballClasses[ballInd][1] && y <= ballClasses[ballInd][1] + ballClasses[ballInd][3]) {
					 mask.at<uchar>(y,x) = ballClasses[ballInd][4];
					 isBall = true;
					 break;
				}
				if (isBall) break;
			}
			if (!isBall) {
				bool isForeground = img.at<uchar>(y,x) == 0;
				if (isForeground) mask.at<uchar>(y, x) = 5;
				else mask.at<uchar>(y, x) = 0;
			}
			isBall = false;
		}
	}
	return mask;
}