#include "SnesController.h"

// Macros for keycodes, e.g. "XK_Right" are found in /usr/include/X11/keysymdef.h

SnesController::SnesController(){

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");

    winRoot = XDefaultRootWindow(display);

    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    buttonUP = createKeyEvent(display,winFocus,winRoot,XK_Up);
    buttonDOWN = createKeyEvent(display,winFocus,winRoot,XK_Down);
    buttonLEFT = createKeyEvent(display,winFocus,winRoot,XK_Left);
    buttonRIGHT = createKeyEvent(display,winFocus,winRoot,XK_Right);

    buttonL = createKeyEvent(display,winFocus,winRoot,XK_q);
    buttonR = createKeyEvent(display,winFocus,winRoot,XK_w);

    buttonSTART = createKeyEvent(display,winFocus,winRoot,XK_o);
    buttonSELECT = createKeyEvent(display,winFocus,winRoot,XK_p);

    buttonY = createKeyEvent(display,winFocus,winRoot,XK_a);
    buttonX = createKeyEvent(display,winFocus,winRoot,XK_s);
    buttonA = createKeyEvent(display,winFocus,winRoot,XK_x);
    buttonB = createKeyEvent(display,winFocus,winRoot,XK_z);

    buttonSaveState = createKeyEvent(display,winFocus,winRoot,XK_k);
    buttonLoadState = createKeyEvent(display,winFocus,winRoot,XK_l);

    boolUP = false;
    boolDOWN = false;
    boolLEFT = false;
    boolRIGHT = false;
    boolY = false;
    boolX = false;
    boolB = false;
    boolA = false;

}

