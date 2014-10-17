#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <Magick++.h>

using namespace cv;

int main(int argc, char** argv ){

    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Magick::Image mimg;
    mimg.read(argv[1]);
    mimg.magick("png"); // set the "format" attribute of my_image to PNG
    mimg.write("temp.png");


    Mat image, gray;
    image = imread("temp.png", 1);
    //gray = imread("temp", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    cvtColor(image, gray, CV_BGR2GRAY);
    // smooth it, otherwise a lot of false circles may be detected
    GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
                 2, gray.rows/4, 200, 100 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( image, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( image, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
