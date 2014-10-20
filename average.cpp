#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <string>
#include <math.h>
#include "cvblob.h"

using namespace cv;
using namespace std;
using namespace cvb;

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
        threshold( newsrc, newsrc, 50, 255, CV_ADAPTIVE_THRESH_MEAN_C);
        //IplImage *gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
        //cvCvtColor(newsrc, gray, CV_BGR2GRAY);        
        //IplImage *labelImg=cvCreateImage(cvGetSize(src), IPL_DEPTH_LABEL, 1);
        //CvBlobs blobs;
        //unsigned int result=cvLabel(gray, labelImg, blobs);
        //cvRenderBlobs(labelImg, blobs, gray, gray);
        //imshow("blobs",labelImg);
        //cvCvtColor(img, gray, CV_BGR2GRAY);
        //cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY);

         vector< vector<Point> > contours; // Vector for storing contour
         vector<Vec4i> hierarchy;
         //findContours( gray, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
         Mat marked;


         for( int i = 0; i< contours.size(); i++ ) {// iterate through each contour. 
            double a=contourArea( contours[i],false);  //  Find the area of contour
           // drawContours( marked, contours, i, Scalar(255), CV_FILLED, 8, hierarchy );
        }

        //imshow("contours",marked);


        images.push_back(newsrc);
        i+=1;

        //imshow("new",newsrc);
        //waitKey(0);
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
    //imshow("sum",sum);
    //waitKey(0);
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
    //line( sum, Point(0, sum.rows-1), Point( sum.cols-1, sum.rows-1 ), Scalar::all(255) );

    SimpleBlobDetector blobDetector( params );
    //blobDetector.create("SimpleBlob");
    blobDetector.detect( sum, keyPoints);
    cout << "{";
    cout << keyPoints[0].pt;
    for (i = 1; i < keyPoints.size(); i++){
        cout << ", " << keyPoints[i].pt;
    }
    cout << "}";
    drawKeypoints( sum, keyPoints, out, CV_RGB(0,keyPoints.size(),0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                //approxContours.resize( contours.size() );
               
    points = keyPoints;
    imwrite("results/average_keypoints.png", out);

    cv::FileStorage store("template.bin", cv::FileStorage::WRITE);
    cv::write(store,"keypoints",keyPoints);
    store.release();


    //vector < vector <Point2f> > contour;
    //vector<Vec4i> hierarchy;
    //findContours( sum, contour, CV_RETR_TREE, CV_CHAIN_APPROX_TC89_L1);
    //cout << "{";
    //cout << contourArea(contour[0]) << endl;
    //for (i = 1; i < contour.size(); i++){
        //cout << ", " << contourArea(contour[i]) << endl;
        //drawContours( out, contour, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point() );
    //}
    //cout << "}";

    imshow( "Out", out);
    waitKey(0);
    return 0;
}