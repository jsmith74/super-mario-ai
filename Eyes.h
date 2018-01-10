#ifndef EYES_H_INCLUDED
#define EYES_H_INCLUDED

#include <X11/Xutil.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>

class Eyes{

    public:
        Eyes();
        ~Eyes();

        void look();
        void printLastSeen();

    private:
        Display* display;
        Window winFocus;
        Window winRoot;

        std::vector<unsigned long> pixels;

        int idx(int& x,int& y);

        int width,height;

};

#endif // EYES_H_INCLUDED
