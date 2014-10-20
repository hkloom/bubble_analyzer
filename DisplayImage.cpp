#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat mutated;
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
  /// Load source image and convert it to gray
  while(1){  
        sprintf(name,"./png_images/%04d.png",i);
  src = imread( name, 1 );

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
  erode( src_gray, src_gray, element );
  dilate( src_gray, src_gray, element2 );
  dilate( src_gray, src_gray, element );
  erode( src_gray, src_gray, element2 );

  imshow("dilated",src_gray);
  //waitKey(0);

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
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
       Scalar color = Scalar( 255, 255, 0 );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
      // cout << "area:  " << contourArea(contours[i]) << endl;
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
