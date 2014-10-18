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
    //threshold( gray, gray, 40, 255, 0);
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
    imshow("sum",sum);
    waitKey(0);
}

return 0;
}