#include "Eyes.h"
#include "SnesController.h"
#include <fstream>
#include "NeuralNetwork.h"
#include <omp.h>
#include <unistd.h>
#include <iomanip>

#define DATA_SIZE 3000

void createTrainingData(Eyes& eyes,SnesController& controller);
void play(NeuralNetwork& marioAI,SnesController& controller,Eyes& eyes);

template<class EMClass>
void importFromData(std::string filename,EMClass& dataStruct,int rows,int cols);

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

    //createTrainingData(eyes,controller);

    //Eigen::MatrixXd X(DATA_SIZE,56388);
    //Eigen::MatrixXi Y(DATA_SIZE,1);

    //importFromData("X.dat",X,DATA_SIZE,56388);
    //importFromData("Y.dat",Y,DATA_SIZE,1);

    //X = X.transpose().eval();
    //Y = Y.transpose().eval();

    //double avg = X.mean();
    //double stdDev = sqrt( ( (X.array() - avg) * (X.array() - avg) ).sum() / (X.cols()*X.rows() -1.0) );

    double avg = 5660309.226747299;
    double stdDev = 6996794.565084721;

    //std::cout << "Average: " << std::setprecision(16) << avg << std::endl;
    //std::cout << "Standard Deviation: " << std::setprecision(16) << stdDev << std::endl;

    eyes.setXStatistics(avg,stdDev);

    //X = (X.array() - avg);
    //X /= stdDev;

    //Eigen::VectorXd XTest = X.col(14);

    //eyes.printXVector(XTest);
    //controller.printPressedButtons(Y(0,14));

    int layerTest[3] = {56388,10,128};

    NeuralNetwork marioAI(3,128,layerTest);

    marioAI.importTheta();

    //marioAI.train(X,Y);

    //marioAI.saveTheta();

    //marioAI.trainingDataClassificationTest(X,Y);

    //marioAI.feedForwardPropagate(XTest);

    //controller.printPressedButtons(marioAI.returnPrediction());

    play(marioAI,controller,eyes);

    return 0;

}

void play(NeuralNetwork& marioAI,SnesController& controller,Eyes& eyes){

    int intendedConfig;
    int t=0;
    while(true){

        usleep(200000);

        eyes.lookScreen();

        Eigen::VectorXd XTemp = eyes.returnVectorImage();

        //eyes.printXVector( XTemp );

        marioAI.feedForwardPropagate( XTemp );

        intendedConfig = marioAI.returnPrediction();

        controller.setControllerConfig( intendedConfig );

        //controller.printPressedButtons( intendedConfig );

        t++;

    }

    return;

}

template<class EMClass>
void importFromData(std::string filename,EMClass& dataStruct,int rows,int cols){

    std::ifstream infile(filename.c_str());

    for(int i=0;i<rows;i++) for(int j=0;j<cols;j++){

        infile >> dataStruct(i,j);

    }

    infile.close();

    return;

}

void createTrainingData(Eyes& eyes,SnesController& controller){

    remove("X.dat");
    remove("Y.dat");

    omp_set_num_threads(2);

#pragma omp parallel shared(controller)
{
    int threadNum = omp_get_thread_num();

    if( threadNum == 0 ) controller.monitorController();

    else if( threadNum == 1){

        for(int i=0;i<DATA_SIZE;i++){

            std::cout << "Learning iteration: " << i << std::endl;

            std::ofstream outfile("X.dat",std::ofstream::app);
            usleep(500000);
            eyes.lookScreen();
            controller.pollController();
            eyes.printLastSeen(outfile);
            outfile << std::endl;
            outfile.close();
            outfile.open("Y.dat",std::ofstream::app);
            outfile << controller.configuration << std::endl;
            outfile.close();

        }

    }

    else assert(false);

    std::cout << "Thread " << threadNum << " is finished." << std::endl;

}

    return;

}
