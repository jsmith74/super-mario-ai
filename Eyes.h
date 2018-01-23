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
        void findMario();
        void printScreenSection(int xStart,int xEnd,int yStart,int yEnd);
        int marioPosition[2];

    private:
        Display* display;
        Window winFocus;
        Window winRoot;
        XImage screenImage;

        std::vector<unsigned long> pixels;

        int idx(int& x,int& y);

        int screenWidth,screenHeight;

        bool localMarioSearch(int X,int Y);
        bool isMario(int X,int Y);

        int marioWidth,marioHeight;


};

#endif // EYES_H_INCLUDED
