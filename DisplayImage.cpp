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
int i = 1;
int max_thresh = 255;
RNG rng(12345);



//gets the square of the distance between two points
float euclideanDistSqrd(Point2f& p, Point2f& q) {
    Point2f diff = p - q;
    return diff.x*diff.x + diff.y*diff.y;
}

//callback function for slider
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


  //normalize?

    blur( src_gray, src_gray, Size(3,3) );
    threshold( src_gray, src_gray, 50, 255, CV_ADAPTIVE_THRESH_MEAN_C);

  /// Create Window
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src);
  //waitKey(0);


    Mat element = getStructuringElement( MORPH_ELLIPSE,
     Size( 5, 5 ),
     Point( 2, 2 ) );
    Mat element2 = getStructuringElement( MORPH_ELLIPSE,
     Size( 4, 4 ),
     Point( 2, 2 ) );

  /// Apply the dilation and erosion operation
  //to get rid of holes and other noise
  //erode( src_gray, src_gray, element );
  //dilate( src_gray, src_gray, element2 );
    dilate( src_gray, src_gray, element2 );
    erode( src_gray, src_gray, element2 );
    imshow("dilated",src_gray);
    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );
    waitKey(10);
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

    SimpleBlobDetector blobDetector( params );
    blobDetector.detect( src_gray, keyPoints);

    /// Get the moments
    vector<Moments> mu(contours.size() ); 
    for( int i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); } 

    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    if (i==1){
      cout << "frame";
    for (int k = 0; k < keyPoints2.size(); k++){
      if (!(keyPoints2[k].pt.x < 20 || keyPoints2[k].pt.x > 620 || keyPoints2[k].pt.y < 20 || keyPoints2[k].pt.y > 300)){
        cout << ",divet" << k;
      }
    }
    cout << endl;
    } 
    cout << i;


    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
       { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
         minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
       }


    /// Draw polygonal contour + bonding rects + circles
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( 255, 255, 255 );
         //drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
         circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       }






    //dumb and slow O(n^2) algorithm
    //need to replace with quadtree, kdtree or something like that
    for (int k = 0; k < keyPoints2.size(); k++){
      if (!(keyPoints2[k].pt.x < 20 || keyPoints2[k].pt.x > 620 || keyPoints2[k].pt.y < 20 || keyPoints2[k].pt.y > 300)){
        cout<<",";
            for (int j = 0; j<mc.size();j++){
              //cout << ",";
            float d = euclideanDistSqrd(keyPoints2[k].pt, mc[j]);
            //int area = contourArea(contours[j]);
            //cout << "radius = " << radius[j] << endl;
            float area = radius[j]*radius[j]*3.1415;
            if (radius[j] > 16 || radius[j] <= 1){
                drawContours( drawing, contours, j,  Scalar( 0, 0, 255 ), CV_FILLED, 8, hierarchy, 0, Point() );
            }else if (d > 50){
                drawContours( drawing, contours, j,  Scalar( 255, 100, 0 ), CV_FILLED, 8, hierarchy, 0, Point() );
            }
            else if (area < 800 && d <= 50){
              //if (k==30) cout << "point: " << k << " --- " << area << endl;
              cout <<area;
              drawContours( src, contours, j,  Scalar( 255, 255, 255 ), CV_FILLED, 8, hierarchy, 0, Point() );
              circle( src, center[j], (int)radius[j], Scalar( 0, 255, 255 ), 2, 8, 0 );
              break;
            }
            //break;
        }
      }
    }
  cout << endl;

  namedWindow( "Process", CV_WINDOW_AUTOSIZE );
  imshow( "Process", drawing );
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", src );
}
