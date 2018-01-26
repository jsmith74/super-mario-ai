#ifndef NEURALNETWORK_H_INCLUDED
#define NEURALNETWORK_H_INCLUDED

#include <vector>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <omp.h>

class NeuralNetwork{

    public:
        NeuralNetwork(int layers, int numbOutputs, int layerUnits[]);
        void feedForwardPropagate(Eigen::VectorXd& x);
        int returnPrediction();
        void train(Eigen::MatrixXd& X, Eigen::MatrixXi& Y);
        void saveTheta();
        void importTheta();
        void trainingDataClassificationTest(Eigen::MatrixXd& X, Eigen::MatrixXi& Y);

    private:
        int L,K;
        std::vector<int> s;
        std::vector<Eigen::MatrixXd> Theta;
        std::vector<Eigen::VectorXd> a;

        double sigmoid( double x );
        void importFromOctave(std::string filename,Eigen::MatrixXd& dataStruct,int rows,int cols);

        double alpha0,alpha1,alpha2;
        double phi0,phi1,phi2;
        double phiPrime0,phiPrime1,phiPrime2;
        double eps, tol, alphaMax, stepMonitor, rho, denom, stepMonitorSafety;
        double secondDerivativeTest;
        int funcDimension, counter, cacheIndex;
        bool zoomGuard,quadInterpolationFailure, wolfeConditionFailure;
        bool maxStepSize, maxIterationGuard;
        Eigen::VectorXd position, alphaPosition, gradient;
        Eigen::MatrixXd H,I;
        Eigen::VectorXd p,ss,y;
        std::vector<Eigen::MatrixXd> Delta;
        void initializeOptimizer(double tolerance,double maxStepSize);
        void minimizeCostFunction(Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        double costFunction(Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        void testGradient(Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        void backPropagationGradient(Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        void setPositionFromTheta(Eigen::VectorXd& dummyPosition);
        void setThetaFromPosition(Eigen::VectorXd& dummyPosition);
        void setInitialH();
        void printStepMonitor();
        double alpha(Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        double phi(double& a,Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        double phiPrime(double& a,Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        double zoom(double alphaLow,double alphaHigh,double phiLow,double phiHigh,
                    double phiLowPrime,Eigen::MatrixXd& X,Eigen::MatrixXi& Y);
        void setAlphaJ(double& alphaj,double& alphaLow,double& alphaHigh,
                              double& phiLow,double& phiHigh,double& phiLowPrime);
        void printErrorReport();
        void printGradientComparision(Eigen::MatrixXd& X, Eigen::MatrixXi& Y);
        std::vector<Eigen::VectorXd> ssCache,yCache;

};

#endif // NEURALNETWORK_H_INCLUDED
