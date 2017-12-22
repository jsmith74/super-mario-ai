#include "SnesController.h"


int main(){

    SnesController controller;

    controller.pressA();

    controller.pressRIGHT();

    controller.pressY();

    sleep(6);

    for(int i=0;i<20;i++){

        controller.releaseA();
        sleep(1);
        controller.pressA();
        sleep(1);

    }

    controller.releaseA();

    controller.releaseRIGHT();

    controller.releaseY();

    return 0;

}
