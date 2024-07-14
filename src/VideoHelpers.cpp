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
void DrawItems(vector<T> items, cv::Mat &img, int itemType) {
    for (size_t i = 0; i < items.size(); i++) {
        switch (itemType) {
            case 0:
            {
                float rho = items[i][0], theta = items[i][1];
                cv::Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a * rho, y0 = b * rho;
                pt1.x = cvRound(x0 + 1000 * (-b));
                pt1.y = cvRound(y0 + 1000 * (a));
                pt2.x = cvRound(x0 - 1000 * (-b));
                pt2.y = cvRound(y0 - 1000 * (a));
                cv::line(img, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
            }
            break;
            case 1:
            {
                cv::Point center = cv::Point(items[i][0], items[i][1]);
                int radius = items[i][2];
                Vec3b color = ObjectRecognition::getBallsClass(img, radius, center);
                if(center.y - radius >= 0 && center.y + radius < img.rows && center.x - radius >= 0 && center.x + radius < img.cols)
                    cv::circle(img, center, radius, color, 3, cv::LINE_AA);
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
void VideoHelpers::showVideo(std::string filename, cv::VideoCapture cap, cv::Mat frame) {
    cv::namedWindow(filename, 1);
    cv::Mat greyMat, table;
    cv::Mat elaboratedFrame;
    std::vector<cv::Vec2f> lines;
    std::vector<cv::Vec3f> circles;

    for (;;) {
        cap >> frame;
        if (frame.empty())
            break;
        elaboratedFrame = frame.clone();
        cv::cvtColor(frame, greyMat, cv::COLOR_BGR2GRAY);
        medianBlur(greyMat, greyMat, 3);
        // DrawItems(ObjectRecognition::detectPlayingField(greyMat), elaboratedFrame, 0);
         DrawItems(ObjectRecognition::detectBilliardBalls(greyMat), elaboratedFrame, 1);
         imshow("Image", elaboratedFrame);
         waitKey();
        
        // Ensure table is initialized and has the same type and number of rows as elaboratedFrame
        if (table.empty()) {
            table = cv::Mat::zeros(elaboratedFrame.size(), elaboratedFrame.type());
        }

        if (table.rows != elaboratedFrame.rows) {
            cv::resize(table, table, cv::Size(table.cols, elaboratedFrame.rows));
        }

        if (table.type() != elaboratedFrame.type()) {
            cv::cvtColor(table, table, cv::COLOR_BGR2GRAY);
            cv::cvtColor(table, table, cv::COLOR_GRAY2BGR); 
        }


        //--------------------------------
        vector<Point> corners;
        Mat output;
        tie(corners, output) = Table2D::detectBilliardTable(elaboratedFrame);
        

        // Mat combinedFrame;
        table = Table2D::creatTable(elaboratedFrame, corners);
        // cv::hconcat(elaboratedFrame, table, combinedFrame);

        imshow("Detected Billiard Table", table);
        //--------------------------------

        // Concatenate table image and frame side by side
        // cv::Mat combinedFrame;
        // cv::hconcat(elaboratedFrame, table, combinedFrame);

        // cv::imshow(filename, combinedFrame);

        if (cv::waitKey(20) >= 0)
            break;  
    }
}
