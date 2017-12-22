#include "SnesController.h"

// A full list of available button keycodes can be found in /usr/include/X11/keysymdef.h

SnesController::SnesController(){

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");

    std::cout << "Open emulator window..." << std::endl;

    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }

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

    XTestFakeKeyEvent(buttonUP.display, buttonUP.keycode, True, 0);
    XFlush(buttonUP.display);

}

void SnesController::pressDOWN(){

    XTestFakeKeyEvent(buttonDOWN.display, buttonDOWN.keycode, True, 0);
    XFlush(buttonDOWN.display);

}

void SnesController::pressLEFT(){

    XTestFakeKeyEvent(buttonLEFT.display, buttonLEFT.keycode, True, 0);
    XFlush(buttonLEFT.display);

}


void SnesController::pressRIGHT(){

    XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, True, 0);
    XFlush(buttonRIGHT.display);

}

void SnesController::releaseUP(){

    XTestFakeKeyEvent(buttonUP.display, buttonUP.keycode, False, 0);
    XFlush(buttonUP.display);

}

void SnesController::releaseDOWN(){

    XTestFakeKeyEvent(buttonDOWN.display, buttonDOWN.keycode, False, 0);
    XFlush(buttonDOWN.display);

}

void SnesController::releaseLEFT(){

    XTestFakeKeyEvent(buttonLEFT.display, buttonLEFT.keycode, False, 0);
    XFlush(buttonLEFT.display);

}

void SnesController::releaseRIGHT(){

    XTestFakeKeyEvent(buttonRIGHT.display, buttonRIGHT.keycode, False, 0);
    XFlush(buttonRIGHT.display);

}

void SnesController::pressL(){

    XTestFakeKeyEvent(buttonL.display, buttonL.keycode, True, 0);
    XFlush(buttonL.display);

}

void SnesController::pressR(){

    XTestFakeKeyEvent(buttonR.display, buttonR.keycode, True, 0);
    XFlush(buttonR.display);

}

void SnesController::releaseL(){

    XTestFakeKeyEvent(buttonL.display, buttonL.keycode, False, 0);
    XFlush(buttonL.display);

}

void SnesController::releaseR(){

    XTestFakeKeyEvent(buttonR.display, buttonR.keycode, False, 0);
    XFlush(buttonR.display);

}

void SnesController::pressSTART(){

    XTestFakeKeyEvent(buttonSTART.display, buttonSTART.keycode, True, 0);
    XFlush(buttonSTART.display);

}

void SnesController::pressSELECT(){

    XTestFakeKeyEvent(buttonSELECT.display, buttonSELECT.keycode, True, 0);
    XFlush(buttonSELECT.display);

}

void SnesController::releaseSTART(){

    XTestFakeKeyEvent(buttonSTART.display, buttonSTART.keycode, False, 0);
    XFlush(buttonSTART.display);

}

void SnesController::releaseSELECT(){

    XTestFakeKeyEvent(buttonSELECT.display, buttonSELECT.keycode, False, 0);
    XFlush(buttonSELECT.display);

}

void SnesController::pressY(){

    XTestFakeKeyEvent(buttonY.display, buttonY.keycode, True, 0);
    XFlush(buttonY.display);

}

void SnesController::pressX(){

    XTestFakeKeyEvent(buttonX.display, buttonX.keycode, True, 0);
    XFlush(buttonX.display);

}

void SnesController::pressA(){

    XTestFakeKeyEvent(buttonA.display, buttonA.keycode, True, 0);
    XFlush(buttonA.display);

}

void SnesController::pressB(){

    XTestFakeKeyEvent(buttonB.display, buttonB.keycode, True, 0);
    XFlush(buttonB.display);

}

void SnesController::releaseY(){

    XTestFakeKeyEvent(buttonY.display, buttonY.keycode, False, 0);
    XFlush(buttonY.display);

}

void SnesController::releaseX(){

    XTestFakeKeyEvent(buttonX.display, buttonX.keycode, False, 0);
    XFlush(buttonX.display);

}

void SnesController::releaseA(){

    XTestFakeKeyEvent(buttonA.display, buttonA.keycode, False, 0);
    XFlush(buttonA.display);

}
void SnesController::releaseB(){

    XTestFakeKeyEvent(buttonB.display, buttonB.keycode, False, 0);
    XFlush(buttonB.display);

}

SnesController::~SnesController(){

    XCloseDisplay(display);

}
