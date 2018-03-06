#include "Eyes.h"

#define MARIO_HAT_RED 16269424
#define STRIDE 2
#define DOUBLECHECK 1e-10
#define REF_TRIANGLE_SIZE 7
#define PIXEL_CHECK_RANGE 500

int Eyes::idx(int& x,int& y){

    return x + y * pixels.rows();

}

int Eyes::idxT(int x,int y){

    return x + y * pixels.rows();

}

Eyes::Eyes(){

    avgX = 0;
    stdDevX = 1.0;

    vEdgefilter.resize(5,5);
    hEdgefilter.resize(5,5);

    vEdgefilter << 1,1,0,-1,-1,
                   1,1,0,-1,-1,
                   1,1,0,-1,-1,
                   1,1,0,-1,-1,
                   1,1,0,-1,-1;

    hEdgefilter <<  1,-1, 1,-1, 1,
                   -1, 1,-1, 1,-1,
                    1,-1, 1,-1, 1,
                   -1, 1,-1, 1,-1,
                    1,-1, 1,-1, 1;

    display = XOpenDisplay(0);
    assert(display != NULL && "Error loading X11 Display");
    winRoot = XDefaultRootWindow(display);
    int revert;
    XGetInputFocus(display, &winFocus, &revert);

    XWindowAttributes attrib;

    XGetWindowAttributes(display,winFocus,&attrib);

    screenWidth = attrib.width;
    screenHeight = attrib.height;

    funcDimension = ((screenHeight-vEdgefilter.rows())/STRIDE + 1) * ((screenWidth-vEdgefilter.cols())/STRIDE + 1);

    std::cout << "Input size: " << funcDimension << std::endl;

    pixels.resize( screenWidth , screenHeight );

}

void Eyes::setXStatistics(double XAverage,double XStandardDeviation){

    avgX = XAverage;
    stdDevX = XStandardDeviation;

    marioHatRed = MARIO_HAT_RED - avgX;
    marioHatRed /= stdDevX;

    return;

}

void Eyes::setBackgroundColor(){

    lookScreen();

    backgroundColor = pixels( pixels.rows()/2 , pixels.cols()/2 );

    int misses = 0;
    int total = 0;

    for(int y=0;y<pixels.cols();y++) for(int x=0;x<pixels.rows();x++){
        if( !isApprox(pixels(x,y),backgroundColor) ) misses++;
        total++;
    }

    assert( ( 1.0 * misses ) / total < 0.02 );

    return;

}

void Eyes::setVerticalDisplacement(int& i){

    bool inBackground;

    if( isApprox( backgroundColor,pixels( refTriangleVert[i][0],refTriangleVert[i][1] ) ) ) inBackground = true;

    for(int j=0;j<PIXEL_CHECK_RANGE;j++){

        if( refTriangleVert[i][1] + j >= pixels.cols() ){ }

        else if( isApprox( backgroundColor,pixels( refTriangleVert[i][0],refTriangleVert[i][1] + j ) ) != inBackground ){

            if( refTriangleVertBackground[i] == inBackground ) j--;

            verticalDisplacement[i] = j;

            refTriangleVert[i][1] += j;

            return;

        }

        if( refTriangleVert[i][1] - j < 0 ){ }

        else if( isApprox( backgroundColor,pixels( refTriangleVert[i][0],refTriangleVert[i][1] - j ) ) != inBackground ){

            if( refTriangleVertBackground[i] == inBackground ) j--;

            verticalDisplacement[i] = -j;

            refTriangleVert[i][1] -= j;

            return;

        }

    }

    verticalDisplacement[i] = rand();

    return;

}

void Eyes::setHorizontalDisplacement(int& i){

    bool inBackground;

    if( isApprox( backgroundColor,pixels( refTriangleHori[i][0],refTriangleHori[i][1] ) ) ) inBackground = true;

    for(int j=0;j<PIXEL_CHECK_RANGE;j++){

        if( refTriangleHori[i][0] + j >= pixels.rows() ){ }

        else if( isApprox( backgroundColor,pixels( refTriangleHori[i][0] + j,refTriangleHori[i][1] ) ) != inBackground ){

            if( refTriangleHoriBackground[i] == inBackground ) j--;

            horizontalDisplacement[i] = j;

            refTriangleHori[i][0] += j;

            return;

        }

        if( refTriangleHori[i][0] - j < 0 ){ }

        else if( isApprox( backgroundColor,pixels( refTriangleHori[i][0] - j,refTriangleHori[i][1] ) ) != inBackground ){

            if( refTriangleHoriBackground[i] == inBackground ) j--;

            horizontalDisplacement[i] = -j;

            refTriangleHori[i][0] -= j;

            return;

        }

    }

    horizontalDisplacement[i] = rand();

    return;

}

void Eyes::updateReferenceTriangle(){

    for(int i=0;i<REF_TRIANGLE_SIZE;i++){

        setVerticalDisplacement(i);

        setHorizontalDisplacement(i);

    }

    for(int j=0;j<REF_TRIANGLE_SIZE;j++) std::cout << "D: " << horizontalDisplacement[j] << "\t" << verticalDisplacement[j] << std::endl;

    return;

}

