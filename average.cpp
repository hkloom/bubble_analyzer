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

int main(){
while(1){  

    sprintf(name,"./png_images/%04d.png",i);
    Mat src= imread(name,1);


    if(!src.data ) break;

    cvtColor(src,gray,CV_BGR2GRAY);
    //sprintf(name,"gray%4d.jpg",i);
    Mat newsrc = src(Rect(92,48,640,320));
    //imwrite(name, gray);

    //namedWindow("src");
    //imshow("src",src);
    threshold( newsrc, newsrc, 50, 255, 0);
    images.push_back(newsrc);
    //imshow("result",gray);

    i+=1;
    //waitKey(0);
}


Mat sum = images[0];
for (float k = 1; k < images.size(); k++){
    float K = log2(k);
    float factor = k/(k+1.0);
    addWeighted( sum, factor, images[k], 1.0-factor, 0.0, sum);
    printf("%4f, %f\n",k,factor);
   
}

threshold( sum, sum, 80, 255, 0);
 imshow("sum",sum);
    waitKey(0);

    const char      * wndName = "Source image",
                                * wndNameGray = "Gray img", 
                                * wndNameOut = "Out",
                                * filename = "../data/images/eyebrows-smoothed.bmp";

        Mat src, gray, thresh, binary;
        Mat out = sum;
        vector<KeyPoint> keyPoints;
        vector< vector <Point> > contours;
        vector< vector <Point> > approxContours;

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
        namedWindow( wndNameOut, CV_GUI_NORMAL );

        src = imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
        line( sum, Point(0, sum.rows-1), Point( sum.cols-1, sum.rows-1 ), Scalar::all(255) );

        SimpleBlobDetector blobDetector( params );
        blobDetector.create("SimpleBlob");

        for(;;)
        {
                blobDetector.detect( sum, keyPoints);
                //blobDetector.detectEx( src, keyPoints, contours );
                drawKeypoints( sum, keyPoints, out, CV_RGB(0,keyPoints.size(),0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                //approxContours.resize( contours.size() );


                cout << "Keypoints " << keyPoints.size() << endl;
        
                imshow( wndNameOut, out );
                waitKey(0);
        }



return 0;
}