void SnesController::pollController(){

    XEvent event;
    XSelectInput(display, winFocus, KeyPressMask | KeyReleaseMask );

    while(true){

    XNextEvent(display, &event);

    if (event.type == KeyPress){

        std::cout << "Key press: " << event.xkey.keycode << "\t" << XK_z << std::endl;

        if(event.xkey.keycode == 58) return;

    }

    if (event.type == KeyRelease){

        std::cout << "Key release: " << event.xkey.keycode << std::endl;

    }

}

    return;

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

void SnesController::saveState(){

    XTestFakeKeyEvent(buttonSaveState.display, buttonSaveState.keycode, True, 0);
    XTestFakeKeyEvent(buttonSaveState.display, buttonSaveState.keycode, False, 1000);

}

void SnesController::loadState(){

    XTestFakeKeyEvent(buttonLoadState.display, buttonLoadState.keycode, True, 0);
    XTestFakeKeyEvent(buttonLoadState.display, buttonLoadState.keycode, False, 1000);

}

void SnesController::pressUP(){

    //XTestFakeKeyEvent(buttonUP.display, buttonUP.keycode, True, 0);
    buttonUP.type = KeyPress;
    XSendEvent(buttonUP.display, buttonUP.window, True, KeyPressMask, (XEvent *)&buttonUP);
    XFlush(buttonUP.display);

}

void SnesController::pressDOWN(){

    //XTestFakeKeyEvent(buttonDOWN.display, buttonDOWN.keycode, True, 0);
    buttonDOWN.type = KeyPress;
    XSendEvent(buttonDOWN.display, buttonDOWN.window, True, KeyPressMask, (XEvent *)&buttonDOWN);
    XFlush(buttonDOWN.display);

}

void SnesController::pressLEFT(){

    //XTestFakeKeyEvent(buttonLEFT.display, buttonLEFT.keycode, True, 0);
    buttonLEFT.type = KeyPress;
    XSendEvent(buttonLEFT.display, buttonLEFT.window, True, KeyPressMask, (XEvent *)&buttonLEFT);
    XFlush(buttonLEFT.display);

}


void SnesController::pressRIGHT(){

    //XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, True, 0);
    buttonRIGHT.type = KeyPress;
    XSendEvent(buttonRIGHT.display, buttonRIGHT.window, True, KeyPressMask, (XEvent *)&buttonRIGHT);
    XFlush(buttonRIGHT.display);

}

void SnesController::releaseUP(){

    //XTestFakeKeyEvent(buttonUP.display, buttonUP.keycode, False, 0);
    buttonUP.type = KeyRelease;
    XSendEvent(buttonUP.display, buttonUP.window, True, KeyPressMask, (XEvent *)&buttonUP);
    XFlush(buttonUP.display);

}

void SnesController::releaseDOWN(){

    //XTestFakeKeyEvent(buttonDOWN.display, buttonDOWN.keycode, False, 0);
    buttonDOWN.type = KeyRelease;
    XSendEvent(buttonDOWN.display, buttonDOWN.window, True, KeyPressMask, (XEvent *)&buttonDOWN);
    XFlush(buttonDOWN.display);

}

void SnesController::releaseLEFT(){

    //XTestFakeKeyEvent(buttonLEFT.display, buttonLEFT.keycode, False, 0);
    buttonLEFT.type = KeyRelease;
    XSendEvent(buttonLEFT.display, buttonLEFT.window, True, KeyPressMask, (XEvent *)&buttonLEFT);
    XFlush(buttonLEFT.display);

}

void SnesController::releaseRIGHT(){

    //XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, False, 0);
    buttonRIGHT.type = KeyRelease;
    XSendEvent(buttonRIGHT.display, buttonRIGHT.window, True, KeyPressMask, (XEvent *)&buttonRIGHT);
    XFlush(buttonRIGHT.display);

}

void SnesController::pressL(){

    //XTestFakeKeyEvent(buttonL.display, buttonL.keycode, True, 0);
    buttonL.type = KeyPress;
    XSendEvent(buttonL.display, buttonL.window, True, KeyPressMask, (XEvent *)&buttonL);
    XFlush(buttonL.display);

}

void SnesController::pressR(){

    //XTestFakeKeyEvent(buttonR.display, buttonR.keycode, True, 0);
    buttonR.type = KeyPress;
    XSendEvent(buttonR.display, buttonR.window, True, KeyPressMask, (XEvent *)&buttonR);
    XFlush(buttonR.display);

}

void SnesController::releaseL(){

    //XTestFakeKeyEvent(buttonL.display, buttonL.keycode, False, 0);
    buttonL.type = KeyRelease;
    XSendEvent(buttonL.display, buttonL.window, True, KeyPressMask, (XEvent *)&buttonL);
    XFlush(buttonL.display);

}

void SnesController::releaseR(){

    //XTestFakeKeyEvent(buttonR.display, buttonR.keycode, False, 0);
    buttonR.type = KeyRelease;
    XSendEvent(buttonR.display, buttonR.window, True, KeyPressMask, (XEvent *)&buttonR);
    XFlush(buttonR.display);

}

void SnesController::pressSTART(){

    //XTestFakeKeyEvent(buttonSTART.display, buttonSTART.keycode, True, 0);
    buttonSTART.type = KeyPress;
    XSendEvent(buttonSTART.display, buttonSTART.window, True, KeyPressMask, (XEvent *)&buttonSTART);
    XFlush(buttonSTART.display);

}

void SnesController::pressSELECT(){

    //XTestFakeKeyEvent(buttonSELECT.display, buttonSELECT.keycode, True, 0);
    buttonSELECT.type = KeyPress;
    XSendEvent(buttonSELECT.display, buttonSELECT.window, True, KeyPressMask, (XEvent *)&buttonSELECT);
    XFlush(buttonSELECT.display);

}

void SnesController::releaseSTART(){

    //XTestFakeKeyEvent(buttonSTART.display, buttonSTART.keycode, False, 0);
    buttonSTART.type = KeyRelease;
    XSendEvent(buttonSTART.display, buttonSTART.window, True, KeyPressMask, (XEvent *)&buttonSTART);
    XFlush(buttonSTART.display);

}

void SnesController::releaseSELECT(){

    //XTestFakeKeyEvent(buttonSELECT.display, buttonSELECT.keycode, False, 0);
    buttonSELECT.type = KeyRelease;
    XSendEvent(buttonSELECT.display, buttonSELECT.window, True, KeyPressMask, (XEvent *)&buttonSELECT);
    XFlush(buttonSELECT.display);

}

void SnesController::pressY(){

    //XTestFakeKeyEvent(buttonY.display, buttonY.keycode, True, 0);
    buttonY.type = KeyPress;
    XSendEvent(buttonY.display, buttonY.window, True, KeyPressMask, (XEvent *)&buttonY);
    XFlush(buttonY.display);

}

void SnesController::pressX(){

    //XTestFakeKeyEvent(buttonX.display, buttonX.keycode, True, 0);
    buttonX.type = KeyPress;
    XSendEvent(buttonX.display, buttonX.window, True, KeyPressMask, (XEvent *)&buttonX);
    XFlush(buttonX.display);

}

void SnesController::pressA(){

    //XTestFakeKeyEvent(buttonA.display, buttonA.keycode, True, 0);
    buttonA.type = KeyPress;
    XSendEvent(buttonA.display, buttonA.window, True, KeyPressMask, (XEvent *)&buttonA);

    XFlush(buttonA.display);

}

void SnesController::pressB(){

    //XTestFakeKeyEvent(buttonB.display, buttonB.keycode, True, 0);
    buttonB.type = KeyPress;
    XSendEvent(buttonB.display, buttonB.window, True, KeyPressMask, (XEvent *)&buttonB);
    XFlush(buttonB.display);

}

void SnesController::releaseY(){

    //XTestFakeKeyEvent(buttonY.display, buttonY.keycode, False, 0);
    buttonY.type = KeyRelease;
    XSendEvent(buttonY.display, buttonY.window, True, KeyPressMask, (XEvent *)&buttonY);
    XFlush(buttonY.display);

}

void SnesController::releaseX(){

    //XTestFakeKeyEvent(buttonX.display, buttonX.keycode, False, 0);
    buttonX.type = KeyRelease;
    XSendEvent(buttonX.display, buttonX.window, True, KeyPressMask, (XEvent *)&buttonX);
    XFlush(buttonX.display);

}

void SnesController::releaseA(){

    //XTestFakeKeyEvent(buttonA.display, buttonA.keycode, False, 0);
    buttonA.type = KeyRelease;
    XSendEvent(buttonA.display, buttonA.window, True, KeyPressMask, (XEvent *)&buttonA);
    XFlush(buttonA.display);

}

void SnesController::releaseB(){

    //XTestFakeKeyEvent(buttonB.display, buttonB.keycode, False, 0);
    buttonB.type = KeyRelease;
    XSendEvent(buttonB.display, buttonB.window, True, KeyPressMask, (XEvent *)&buttonB);
    XFlush(buttonB.display);

}

SnesController::~SnesController(){

    XCloseDisplay(display);

}
