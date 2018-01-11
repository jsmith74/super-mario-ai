#include "Eyes.h"

#define MARIO_HAT_RED 16269424

int Eyes::idx(int& x,int& y){

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

    pixels.resize( screenWidth * screenHeight );

    marioWidth = 26;
    marioHeight = 39;

}

void Eyes::printScreenSection(int xStart,int xEnd,int yStart, int yEnd){

    std::ofstream outfile("screenShot.dat");

    for(int y=yStart;y<yEnd;y++) for(int x=xStart;x<xEnd;x++) outfile << x << "\t" << y << "\t" << pixels[ idx(x,y) ] << std::endl;

    outfile.close();

    return;

}

bool Eyes::isMario(int X,int Y){

    int matchCount = 0;

    for(int y=Y;y<Y+marioHeight;y++) for(int x=X;x<X+marioWidth;x++){

        if(pixels[ idx(x,y) ] == 2109576 ) matchCount++;
        if(pixels[ idx(x,y) ] == 4227224 ) matchCount++;
        if(pixels[ idx(x,y) ] == 5242880 ) matchCount++;
        if(pixels[ idx(x,y) ] == 8444104 ) matchCount++;
        if(pixels[ idx(x,y) ] == 8935448 ) matchCount++;
        if(pixels[ idx(x,y) ] == 11544672 ) matchCount++;
        if(pixels[ idx(x,y) ] == 16269424 ) matchCount++;
        if(pixels[ idx(x,y) ] == 16281704 ) matchCount++;
        if(pixels[ idx(x,y) ] == 16306368 ) matchCount++;
        if(pixels[ idx(x,y) ] == 16308336 ) matchCount++;

    }

    if(matchCount >= 500) return true;
    else return false;

}

bool Eyes::localMarioSearch(int X,int Y){

    for(int y=Y-marioHeight;y<Y;y++) for(int x=X-marioWidth;x<X;x++){

        if( isMario(x,y) ){

            marioPosition[0] = x;
            marioPosition[1] = y;
            printScreenSection(x,x+marioWidth,y,y+marioHeight);

            return true;

        }

    }

    return false;

}

void Eyes::findMario(){

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++){

        if( pixels[ idx(x,y) ] == MARIO_HAT_RED ){

            if( localMarioSearch(x,y) ) return;

        }

    }

    assert( false && "Err. Mario Not Found");

    return;

}

void Eyes::lookScreen(){

    screenImage = *XGetImage(display,winFocus,0,0,screenWidth,screenHeight,XAllPlanes(),ZPixmap);

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) pixels[ idx(x,y) ] = XGetPixel(&screenImage,x,screenHeight - y - 1);

    return;

}


void Eyes::printLastSeen(){

    std::ofstream outfile("screenShot.dat");

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) outfile << x << "\t" << y << "\t" << pixels[ idx(x,y) ] << std::endl;

    outfile.close();

    return;

}

Eyes::~Eyes(){

    XCloseDisplay(display);

}
