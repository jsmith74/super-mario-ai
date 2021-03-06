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

        void pollController();
        void monitorController();
        void printPressedButtons(int config);
        void setControllerConfig(int config);
        int configuration;

        void toggleBG1Layer();
        void toggleBG2Layer();
        void toggleBG3Layer();
        void toggleBG4Layer();
        void toggleSpriteLayer();

    private:
        XKeyEvent createKeyEvent(Display *display, Window &win,Window &winRoot, int keycode);

        Display *display;
        Window winFocus;
        Window winRoot;
        XKeyEvent buttonUP, buttonDOWN, buttonLEFT, buttonRIGHT, buttonL, buttonR;
        XKeyEvent buttonSTART, buttonSELECT, buttonY, buttonX, buttonA, buttonB;
        XKeyEvent buttonSaveState,buttonLoadState, button1, button2, button3, button4, button5;

        bool boolConfigutarion[7];


};

#endif // SNESCONTROLLER_H_INCLUDED
