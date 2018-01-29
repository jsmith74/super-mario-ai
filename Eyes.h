#ifndef EYES_H_INCLUDED
#define EYES_H_INCLUDED

#include <unsupported/Eigen/CXX11/Tensor>
#include <X11/Xutil.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>

class Eyes{

    public:
        Eyes();
        ~Eyes();

        void lookScreen();
        void printLastSeen();
        void printScreenSection(int xStart,int xEnd,int yStart,int yEnd);
        void printLastSeen(std::ofstream& outfile);

    private:
        Display* display;
        Window winFocus;
        Window winRoot;
        XImage screenImage;

        Eigen::VectorXd pixels;

        int idx(int& x,int& y);
        int idxT(int x,int y);

        int screenWidth,screenHeight;
        int strideWidth, strideHeight;

};

#endif // EYES_H_INCLUDED
