#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat mutated;
vector<KeyPoint> keyPoints2;
int thresh = 100;
char name[50];
int i = 2;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  cv::FileStorage store("template.bin", cv::FileStorage::READ);
  cv::FileNode n1 = store["keypoints"];
  cv::read(n1,keyPoints2);
  /// Load source image and convert it to gray
  while(1){  
        sprintf(name,"./png_images/%04d.png",i);
  src = imread( name, 1 );
  if(!src.data ) break;
  src = src(Rect(92,48,640,320));

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  threshold( src_gray, src_gray, 50, 255, CV_ADAPTIVE_THRESH_MEAN_C);

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src_gray);
  //waitKey(0);


  Mat element = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 5, 5 ),
                                       Point( 2, 2 ) );
  Mat element2 = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 4, 4 ),
                                       Point( 2, 2 ) );
  /// Apply the dilation operation
  //erode( src_gray, src_gray, element );
  //dilate( src_gray, src_gray, element2 );
  dilate( src_gray, src_gray, element2 );
  erode( src_gray, src_gray, element2 );

  imshow("dilated",src_gray);
  //waitKey(0);

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );
  //cout << i << endl;
  waitKey(5);
  i++;
}

  return(0);
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar purple = Scalar( 255, 100, 80 );
        Scalar red = Scalar( 100, 100, 255 );
       if (contourArea(contours[i]) > 600){
            //cout << "area:  " << contourArea(contours[i]) << endl;
            drawContours( drawing, contours, i, red, CV_FILLED, 8, hierarchy, 0, Point() );
          }else{
            drawContours( drawing, contours, i, purple, CV_FILLED, 8, hierarchy, 0, Point() );
          }
     }


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
      //line( sum, Point(0, sum.rows-1), Point( sum.cols-1, sum.rows-1 ), Scalar::all(255) );

    SimpleBlobDetector blobDetector( params );
    //blobDetector.create("SimpleBlob");
    blobDetector.detect( src_gray, keyPoints);
    /*cout << "{";
    cout << keyPoints[0].pt;
    for (i = 1; i < keyPoints.size(); i++){
        cout << ", " << keyPoints[i].pt;
    }
    cout << "}"; */
    drawKeypoints( drawing, keyPoints, drawing, CV_RGB(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //KDTree kdtree = KDTree(keyPoints);
    //vector <Point> ns3;
    //kdtree.findNearest(keyPoints2,2,5, ns3);

    /*
  */




    drawKeypoints( drawing, keyPoints2, drawing, CV_RGB(0,255,0), 1);

                //approxContours.resize( contours.size() );




  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
