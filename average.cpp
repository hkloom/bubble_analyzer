#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <string>

using namespace cv;
using namespace std;

char name[50];
int i = 2;
Mat gray;

int main(){
while(1){  

    sprintf(name,"./images/20140822_144213_C1S0001%04d.tif",i);


    Magick::Image mimg;
    mimg.read(name);
    mimg.magick("png");
    mimg.write("temp.png");
    Mat src= imread("temp.png",1);

    printf("name %s\n",name);

    if(!src.data ) break;

    cvtColor(src,gray,CV_BGR2GRAY);
    //sprintf(name,"gray%4d.jpg",i);
    //imwrite(name, gray);

    //namedWindow("src");
    imshow("src",src);
    //imshow("result",gray);

    i++;
   // waitKey(0);
}

return 0;
}