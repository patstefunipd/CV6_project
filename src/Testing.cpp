#include "Testing.h"
//PATRIZIA STEFANI

vector<vector<int>> Testing::importTestData(string filename) {
	// Create a text string, which is used to output the text file
	vector<vector<int>> data;
	string stringData = "";

	// Read from the text file
	ifstream testData(filename);

	while (getline(testData, stringData)) {
		stringstream ss(stringData);
		string word;
		vector<int> singleLine;
		while (getline(ss, word, ' ')) {
			singleLine.emplace_back(stoi(word));
		}
		data.emplace_back(singleLine);
		stringData = "";
	}
	//for (int i = 0; i < data.size(); i++) {
	//	for (int j = 0; j < data[i].size(); j++) {
	//		cout << data[i][j] << endl;
	//	}
	//}
	// Close the file
	testData.close();
	return data;
}

vector<tuple<Point, float>> ioU(vector<vector<int>> dataset1, vector<vector<int>> dataset2) {
	vector<tuple<Point, float>> iouData;
	Point rectPoint;
	float value;
	bool matchFound = false;
	for (int balls = 0; balls < dataset1.size(); balls++) {
			for (int testBalls = 0; testBalls < dataset2.size(); testBalls++) {
				//rectangle(img, Rect(Point(dataset2[testBalls][0], dataset2[testBalls][1]), Point(dataset2[testBalls][0] + dataset2[testBalls][2], dataset2[testBalls][1] + dataset2[testBalls][3])), Scalar(0,0,255));
				int x1 = max(dataset1[balls][0], dataset2[testBalls][0]);
				int y1 = max(dataset1[balls][1], dataset2[testBalls][1]);
				int x2 = min(dataset1[balls][0] + dataset1[balls][2], dataset2[testBalls][0] + dataset2[testBalls][2]);
				int y2 = min(dataset1[balls][1] + dataset1[balls][3], dataset2[testBalls][1] + dataset2[testBalls][3]);
				//cout << "x1 " << x1 << " y1 " << y1 << " x2 " << x2 << " y2 " << y2 << endl;
				int area1 = dataset1[balls][2] * dataset1[balls][3];
				int area2 = dataset2[testBalls][2] * dataset2[testBalls][3];

				int intersWidth = max(0, x2 - x1);
				int intersHeight = max(0, y2 - y1);
				int intersArea = intersWidth * intersHeight;
				//cout << "Inters area " << intersArea << endl;


				int unionArea = area1 + area2 - intersArea;
				float iou = (float)intersArea / (float)unionArea;

				for (int computedIou = 0; computedIou < iouData.size(); computedIou++) {
					tie(rectPoint, value) = iouData[computedIou];
					if (rectPoint.x == dataset1[balls][0] && rectPoint.y == dataset1[balls][1]) {
						if (iou >= value && iou > 0.00) {
							iouData.erase(iouData.begin() + computedIou);
							iouData.emplace_back(make_tuple(rectPoint, iou));
							matchFound = true;
						}
						else if (iou == 0.00) {
							matchFound = true;
						}
					}
					if (matchFound) break;
				}
				if (!matchFound) {
					iouData.emplace_back(make_tuple(Point(dataset1[balls][0], dataset1[balls][1]), iou));
				}
				matchFound = false;
			}
	}
	return iouData;
}

void Testing::computeMeanAvaragePrecision(vector<vector<int>> dataset1, vector<vector<int>> dataset2) {
	vector<tuple<Point, float>> iouData = ioU(dataset1, dataset2);
	Point rectPoint;
	float value;
	int countFalseNeg = 0;
	int countFalsePos = 0;
	int countTruePos = 0;

	for (int i = 0; i < iouData.size(); i++) {
		tie(rectPoint, value) = iouData[i];
		if (value == 0.00) {
			countFalseNeg++;
			cout << "Point: " << rectPoint << " false negative " << value << endl;
		}
		else if (value < 0.5) {
			countFalsePos++;
			cout << "Point: " << rectPoint << " false positive " << value << endl;
		}
		else {
			countTruePos++;
			cout << "Point: " << rectPoint << " true positive " << value << endl;
		}
	}
	float precision = (float)countTruePos/(float)(countTruePos + countFalsePos);
	float recall = (float)countTruePos/(float)(countTruePos + countFalseNeg);
	cout << "Precision: " << precision << endl;
	cout << "Recall: " << recall << endl;
}

void Testing::computeMeanIntersectionOverUnion(Mat mask1, Mat mask2) {
	vector<int>intersArea = {0, 0, 0, 0, 0, 0};
	vector<int>unionArea = { 0, 0, 0, 0, 0, 0 };
	vector<float> iou;
	Mat img = Mat::zeros(Size(mask1.cols, mask1.rows), CV_8UC3);

	for (int x = 0; x < mask1.cols; x++) {
		for (int y = 0; y < mask1.rows; y++) {
		/*	cout << "MASK 1 " << y << " " << x << " class " << to_string(mask1.at<uchar>(y, x)) << endl;
			cout << "MASK 2 " << y << " " << x << " class " << to_string(mask2.at<uchar>(y, x)) << endl;*/
			if (mask1.at<uchar>(y, x) != 0 && mask1.at<uchar>(y, x) != 5) {
				img.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
			}
			if (mask2.at<uchar>(y, x) != 0 && mask2.at<uchar>(y, x) != 5) {
				img.at<Vec3b>(y, x) = Vec3b(255, 0, 0);
			}

			if (mask2.at<uchar>(y, x) != 5 && mask2.at<uchar>(y, x) != 0) {
			/*	cout << "MASK 1 BALL " << x << " " << y << " class " << to_string(mask1.at<uchar>(y, x)) << endl;
				cout << "MASK 2 BALL " << x << " " << y << " class " << to_string(mask2.at<uchar>(y, x)) << endl << endl;*/

			}
			if (to_string(mask1.at<uchar>(y, x)) == to_string(mask2.at<uchar>(y, x))) { 
				intersArea[mask1.at<uchar>(y, x)]++;
			}
			unionArea[mask1.at<uchar>(y, x)]++;
			unionArea[mask2.at<uchar>(y, x)]++;

		}
	}
	imshow("Img", img);
	waitKey();
	float iouResult;
	for (int i = 0; i < intersArea.size(); i++) {
	/*	cout << "inters " << i << " : " << intersArea[i] << endl;
		cout << "union " << i << " : " << unionArea[i] << endl;*/
		iouResult = (float)intersArea[i] / (float)unionArea[i];
		cout << iouResult << endl;
		iou.push_back(iouResult);
	}
}