#include "SnesController.h"


int main(){

    SnesController controller;

    controller.pressA();

    controller.pressRIGHT();

    controller.pressY();

    sleep(1);

    for(int i=0;i<10;i++){

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
