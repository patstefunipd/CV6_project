#include "Testing.h"

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

void Testing::computeMeanAvaragePrecision(vector<vector<int>> dataset1, vector<vector<int>> dataset2, Mat img) {
	vector<tuple<Point, bool>> iouData;
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

			int intersWidth  = max(0, x2 - x1);
			int intersHeight = max(0, y2 - y1);
			int intersArea = intersWidth * intersHeight;
			//cout << "Inters area " << intersArea << endl;


			int unionArea = area1 + area2 - intersArea;
			float iou = (float)intersArea / (float) unionArea;

			if (iou > 0.00) {
				if (iouData.size() == 0) {
					iouData.emplace_back(make_tuple(Point(x1, y1), iou > 0.5));
				}
				for (int computedIou = 0; computedIou < iouData.size(); computedIou++) {
					tie(rectPoint, value) = iouData[computedIou];
					if (rectPoint.x == dataset1[balls][0] && rectPoint.y == dataset1[balls][1]) {
						if (iou > value) {
							iouData[computedIou] = make_tuple(rectPoint, iou > 0.5);
							matchFound = true;
						}
					}
				}
				if (!matchFound) {
					iouData.emplace_back(make_tuple(Point(dataset1[balls][0], dataset1[balls][1]), iou > 0.5));
				}
				matchFound = false;
			}
		}
	}

	//imshow("gfas", img);
	//waitKey();
	for (int i = 0; i < iouData.size(); i++) {
		tie(rectPoint, value) = iouData[i];
		cout << "Point: " << rectPoint << " is > 0.5: " << value << endl;
	}
}