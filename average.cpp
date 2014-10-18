#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <string>
#include <math.h>

using namespace cv;
using namespace std;

char name[50];
int i = 2;
Mat gray;
std::vector<Mat> images;
std::vector<KeyPoint> points;

int main(){
    while(1){  
        sprintf(name,"./png_images/%04d.png",i);
        Mat src= imread(name,1);
        if(!src.data ) break;
        cvtColor(src,gray,CV_BGR2GRAY);
        Mat newsrc = src(Rect(92,48,640,320));
        threshold( newsrc, newsrc, 50, 255, 0);
        images.push_back(newsrc);
        i+=1;
    }

    /*
    AVERAGE ALL THE IMAGES
    adding weighted:
    1 + 1/2 + 1/3 + 1/4 + ... + 1/n
    */
    Mat sum = images[0];
    for (float k = 1; k < images.size(); k++){
        float factor = k/(k+1.0);
        addWeighted( sum, factor, images[k], 1.0-factor, 0.0, sum);
    }

    imwrite("results/average.png", sum);
    threshold( sum, sum, 80, 255, 0);
    imwrite("results/average_threshold.png", sum);
    imshow("sum",sum);
    waitKey(0);
    Mat out = sum;
    vector<KeyPoint> keyPoints;
    SimpleBlobDetector::Params params;
        params.minThreshold = 40;
        params.maxThreshold = 60;
        params.thresholdStep = 5;
        params.minArea = 5; 
        params.minConvexity = 0.3;
        params.minInertiaRatio = 0.01;
        params.minDistBetweenBlobs = 5;
        params.maxArea = 8000;
        params.maxConvexity = 10;
        params.filterByCircularity = false;
        params.filterByColor = false;
    namedWindow( "Out", CV_GUI_NORMAL );
    line( sum, Point(0, sum.rows-1), Point( sum.cols-1, sum.rows-1 ), Scalar::all(255) );

    SimpleBlobDetector blobDetector( params );
    blobDetector.create("SimpleBlob");
    blobDetector.detect( sum, keyPoints);
                //blobDetector.detectEx( src, keyPoints, contours );
    drawKeypoints( sum, keyPoints, out, CV_RGB(0,keyPoints.size(),0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                //approxContours.resize( contours.size() );
                /*
                cout << "{";
                cout << keyPoints[0].pt;
                for (i = 1; i < keyPoints.size(); i++){
                    cout << ", " << keyPoints[i].pt;
                }
                cout << "}";
                */
    points = keyPoints;
    imwrite("results/average_keypoints.png", out);
    imshow( "Out", out);
    waitKey(0);
    return 0;
}