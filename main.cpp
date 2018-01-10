#include "SnesController.h"
#include <X11/Xutil.h>
#include <fstream>

int main(){

    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }

    Display* display;
    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");
    Window winRoot, winFocus;

    winRoot = XDefaultRootWindow(display);
    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    XImage eyeTest = *XGetImage(display,winFocus,0,0,512,448,XAllPlanes(),ZPixmap);

    unsigned long pixels[512*448];

    int k;

    for(int test = 0;test<100;test++){

        k = 0;

        for(int i=0;i<200;i++) for(int j=0;j<400;j++){

            pixels[k] = XGetPixel( &eyeTest,i,j );
            k++;

        }

    }

    //for(int i=0;i<k;i++) std::cout << pixels[i] << std::endl;

    std::ofstream printTest("file.dat");

    k = 0;

    for(int i=0;i<400;i++) for(int j=0;j<200;j++){

        printTest << i << "\t" << -j << "\t" << pixels[k] << std::endl;

        k++;

    }

    printTest.close();

    int windowWidth, windowHeight;

    XWindowAttributes attrib;

    XGetWindowAttributes(display,winFocus,&attrib);

    std::cout << attrib.height << "\t" << attrib.width << std::endl;

//    SnesController controller;
//
//    controller.pressUP();
//
//    sleep(2);
//
//    controller.releaseUP();
//
//    controller.pressDOWN();
//
//    sleep(1);
//
//    controller.releaseDOWN();
//
//    controller.pressA();
//
//    controller.pressRIGHT();
//
//    controller.pressY();
//
//    sleep(6);
//
//    for(int i=0;i<15;i++){
//
//        controller.releaseA();
//        sleep(1);
//        controller.pressA();
//        sleep(1);
//
//    }
//
//    controller.releaseA();
//
//    controller.releaseRIGHT();
//
//    controller.releaseY();

    return 0;

}
