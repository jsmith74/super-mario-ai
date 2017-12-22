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

        void pressUP();
        void pressDOWN();
        void pressLEFT();
        void pressRIGHT();

        void releaseUP();
        void releaseDOWN();
        void releaseLEFT();
        void releaseRIGHT();

        void pressL();
        void pressR();

        void releaseL();
        void releaseR();

        void pressSTART();
        void pressSELECT();

        void releaseSTART();
        void releaseSELECT();

        void pressY();
        void pressX();
        void pressA();
        void pressB();

        void releaseY();
        void releaseX();
        void releaseA();
        void releaseB();

        void saveState();
        void loadState();

    private:
        XKeyEvent createKeyEvent(Display *display, Window &win,Window &winRoot, int keycode);

        Display *display;
        Window winFocus;
        Window winRoot;
        XKeyEvent buttonUP, buttonDOWN, buttonLEFT, buttonRIGHT, buttonL, buttonR;
        XKeyEvent buttonSTART, buttonSELECT, buttonY, buttonX, buttonA, buttonB;
        XKeyEvent buttonSaveState,buttonLoadState;


};

#endif // SNESCONTROLLER_H_INCLUDED
