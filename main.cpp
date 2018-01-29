#include "Eyes.h"
#include "SnesController.h"
#include <fstream>
#include "NeuralNetwork.h"

void createTrainingData(Eyes& eyes,SnesController& controller);

int main(){

    std::cout << "Open emulator window..." << std::endl;

    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }

    Eyes eyes;
    SnesController controller;

    std::cout << "go" << std::endl;

    eyes.lookScreen();

    eyes.printLastSeen();

    createTrainingData(eyes,controller);

//    int layerUnits[3] = {229376,25,256};
//
//    for(int i=0;i<3;i++) std::cout << layerUnits[i] << "\t";
//    std::cout << std::endl;
//
//    NeuralNetwork marioPlayer(3,256,layerUnits);

    return 0;

}


void createTrainingData(Eyes& eyes,SnesController& controller){

    //for(int i=0;i<2000;i++){

        std::ofstream outfile("X.dat",std::ofstream::app);
        sleep(1);
        eyes.lookScreen();
        controller.pollController();
        eyes.printLastSeen(outfile);
        outfile << std::endl;
        outfile.close();

        outfile.open("Y.dat",std::ofstream::app);
        outfile << controller.configuration << std::endl;
        outfile.close();

//    }

    return;

}
