#include "VideoHelpers.h"
#include "FrameProcessing.h"
#include <opencv2/video/tracking.hpp>
#include "BallTracker.h"
//PATRIZIA STEFANI

/*
* 
* Function that elaborates video given filename
* 
*/
void VideoHelpers::processVideo(VideoCapture capture, int game, int clip) {
    //TO WRITE VIDEO
    VideoWriter outputVideo;

    //TO COMPUTE TRAJECTORIES
    Ptr<DISOpticalFlow> disOpticalFlow = DISOpticalFlow::create();

    Mat prevFrame, flow, mask3d, frame, elaboratedFrame, miniMap;
    int countFrame = 0;
    cv::Mat segmentMask;
    vector<vector<int>> balls;
    bool sizeChosen = false;
    for (;;) {
        capture >> frame;
        if (frame.empty())
            break;
            elaboratedFrame = frame.clone();
            tie(balls, segmentMask, mask3d, miniMap) = FrameProcessing::processFrame(elaboratedFrame);
            if (!sizeChosen) {
                std::string filename = "game" + to_string(game) + "_clip" + to_string(clip) + ".avi";
                outputVideo.open(filename, VideoWriter::fourcc('M', 'J', 'P', 'G'), capture.get(CAP_PROP_FPS), Size(miniMap.cols, miniMap.rows), true);
                if (!outputVideo.isOpened())
                {
                    cout << "Impossible to write file: " << filename << endl;
                }
                sizeChosen = true;
            }
         /*   if (!prevFrame.empty()) {
               BallTracker::trackBall(prevFrame, mask3d, flow, disOpticalFlow, elaboratedFrame, balls);
            }*/
            imshow("Minimap", miniMap);
            waitKey();
        //outputVideo.write(miniMap);
        prevFrame = mask3d.clone();
        countFrame++;
    }
    cout << "Finished writing" << endl;
}
