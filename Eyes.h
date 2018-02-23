#ifndef EYES_H_INCLUDED
#define EYES_H_INCLUDED

//#include <unsupported/Eigen/CXX11/Tensor>
#include <Eigen/Dense>
#include <X11/Xutil.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#define REF_TRIANGLE_SIZE 5

class Eyes{

    public:
        Eyes();
        ~Eyes();

        void lookScreen();
        void printLastSeen();
        void findMario();
        void printScreenSection(int xStart,int xEnd,int yStart,int yEnd);
        void printLastX(std::ofstream& outfile);
        void printXVector(Eigen::VectorXd X);
        void setXStatistics(double XAverage,double XStandardDeviation);
        void setBackgroundColor();
        void initializeReferenceTriangle();
        Eigen::VectorXd returnVectorImage();

    private:
        Display* display;
        Window winFocus;
        Window winRoot;
        XImage screenImage;

        Eigen::MatrixXd pixels;

        int idx(int& x,int& y);
        int idxT(int x,int y);

        int screenWidth,screenHeight, funcDimension;
        Eigen::MatrixXd vEdgefilter, hEdgefilter;

        Eigen::MatrixXd convolve(Eigen::MatrixXd& M,Eigen::MatrixXd& filter,int stride);

        double avgX, stdDevX, marioHatRed, backgroundColor;
        int marioLoc[2];

        int refTriangle[REF_TRIANGLE_SIZE][2];

        bool isApprox(double& d1,double& d2);

};

#endif // EYES_H_INCLUDED
