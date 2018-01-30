#include <fstream>
#include "NeuralNetwork.h"
#include <omp.h>
#include <unistd.h>
#include <iomanip>

#define DATA_SIZE 300
#define AVERAGE 3650694.186012154
#define STANDARD_DEVIATION 4383510.452386877
#define TEST_SAMPLE 14

#define LAYERS 3
#define LAYER_ARRAY {56388,10,128}

void trainNeuralNetwork();

template<class EMClass>
void importFromData(std::string filename,EMClass& dataStruct,int rows,int cols);



int main(){

    trainNeuralNetwork();

    return 0;

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



template<class EMClass>
void importFromData(std::string filename,EMClass& dataStruct,int rows,int cols){

    std::ifstream infile(filename.c_str());

    for(int i=0;i<rows;i++) for(int j=0;j<cols;j++){

        infile >> dataStruct(i,j);

    }

    infile.close();

    return;

}

