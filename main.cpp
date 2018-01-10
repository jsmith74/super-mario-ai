#include "SnesController.h"
#include "Eyes.h"
#include <fstream>

int main(){

    std::cout << "Open emulator window..." << std::endl;

    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }

    Eyes eyes;

    SnesController controller;

    controller.pressUP();

    sleep(1);

    eyes.look();

    sleep(1);

    controller.releaseUP();

    eyes.printLastSeen();

    return 0;

}
