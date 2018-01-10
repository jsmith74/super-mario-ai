#include "Eyes.h"

int Eyes::idx(int& x,int& y){

    return x + y * width;

}

Eyes::Eyes(){

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");
    winRoot = XDefaultRootWindow(display);
    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    XWindowAttributes attrib;

    XGetWindowAttributes(display,winFocus,&attrib);

    width = attrib.width;
    height = attrib.height;

    pixels.resize( width * height );

}

void Eyes::look(){

    XImage eyeImage = *XGetImage(display,winFocus,0,0,width,height,XAllPlanes(),ZPixmap);

    for(int y=0;y<height;y++) for(int x=0;x<width;x++) pixels[ idx(x,y) ] = XGetPixel(&eyeImage,x,height - y - 1);

    return;

}


void Eyes::printLastSeen(){

    std::ofstream outfile("screenShot.dat");

    for(int y=0;y<height;y++) for(int x=0;x<width;x++) outfile << x << "\t" << y << "\t" << pixels[ idx(x,y) ] << std::endl;

    outfile.close();

    return;

}

Eyes::~Eyes(){

    XCloseDisplay(display);

}
