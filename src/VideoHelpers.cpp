#include "VideoHelpers.h"
#include "ObjectRecognition.h"
#include "Table2D.h"

/*
*  Template function that draws items (like lines and circles) in 
*  an image
*  @param itemType:
*    0 : lines
*    1 : circles
*/
template <class T>
void DrawItems(vector<T> items, Mat &img, int itemType) {
    for (size_t i = 0; i < items.size(); i++)
    {
        switch (itemType) {
            case 0:
            {
                float rho = items[i][0], theta = items[i][1];
                Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a * rho, y0 = b * rho;
                pt1.x = cvRound(x0 + 1000 * (-b));
                pt1.y = cvRound(y0 + 1000 * (a));
                pt2.x = cvRound(x0 - 1000 * (-b));
                pt2.y = cvRound(y0 - 1000 * (a));
                line(img, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
            }
                break;
            case 1:
            {
                Point center = Point(items[i][0], items[i][1]);
                int radius = items[i][2];
                circle(img, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
            }
                break;
        }
    }
}

/*
* 
* Function that, given video filename, a videocapture and a mat frame obj, shows the video read
* 
*/
void VideoHelpers::showVideo(string filename, VideoCapture cap, Mat frame) {
    namedWindow(filename, 1);
    Mat greyMat, table;
    Mat elaboratedFrame;
    vector<Vec2f> lines;
    vector<Vec3f> circles;

    // Create the table once
    table = Table2D::creatTable();

    for (;;)
    {
        cap >> frame;
        if (frame.empty())
            break;
        elaboratedFrame = frame.clone();
        cv::cvtColor(frame, greyMat, COLOR_BGR2GRAY);
        GaussianBlur(greyMat, greyMat, Size(9, 9), 0);
        DrawItems(ObjectRecognition::detectPlayingField(greyMat), elaboratedFrame, 0);
        DrawItems(ObjectRecognition::detectBilliardBalls(greyMat), elaboratedFrame, 1);

        // Ensure table has the same type and number of rows as elaboratedFrame
        if (table.rows != elaboratedFrame.rows) {
            cv::resize(table, table, Size(table.cols, elaboratedFrame.rows));
        }
        if (table.type() != elaboratedFrame.type()) {
            cv::cvtColor(table, table, COLOR_BGR2GRAY);
            cv::cvtColor(table, table, COLOR_GRAY2BGR); 
        }

        // Concatenate table image and frame side by side
        Mat combinedFrame;
        hconcat(elaboratedFrame, table, combinedFrame);

        imshow(filename, combinedFrame);

        if (waitKey(20) >= 0)
            break;  
    }
}