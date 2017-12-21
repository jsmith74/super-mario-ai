#ifndef SNESCONTROLLER_H_INCLUDED
#define SNESCONTROLLER_H_INCLUDED

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
#include <unistd.h>
#include <X11/extensions/XTest.h>

class SnesController{

    public:
        SnesController();
        void testButton();

    private:
        XKeyEvent createKeyEvent(Display *display, Window &win,Window &winRoot, bool press,int keycode, int modifiers);

};

#endif // SNESCONTROLLER_H_INCLUDED
