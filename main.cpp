#include "Eyes.h"
#include "SnesController.h"
#include <fstream>
#include "NeuralNetwork.h"
#include <omp.h>
#include <unistd.h>
#include <iomanip>

#define DATA_SIZE 10000
#define AVERAGE 6330854.822645681
#define STANDARD_DEVIATION 7370946.75339683
#define TEST_SAMPLE 14

#define LAYERS 3
#define LAYER_ARRAY {56388,20,128}

void createTrainingData();
void play();
void trainNeuralNetwork();
void selfTeachNeuralNetwork();

template<class EMClass>
void importFromData(std::string filename,EMClass& dataStruct,int rows,int cols);



int main(){

    //createTrainingData();

    //trainNeuralNetwork();

    selfTeachNeuralNetwork();

    //play();

    return 0;

}

double f(NeuralNetwork& marioAI,Eyes& eyes,SnesController& controller){

    double startTime = omp_get_wtime();

    int intendedConfig;

    eyes.initializeReferenceTriangle();

    while(true){

        usleep(100000);

        eyes.lookScreen();

        Eigen::VectorXd XTemp = eyes.returnVectorImage();

        eyes.printXVector( XTemp );

        eyes.printScreenSection(25,100,25,100);

        eyes.findMario();

        // UP TO HERE - TAKE A MAJORITY VOTE OF "REFERENCE TRIANGLE"
        // AS AN ERROR CORRECTION, USE IT WITH MARIO POSITION TO CALCULATE HOW FAR MARIO MOVES
        // OUTPUT THAT DISTANCE OF THIS FUNCTION FACTOR IN TIME AND OPTIMIZE WITH ANNEALING
        // USE SAVE AND LOAD STATES TO ITERATE TRIALS, FIND A WAY TO DETERMINE IF YOU WON OR LOST

        break;

        marioAI.feedForwardPropagate( XTemp );

        intendedConfig = marioAI.returnPrediction();

        controller.setControllerConfig( intendedConfig );

        controller.printPressedButtons( intendedConfig );

    }

    return omp_get_wtime() - startTime;

}

void anneal(NeuralNetwork& marioAI,Eyes& eyes,SnesController& controller){

    f(marioAI,eyes,controller);

    return;

}

void selfTeachNeuralNetwork(){

    srand(611*time(NULL));

    std::cout << "Set Mario at the beginning of training level" << std::endl;
    std::cout << " and unload layers 2 and 3 in ZSNES." << std::endl;
    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }
    std::cout << "go" << std::endl;

    Eyes eyes;
    SnesController controller;

    int layerTest[LAYERS] = LAYER_ARRAY;

    NeuralNetwork marioAI(LAYERS,128,layerTest);
    marioAI.importTheta();

    double avg = AVERAGE;
    double stdDev = STANDARD_DEVIATION;

    eyes.setXStatistics(avg,stdDev);

    controller.toggleBG1Layer();
    controller.toggleBG4Layer();
    controller.toggleSpriteLayer();

    eyes.setBackgroundColor();

    controller.toggleBG1Layer();
    controller.toggleBG4Layer();
    controller.toggleSpriteLayer();

    anneal(marioAI,eyes,controller);

    return;

}

void trainNeuralNetwork(){

    Eigen::MatrixXd X(DATA_SIZE,56388);
    Eigen::MatrixXi Y(DATA_SIZE,1);

    importFromData("X.dat",X,DATA_SIZE,56388);
    importFromData("Y.dat",Y,DATA_SIZE,1);

    X = X.transpose().eval();
    Y = Y.transpose().eval();

    double avg = X.mean();
    double stdDev = sqrt( ( (X.array() - avg) * (X.array() - avg) ).sum() / (X.cols()*X.rows() -1.0) );

    X = (X.array() - avg);
    X /= stdDev;

    int layerTest[LAYERS] = LAYER_ARRAY;

    NeuralNetwork marioAI(LAYERS,128,layerTest);

    marioAI.train(X,Y);

    marioAI.saveTheta();

    std::cout << "Average: " << std::setprecision(16) << avg << std::endl;
    std::cout << "Standard Deviation: " << std::setprecision(16) << stdDev << std::endl;

    Eigen::VectorXd XTest = X.col(TEST_SAMPLE);

    marioAI.trainingDataClassificationTest(X,Y);

    marioAI.feedForwardPropagate(XTest);

    std::cout << "Feed forward test: " << marioAI.returnPrediction() << std::endl;

    std::cout << "Actual value: " << Y(0,TEST_SAMPLE) << std::endl;

    return;

}

void play(){

    std::cout << "Open emulator window..." << std::endl;
    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }
    std::cout << "go" << std::endl;

    Eyes eyes;
    SnesController controller;

    int layerTest[LAYERS] = LAYER_ARRAY;

    NeuralNetwork marioAI(LAYERS,128,layerTest);
    marioAI.importTheta();

    double avg = AVERAGE;
    double stdDev = STANDARD_DEVIATION;

    eyes.setXStatistics(avg,stdDev);

    int intendedConfig;

    while(true){

        usleep(100000);

        eyes.lookScreen();

        Eigen::VectorXd XTemp = eyes.returnVectorImage();

        //eyes.printXVector( XTemp );

        marioAI.feedForwardPropagate( XTemp );

        intendedConfig = marioAI.returnPrediction();

        controller.setControllerConfig( intendedConfig );

        controller.printPressedButtons( intendedConfig );

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

void createTrainingData(){

    std::cout << "Open emulator window..." << std::endl;
    for(int i=2;i>=0;i--){
        sleep(1);
        std::cout << i << std::endl;
    }
    std::cout << "go" << std::endl;

    Eyes eyes;
    SnesController controller;

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
            usleep(200000);
            eyes.lookScreen();
            controller.pollController();
            eyes.printLastX(outfile);
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
