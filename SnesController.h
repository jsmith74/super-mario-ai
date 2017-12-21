#ifndef SNESCONTROLLER_H_INCLUDED
#define SNESCONTROLLER_H_INCLUDED

#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
#include <unistd.h>
#include <X11/extensions/XTest.h>

class SnesController{

    public:
        SnesController();
        ~SnesController();

        void pressRight();
        void pressB();
        void releaseRight();

    private:
        XKeyEvent createKeyEvent(Display *display, Window &win,Window &winRoot, int keycode);

        Display *display;
        Window winFocus;
        Window winRoot;
        XKeyEvent buttonUP, buttonDOWN, buttonLEFT, buttonRIGHT, buttonL, buttonR;
        XKeyEvent buttonSTART, buttonSELECT, buttonY, buttonX, buttonA, buttonB;


};

#endif // SNESCONTROLLER_H_INCLUDED