void Eyes::initializeReferenceTriangle(){

    lookScreen();

    for(int i=0;i<REF_TRIANGLE_SIZE;i++){

        bool findingSolution = true;

        while(findingSolution){

            refTriangleVert[i][0] = rand() % pixels.rows();
            refTriangleVert[i][1] = rand() % pixels.cols();

            bool inBackground;
            if( isApprox( backgroundColor,pixels( refTriangleVert[i][0],refTriangleVert[i][1] ) ) ) inBackground = true;
            else inBackground = false;

            int direction = rand() % 2;

            while( true ){

                if( refTriangleVert[i][0] < 0 || refTriangleVert[i][0] >= pixels.rows() ) break;
                if( refTriangleVert[i][1] < 0 || refTriangleVert[i][1] >= pixels.cols() ) break;

                if( isApprox( backgroundColor,pixels( refTriangleVert[i][0],refTriangleVert[i][1] ) )  != inBackground ){

                    refTriangleVertBackground[i] = !inBackground;
                    findingSolution = false;
                    break;

                }

                if(direction == 0) refTriangleVert[i][1]++;
                if(direction == 1) refTriangleVert[i][1]--;

            }

        }

        findingSolution = true;

        while(findingSolution){

            refTriangleHori[i][0] = rand() % pixels.rows();
            refTriangleHori[i][1] = rand() % pixels.cols();

            bool inBackground;
            if( isApprox( backgroundColor,pixels( refTriangleHori[i][0],refTriangleHori[i][1] ) ) ) inBackground = true;
            else inBackground = false;

            int direction = rand() % 2;

            while( true ){

                if( refTriangleHori[i][0] < 0 || refTriangleHori[i][0] >= pixels.rows() ) break;
                if( refTriangleHori[i][1] < 0 || refTriangleHori[i][1] >= pixels.cols() ) break;

                if( isApprox( backgroundColor,pixels( refTriangleHori[i][0],refTriangleHori[i][1] ) )  != inBackground ){

                    refTriangleHoriBackground[i] = !inBackground;
                    findingSolution = false;
                    break;

                }

                if(direction == 0) refTriangleHori[i][0]++;
                if(direction == 1) refTriangleHori[i][0]--;

            }

        }

        std::cout << "Vertical: " << refTriangleVert[i][0] << "\t" << refTriangleVert[i][1] << "\t" << refTriangleVertBackground[i] << std::endl;
        std::cout << "Horizontal: " << refTriangleHori[i][0] << "\t" << refTriangleHori[i][1] << "\t" << refTriangleHoriBackground[i] << std::endl << std::endl;

        pixels(refTriangleVert[i][0],refTriangleVert[i][1]) = -3;
        pixels(refTriangleHori[i][0],refTriangleHori[i][1]) =  3;

        printLastSeen();

    }

    return;

}

void Eyes::findMario(){

    for(int y=0;y<pixels.cols();y++) for(int x=0;x<pixels.rows();x++){

        if( isApprox(marioHatRed,pixels(x,y)) ){   marioLoc[0] = x;  marioLoc[1] = y;  break;   }

    }

    return;

}

bool Eyes::isApprox(double& d1,double& d2){

    if( d1 > d2 - DOUBLECHECK && d1 < d2 + DOUBLECHECK ) return true;
    else return false;

}

void Eyes::printScreenSection(int xStart,int xEnd,int yStart, int yEnd){

    std::ofstream outfile("screenShot.dat");

    for(int y=yStart;y<yEnd;y++) for(int x=xStart;x<xEnd;x++) outfile << x << "\t" << y << "\t" << pixels( x,y ) << std::endl;

    outfile.close();

    return;

}

Eigen::MatrixXd Eyes::convolve(Eigen::MatrixXd& M,Eigen::MatrixXd& filter,int stride){

    Eigen::MatrixXd output( (M.rows()-filter.rows())/stride + 1, (M.cols()-filter.cols())/stride + 1 );
    for(int i=0;i<M.rows()-filter.rows()+1;i+=stride) for(int j=0;j<M.cols()-filter.cols()+1;j+=stride){

        output(i/stride,j/stride) = ( filter.array() * M.block(i,j,filter.rows(),filter.cols()).array() ).sum();

    }

    return output;

}


void Eyes::lookScreen(){

    pixels.resize( screenWidth , screenHeight );

    screenImage = *XGetImage(display,winFocus,0,0,screenWidth,screenHeight,XAllPlanes(),ZPixmap);

    for(int y=0;y<screenHeight;y++) for(int x=0;x<screenWidth;x++) pixels( x,y ) = (XGetPixel(&screenImage,x,screenHeight - y - 1));

    pixels = convolve(pixels,hEdgefilter,STRIDE);

    pixels = pixels.array() - avgX;

    pixels /= stdDevX;

    return;

}


void Eyes::printLastSeen(){

    std::ofstream outfile("screenShot.dat");

    for(int y=0;y<pixels.cols();y++) for(int x=0;x<pixels.rows();x++) outfile << x << "\t" << y << "\t" << pixels( x,y ) << std::endl;

    outfile.close();

    return;

}

void Eyes::printLastX(std::ofstream& outfile){

    for(int y=0;y<pixels.cols();y++) for(int x=0;x<pixels.rows();x++) outfile << pixels( x,y ) << "\t";

    return;

}

void Eyes::printXVector(Eigen::VectorXd X){

    std::ofstream outfile("screenShotX.dat");

    for(int y=0;y<pixels.cols();y++)
    for(int x=0;x<pixels.rows();x++) outfile << x << "\t"
                                    << y << "\t" << X( idx(x,y) ) << std::endl;

    outfile.close();

    return;
}

Eigen::VectorXd Eyes::returnVectorImage(){

    Eigen::VectorXd output(56388);

    for(int y=0;y<pixels.cols();y++) for(int x=0;x<pixels.rows();x++){

        output( idx(x,y) ) = pixels(x,y);

    }

    return output;

}

Eyes::~Eyes(){

    XCloseDisplay(display);

}
