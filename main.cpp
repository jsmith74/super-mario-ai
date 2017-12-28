#include "SnesController.h"


int main(){

    SnesController controller;

    controller.pressUP();

    sleep(2);

    controller.releaseUP();

    controller.pressDOWN();

    sleep(1);

    controller.releaseDOWN();

    controller.pressA();

    controller.pressRIGHT();

    controller.pressY();

    sleep(6);

    for(int i=0;i<15;i++){

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
