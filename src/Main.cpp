#include "FrameProcessing.h"
#include "Testing.h"
#include "Segmentation.h"
using namespace cv;
using namespace std;
//PATRIZIA STEFANI


int main(int argc, char* argv[])
{
    string filename = "videos/game1_clip1.mp4";
    if (argc > 0) filename = argv[1];

    //TESTING PART
    string path = "test/";
    string firstFramePath = "frames/frame_first.png";
    string lastFramePath = "frames/frame_first.png";
    string filenametest= "";
    string firstBboxPath = "bounding_boxes/frame_first_bbox.txt";
    string lastBboxPath = "bounding_boxes/frame_last_bbox.txt";
    string firstSegMask = "masks/frame_first.png";
    string lastSegMask = "masks/frame_last.png";
    vector<vector<int>> outputData, testData;
    Mat outputSegMask, testSegMask, mask3d, mask2d;
    Mat testImg = Mat::zeros(Size(5, 5), CV_8UC3);
    for (int game = 1; game <= 4 ; game++) {
        for (int clip = 1; clip <= 4; clip++) {
            //PROCESSING PART
            if (empty(testImg) || (game > 1 && clip > 2)) break;

            filenametest = "game" + to_string(game) + "_clip" + to_string(clip);
            //TO READ VIDEO
            VideoCapture capture("test/" + filenametest + "/" + filenametest + ".mp4");
            if (!capture.isOpened())
                throw "Error in opening video";
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
            filenametest = path + filenametest + "/";
            //TEST
            //FRAME FIRST
            //testImg = imread(filenametest + firstFramePath);
            //testData = Testing::importTestData(filenametest + firstBboxPath);
            //tie(outputData, outputSegMask, mask3d, mask2d) = FrameProcessing::processFrame(testImg);
            //Testing::computeMeanAvaragePrecision(outputData, testData);
            //testSegMask = imread(filenametest + firstSegMask);
            /*Testing::computeMeanIntersectionOverUnion(testImg, outputSegMask);*/

            ////FRAME LAST
            //cout << endl << filenametest << " last" << endl;
            //testData = Testing::importTestData(filenametest + lastBboxPath);
            //testImg = imread(filenametest + lastFramePath);
            //tie(outputData, outputSegMask, mask3d, mask2d) = FrameProcessing::processFrame(testImg);
            //Testing::computeMeanAvaragePrecision(outputData, testData);
            //testSegMask = imread(filenametest + lastSegMask);
            //Testing::computeMeanIntersectionOverUnion(outputSegMask, outputSegMask);
        }
    }
    return 0;
}