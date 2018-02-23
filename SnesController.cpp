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

    button1 = createKeyEvent(display,winFocus,winRoot,XK_1);
    button2 = createKeyEvent(display,winFocus,winRoot,XK_2);
    button3 = createKeyEvent(display,winFocus,winRoot,XK_3);
    button4 = createKeyEvent(display,winFocus,winRoot,XK_4);
    button5 = createKeyEvent(display,winFocus,winRoot,XK_5);

    for(int i=0;i<7;i++) boolConfigutarion[i] = false;
}

void SnesController::toggleBG1Layer(){

    button1.type = KeyPress;
    XSendEvent(button1.display, button1.window, True, KeyPressMask, (XEvent *)&button1);
    XFlush(button1.display);

    usleep(100000);

    button1.type = KeyRelease;
    XSendEvent(button1.display, button1.window, True, KeyPressMask, (XEvent *)&button1);
    XFlush(button1.display);

    return;

}

void SnesController::toggleBG2Layer(){

    button2.type = KeyPress;
    XSendEvent(button2.display, button2.window, True, KeyPressMask, (XEvent *)&button2);
    XFlush(button2.display);

    usleep(100000);

    button2.type = KeyRelease;
    XSendEvent(button2.display, button2.window, True, KeyPressMask, (XEvent *)&button2);
    XFlush(button2.display);

    return;

}

void SnesController::toggleBG3Layer(){

    button3.type = KeyPress;
    XSendEvent(button3.display, button3.window, True, KeyPressMask, (XEvent *)&button3);
    XFlush(button3.display);

    usleep(100000);

    button3.type = KeyRelease;
    XSendEvent(button3.display, button3.window, True, KeyPressMask, (XEvent *)&button3);
    XFlush(button3.display);

    return;

}

void SnesController::toggleBG4Layer(){

    button4.type = KeyPress;
    XSendEvent(button4.display, button4.window, True, KeyPressMask, (XEvent *)&button4);
    XFlush(button4.display);

    usleep(100000);

    button4.type = KeyRelease;
    XSendEvent(button4.display, button4.window, True, KeyPressMask, (XEvent *)&button4);
    XFlush(button4.display);

    return;

}

void SnesController::toggleSpriteLayer(){

    button5.type = KeyPress;
    XSendEvent(button5.display, button5.window, True, KeyPressMask, (XEvent *)&button5);
    XFlush(button5.display);

    usleep(100000);

    button5.type = KeyRelease;
    XSendEvent(button5.display, button5.window, True, KeyPressMask, (XEvent *)&button5);
    XFlush(button5.display);

    return;

}

void SnesController::monitorController(){

    XEvent event;
    XSelectInput(display, winFocus, KeyPressMask | KeyReleaseMask );

    while(true){

        XNextEvent(display, &event);

        if (event.type == KeyPress){
            if(event.xkey.keycode == 52){
                assert(!boolConfigutarion[0]);
                boolConfigutarion[0] = true;
            }
            if(event.xkey.keycode == 53){
                assert(!boolConfigutarion[1]);
                boolConfigutarion[1] = true;
            }
            if(event.xkey.keycode == 38){
                assert(!boolConfigutarion[2]);
                boolConfigutarion[2] = true;
            }
            if(event.xkey.keycode == 113){
                assert(!boolConfigutarion[3]);
                boolConfigutarion[3] = true;
            }
            if(event.xkey.keycode == 114){
                assert(!boolConfigutarion[4]);
                boolConfigutarion[4] = true;
            }
            if(event.xkey.keycode == 111){
                assert(!boolConfigutarion[5]);
                boolConfigutarion[5] = true;
            }
            if(event.xkey.keycode == 116){
                assert(!boolConfigutarion[6]);
                boolConfigutarion[6] = true;
            }

            if(event.xkey.keycode == 58) return;

        }

        if (event.type == KeyRelease){
            if(event.xkey.keycode == 52) boolConfigutarion[0] = false;
            if(event.xkey.keycode == 53) boolConfigutarion[1] = false;
            if(event.xkey.keycode == 38) boolConfigutarion[2] = false;
            if(event.xkey.keycode == 113) boolConfigutarion[3] = false;
            if(event.xkey.keycode == 114) boolConfigutarion[4] = false;
            if(event.xkey.keycode == 111) boolConfigutarion[5] = false;
            if(event.xkey.keycode == 116) boolConfigutarion[6] = false;

        }

    }

    return;

}

void SnesController::setControllerConfig(int config){

    configuration = config;

    if(config/64==1){
        pressDOWN();
        config -= 64;
    }
    else releaseDOWN();

    if(config/32==1){
        pressUP();
        config -= 32;
    }
    else releaseUP();

    if(config/16==1){
        pressRIGHT();
        config -= 16;
    }
    else releaseRIGHT();

    if(config/8==1){
        pressLEFT();
        config -= 8;
    }
    else releaseLEFT();

    if(config/4==1){
        pressY();
        config -= 4;
    }
    else releaseY();

    if(config/2==1){
        pressA();
        config -= 2;
    }
    else releaseA();

    if(config==1){
        pressB();
    }
    else releaseB();

    return;

}

void SnesController::printPressedButtons(int config){

    std::cout << "Pressed buttons:\n";

    if(config/64==1){
        std::cout << "Down" << std::endl;
        config -= 64;
    }
    if(config/32==1){
        std::cout << "Up" << std::endl;
        config -= 32;
    }
    if(config/16==1){
        std::cout << "Right" << std::endl;
        config -= 16;
    }
    if(config/8==1){
        std::cout << "Left" << std::endl;
        config -= 8;
    }
    if(config/4==1){
        std::cout << "Y" << std::endl;
        config -= 4;
    }
    if(config/2==1){
        std::cout << "A" << std::endl;
        config -= 2;
    }
    if(config==1){
        std::cout << "B" << std::endl;
    }

    return;

}

void SnesController::pollController(){

    configuration =  1 * boolConfigutarion[0] +  2 * boolConfigutarion[1] +
                     4 * boolConfigutarion[2] +  8 * boolConfigutarion[3] +
                    16 * boolConfigutarion[4] + 32 * boolConfigutarion[5] +
                    64 * boolConfigutarion[6];

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
