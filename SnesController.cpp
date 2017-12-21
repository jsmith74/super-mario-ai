#include "SnesController.h"

#define KEYCODE XK_Right
// A full list of available codes can be found in /usr/include/X11/keysymdef.h

SnesController::SnesController(){

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");

    std::cout << "Open emulator window..." << std::endl;

    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }

// Get the root window for the current display.
    winRoot = XDefaultRootWindow(display);

// Find the window which has the current keyboard focus.
    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    buttonRIGHT = createKeyEvent(display,winFocus,winRoot,XK_Right);
    buttonB = createKeyEvent(display,winFocus,winRoot,XK_z);

}


XKeyEvent SnesController::createKeyEvent(Display *display, Window &win,Window &winRoot, int keycode){

   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);

   return event;

}

void SnesController::pressRight(){

    XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, True, 0);

    return;

}

void SnesController::releaseRight(){

    XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, True, 0);

    return;

}

void SnesController::pressB(){

    XTestFakeKeyEvent(buttonB.display, buttonB.keycode, True, 0);

}

SnesController::~SnesController(){

    XCloseDisplay(display);

}
