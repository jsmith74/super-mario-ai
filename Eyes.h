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

#define REF_TRIANGLE_SIZE 7

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
        void updateReferenceTriangle();
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
        int marioLocOld[2];

        bool refTriangleVertBackground[REF_TRIANGLE_SIZE];
        bool refTriangleHoriBackground[REF_TRIANGLE_SIZE];
        int refTriangleVert[REF_TRIANGLE_SIZE][2];
        int refTriangleHori[REF_TRIANGLE_SIZE][2];
        int horizontalDisplacement[REF_TRIANGLE_SIZE];
        int verticalDisplacement[REF_TRIANGLE_SIZE];

        bool isApprox(double& d1,double& d2);
        void setVerticalDisplacement(int& i);
        void setHorizontalDisplacement(int& i);

};

#endif // EYES_H_INCLUDED
