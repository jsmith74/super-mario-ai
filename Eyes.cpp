#include "Eyes.h"

#define MARIO_HAT_RED 16269424

int Eyes::idx(int& x,int& y){

    return x + y * screenWidth;

}

int Eyes::idxT(int x,int y){

    return x + y * screenWidth;

}

Eyes::Eyes(){

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");
    winRoot = XDefaultRootWindow(display);
    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    XWindowAttributes attrib;

    XGetWindowAttributes(display,winFocus,&attrib);

    screenWidth = attrib.width;
    screenHeight = attrib.height;

    std::cout << "Input size: " << screenWidth * screenHeight << std::endl;

    pixels.resize( screenWidth * screenHeight );

}

void Eyes::printScreenSection(int xStart,int xEnd,int yStart, int yEnd){

    std::ofstream outfile("screenShot.dat");

    for(int y=yStart;y<yEnd;y++) for(int x=xStart;x<xEnd;x++) outfile << x << "\t" << y << "\t" << pixels[ idx(x,y) ] << std::endl;

    outfile.close();

    return;

}



void Eyes::lookScreen(){

    screenImage = *XGetImage(display,winFocus,0,0,screenWidth,screenHeight,XAllPlanes(),ZPixmap);

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) pixels( idx(x,y) ) = (XGetPixel(&screenImage,x,screenHeight - y - 1));

    //pixels = pixels.array() - pixels.mean();

    //double stdDev = sqrt( (pixels.array() * pixels.array() ).sum() / ( 1.0*pixels.size() - 1.0 ) );

    //pixels /= stdDev;

    return;

}


void Eyes::printLastSeen(){

    std::ofstream outfile("screenShot.dat");

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) outfile << x << "\t" << y << "\t" << pixels( idx(x,y) ) << std::endl;

    outfile.close();

    return;

}

void Eyes::printLastSeen(std::ofstream& outfile){

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) outfile << pixels( idx(x,y) ) << "\t";

    return;

}

Eyes::~Eyes(){

    XCloseDisplay(display);

}
