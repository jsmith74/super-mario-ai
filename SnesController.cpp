#include "SnesController.h"

#define KEYCODE XK_z
// The key code to be sent.
// A full list of available codes can be found in /usr/include/X11/keysymdef.h

SnesController::SnesController(){


}

XKeyEvent SnesController::createKeyEvent(Display *display, Window &win,Window &winRoot, bool press,int keycode, int modifiers){

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
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;

}

void SnesController::testButton(){

    std::cout << KEYCODE << std::endl;

// Obtain the X11 display.
   Display *display = XOpenDisplay(0);
   if(display == NULL){
      std::cout << "display" << std::endl;
      return;
   }

   usleep(3000000);

// Get the root window for the current display.
   Window winRoot = XDefaultRootWindow(display);
   std::cout << winRoot << std::endl;

// Find the window which has the current keyboard focus.
   Window winFocus;
   int    revert;
   XGetInputFocus(display, &winFocus, &revert);
   std::cout << winFocus << std::endl << std::endl;

// Send a fake key press event to the window.
   //XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, KEYCODE, 0);
   //XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
   //XTestFakeKeyEvent(event.display, event.keycode, True, CurrentTime);

// Send a fake key release event to the window.
   XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, KEYCODE, 0);
   //XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
   XTestFakeKeyEvent(event.display, event.keycode, True, 0);

   event = createKeyEvent(display, winFocus, winRoot, false, KEYCODE, 0);

   XTestFakeKeyEvent(event.display, event.keycode, False, 1000);


// Done.
   XCloseDisplay(display);

    return;

}

