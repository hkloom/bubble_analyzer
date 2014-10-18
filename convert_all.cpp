#include <stdio.h>
#include <Magick++.h>

char name[50];
int i = 1;

int main(){
while(1){  

    sprintf(name,"./images/20140822_144213_C1S0001%04d.tif",i);
    Magick::Image mimg;
    mimg.read(name);
    mimg.magick("png");
    sprintf(name,"./png_images/%04d.png",i);
    mimg.write(name);

    i++;
    if (i%20) printf("%04d",i);
}
return 0;
}