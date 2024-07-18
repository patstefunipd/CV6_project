#include "VideoHelpers.h"
#include "FrameProcessing.h"
#include <opencv2/video/tracking.hpp>
#include "BallTracker.h"


/*
* 
* Function that, given video filename, a videocapture and a mat frame obj, shows the video read
* 
*/
void VideoHelpers::showVideo(string filename) {
    VideoCapture capture(filename);
    if (!capture.isOpened())
        throw "Error in opening video";
    Ptr<DISOpticalFlow> disOpticalFlow = DISOpticalFlow::create();
    Mat prevFrame, flow, mask3d, frame, elaboratedFrame, proj2d;
    int countFrame = 0;
    cv::Mat segmentMask;
    vector<vector<int>> balls;
   
    for (;;) {
        capture >> frame;
        if (frame.empty())
            break;
        elaboratedFrame = frame.clone();
        tie(balls, segmentMask, mask3d, proj2d) = FrameProcessing::processFrame(elaboratedFrame);
        if (!prevFrame.empty()) {
            BallTracker::trackBall(prevFrame, mask3d, flow, disOpticalFlow, elaboratedFrame, balls);
        }
        prevFrame = mask3d.clone();
        imshow("Test", elaboratedFrame);
        waitKey(20);
        countFrame++;
    }
}
