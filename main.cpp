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

    std::cout << "go" << std::endl;

    eyes.lookScreen();

    eyes.findMario();

    return 0;

}
