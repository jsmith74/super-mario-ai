#include "NeuralNetwork.h"

/** GRADIENT CHECKING OPTIONS ======================= */
#define MACHINEPRECISION 1.1e-16
#define FORWARD_DIFFERENCE
//#define CENTRAL_DIFFERENCE

/** STARTING WEIGHTS AND REGULARIZATION OPTIONS ===== */
#define SCALING_FACTOR 1.0
#define LAMBDA 1.0
#define RANDOMIZE_STARTING_THETA

/** MINIMIZER OPTIONS =============================== */
#define PRINT_STEP_MONITOR
#define MAX_ITERATIONS 6000
#define ZOOM_GUARD 50
#define C1 1e-4
#define C2 0.9
#define TOLERANCE 0.01
#define MAX_STEP_SIZE 20.0
#define OPT_WARNING_MESSAGE
#define ENABLE_L_BFGS
#define L_BFGS_STORAGE_SIZE 250     // OPTIMAL STORAGE SIZE IS PROBABLY PROBLEM INDEPENDENT
//#define ENABLE_BFGS
//#define TEST_GRADIENT

NeuralNetwork::NeuralNetwork(int layers, int numbOutputs, int layerUnits[]){

    L = layers;
    K = numbOutputs;
    s.resize(L);
    for(int i=0;i<L;i++) s[i] = layerUnits[i];

    Theta.resize(L-1);
    Delta.resize(L-1);

    for(int i=0;i<L-1;i++) Theta[i].resize( s[i+1],s[i]+1 );
    for(int i=0;i<L-1;i++) Delta[i].resize( s[i+1],s[i]+1 );

    a.resize(L);

    for(int i=0;i<L-1;i++) a[i].resize( s[i] + 1 );

    a[L-1].resize( s[L-1] );

    for(int i=0;i<L-1;i++) a[i](0) = 1;

}

void NeuralNetwork::feedForwardPropagate(Eigen::VectorXd& x){

    a[0].segment( 1,s[0] ) = x;

    for(int l=1;l<L-1;l++){

        Eigen::VectorXd z = Theta[l-1] * a[l-1];

        for(int i=0;i<s[l];i++) a[l](i+1) = sigmoid( z(i) );

    }

    a[L-1] = Theta[L-2] * a[L-2];

    for(int i=0;i<s[L-1];i++) a[L-1](i) = sigmoid( a[L-1](i) );

    return;

}

void NeuralNetwork::train(Eigen::MatrixXd& X, Eigen::MatrixXi& Y){

    initializeOptimizer(TOLERANCE,MAX_STEP_SIZE);

    std::cout << "Optimization Function Dimension: " << funcDimension << std::endl;

    minimizeCostFunction(X,Y);

    return;

}

void NeuralNetwork::ClassificationTest(Eigen::MatrixXd& X, Eigen::MatrixXi& Y){

    std::ofstream outfile("classificationTest.txt");

    int misses = 0;

    for(int i=0;i<Y.cols();i++){

        Eigen::VectorXd XTest = X.col(i);

        feedForwardPropagate(XTest);

        int NNResult = returnPrediction();

        outfile << i << "\t" << Y(0,i) << "\t" << NNResult << std::endl;

        if( Y(0,i) != NNResult ) misses++;

    }

    outfile.close();

    double failRate = ( 1.0 * misses )/( 1.0 * Y.cols() );

    std::cout << "Pass rate: " << ( 1.0 - failRate ) * 100 << "%" << std::endl;

    return;

}

void NeuralNetwork::saveTheta(){

    std::ofstream outfile("ThetaSaved.dat");

    for(int l=0;l<L-1;l++){

        for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            outfile << std::setprecision(16) << Theta[l](i,j) << "\t";

        }

    }

    outfile << std::endl;

    outfile.close();

    return;

}

void NeuralNetwork::importTheta(){

    std::ifstream infile("ThetaSaved.dat");

    if( !infile.is_open() ){

        std::cout << "ERROR: could not load saved network weights" << std::endl << std::endl;

        assert( false );

    }

    for(int l=0;l<L-1;l++){

        for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            infile >> Theta[l](i,j);

        }

    }

    infile.close();

    return;

}

void NeuralNetwork::setPositionFromTheta(Eigen::VectorXd& dummyPosition){

    int k =0;

    for(int l=0;l<L-1;l++){

        for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            dummyPosition(k) = Theta[l](i,j);
            k++;

        }

    }

    return;

}

void NeuralNetwork::setThetaFromPosition(Eigen::VectorXd& dummyPosition){

    int k =0;

    for(int l=0;l<L-1;l++){

        for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            Theta[l](i,j) = dummyPosition(k);
            k++;

        }

    }

    return;

}

void NeuralNetwork::minimizeCostFunction(Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    double startTime = omp_get_wtime();

    /** ==== EXPERIMENT WITH USING MAPS INSTEAD OF COPYING DATA =============== */

    #ifdef RANDOMIZE_STARTING_THETA

        for(int i=0;i<L-1;i++) Theta[i] = SCALING_FACTOR * Eigen::MatrixXd::Random( s[i+1],s[i]+1 );

    #endif // RANDOMIZE_STARTING_THETA

    #ifndef RANDOMIZE_STARTING_THETA

        importTheta();

    #endif // RANDOMIZE_STARTING_THETA

    backPropagationGradient(X,Y);

    #ifdef TEST_GRADIENT

        printGradientComparision(X,Y);
        assert( false );

    #endif // TEST_GRADIENT

    stepMonitor = costFunction(X,Y);

    setPositionFromTheta(position);

    #ifdef ENABLE_BFGS

        setInitialH();

        I = Eigen::MatrixXd::Identity(funcDimension,funcDimension);

    #endif // ENABLE_BFGS

    alphaPosition.resize(funcDimension);

    counter = 0;

    zoomGuard = false;

    quadInterpolationFailure = false;

    wolfeConditionFailure = false;

    maxStepSize = false;

    maxIterationGuard = false;

    #ifdef ENABLE_L_BFGS

        cacheIndex = 0;
        yCache.resize(L_BFGS_STORAGE_SIZE);
        ssCache.resize(L_BFGS_STORAGE_SIZE);
        for(int i=0;i<L_BFGS_STORAGE_SIZE;i++) yCache[i] = Eigen::VectorXd::Zero(funcDimension);
        for(int i=0;i<L_BFGS_STORAGE_SIZE;i++) ssCache[i] = Eigen::VectorXd::Zero(funcDimension);
        double alphaLBFGS[L_BFGS_STORAGE_SIZE];
        double rhoLBFGS[L_BFGS_STORAGE_SIZE];
        double gammaLBFGS[L_BFGS_STORAGE_SIZE];
        for(int i=0;i<L_BFGS_STORAGE_SIZE;i++) alphaLBFGS[i] = 0;
        for(int i=0;i<L_BFGS_STORAGE_SIZE;i++) rhoLBFGS[i] = 0;
        for(int i=0;i<L_BFGS_STORAGE_SIZE;i++) gammaLBFGS[i] = 1;

    #endif // ENABLE_L_BFGS

    while(true){

        printStepMonitor();
        counter++;

        if(gradient.norm() < tol) break;

        if(counter > MAX_ITERATIONS) {

            maxIterationGuard = true;
            break;

        }

        p = -gradient;

        #ifdef ENABLE_L_BFGS

            p *= -1;

            for(int k=0;k<L_BFGS_STORAGE_SIZE;k++){

                int i = cacheIndex - 1 - k;

                if(i<0) i = i + L_BFGS_STORAGE_SIZE;

                alphaLBFGS[i] = rhoLBFGS[i] * ssCache[i].transpose() * p;
                p -= alphaLBFGS[i] * yCache[i];


            }

            p *= gammaLBFGS[cacheIndex];

            for(int k=0;k<L_BFGS_STORAGE_SIZE;k++){

                int i = (cacheIndex + k) % L_BFGS_STORAGE_SIZE;

                double beta = rhoLBFGS[i] * yCache[i].transpose() * p;
                p += (alphaLBFGS[i] - beta) * ssCache[i];

            }

            ssCache[cacheIndex] = -position;
            yCache[cacheIndex] = -gradient;

            p *= -1;


        #endif // ENABLE_L_BFGS

        #ifdef ENABLE_BFGS

            p = H * p;

            ss = -position;

            y = -gradient;

        #endif // ENABLE_BFGS

        position += alpha(X,Y) * p;

        setThetaFromPosition(position);

        stepMonitor =  costFunction(X,Y);

        //if(isnan(stepMonitor)) break;
        //else stepMonitorSafety = stepMonitor;

        backPropagationGradient(X,Y);

        #ifdef ENABLE_L_BFGS

            ssCache[cacheIndex] += position;
            yCache[cacheIndex] += gradient;

            denom = yCache[cacheIndex].transpose() * ssCache[cacheIndex];
            if(denom==0) assert( false );
            rhoLBFGS[cacheIndex] = 1.0/denom;

            gammaLBFGS[ (cacheIndex + 1) % L_BFGS_STORAGE_SIZE ] = ssCache[cacheIndex].transpose() * yCache[cacheIndex];

            gammaLBFGS[ (cacheIndex + 1) % L_BFGS_STORAGE_SIZE ] /= yCache[cacheIndex].transpose() * yCache[cacheIndex];

            cacheIndex = (cacheIndex + 1) % L_BFGS_STORAGE_SIZE;

        #endif // ENABLE_L_BFGS

        #ifdef ENABLE_BFGS

            ss += position;

            y += gradient;

            denom = y.transpose() * ss;

            if(denom == 0.0) break;

            rho = 1.0 / denom;

            H = (I - rho * ss * y.transpose()) * H * (I - rho * y * ss.transpose()) + rho * ss * ss.transpose();

        #endif // ENABLE_BFGS

    }

    printErrorReport();

    double endTime = omp_get_wtime();

    std::ofstream outfile("ConvergenceTest.dat",std::ofstream::app);

    outfile << L_BFGS_STORAGE_SIZE << "\t" << counter << "\t" << endTime - startTime << std::endl;

    outfile.close();

    return;

}

void NeuralNetwork::initializeOptimizer(double tolerance,double maxStepSize){

    eps = sqrt(MACHINEPRECISION);

    tol = tolerance;

    alphaMax = maxStepSize;

    funcDimension = 0;

    for(int l=0;l<L-1;l++) funcDimension += Theta[l].rows() * Theta[l].cols();

    position.resize(funcDimension);
    gradient.resize(funcDimension);

    return;

}


void NeuralNetwork::printGradientComparision(Eigen::MatrixXd& X, Eigen::MatrixXi& Y){

    std::ofstream outfile("backpropGradient.txt");

    for(int i=0;i<gradient.size();i++) outfile << i << "\t" << std::setprecision(16) << gradient(i) << std::endl;

    outfile.close();

    testGradient(X,Y);

    outfile.open("finiteDifferenceGradient.txt");

    for(int i=0;i<gradient.size();i++) outfile << i << "\t" << std::setprecision(16) << gradient(i) << std::endl;

    outfile.close();

    return;

}


void NeuralNetwork::backPropagationGradient(Eigen::MatrixXd& X, Eigen::MatrixXi& Y){

    /** EXPERIMENT WITH USING MAPS INSTEAD OF COPYING DATA ==== */

    for(int l=0;l<L-1;l++) Delta[l] = Eigen::MatrixXd::Zero( s[l+1],s[l]+1 );

    for(int t=0;t<X.cols();t++){

        Eigen::VectorXd XTest = X.col(t);
        feedForwardPropagate( XTest );

        std::vector<Eigen::VectorXd> delta(L);

        delta[L-1] = a[L-1];
        delta[L-1]( Y(0,t) ) -= 1;

        for(int l=L-2;l>=1;l--){

            if( Theta[l].rows() == delta[l+1].size() ){
                delta[l] = ( Theta[l].transpose() * delta[l+1] ).array() * a[l].array() * (1-a[l].array());
            }
            else{
                delta[l] = ( Theta[l].transpose() * delta[l+1].segment(1,delta[l+1].size()-1) ).array() * a[l].array() * (1-a[l].array());
            }

        }

        for(int l=0;l<L-2;l++) Delta[l] += delta[l+1].segment(1,s[l+1]) * a[l].transpose();

        Delta[L-2] += delta[L-1] * a[L-2].transpose();

    }

    int k =0;

    for(int l=0;l<L-1;l++){

        for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            gradient(k) = Delta[l](i,j);
            if(j != 0) gradient(k) += LAMBDA * Theta[l](i,j);
            k++;

        }

    }

    gradient *= ( 1.0/X.cols() );

    return;

}

double NeuralNetwork::costFunction(Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    /** EXPERIMENT WITH USING MAPS INSTEAD OF COPYING DATA ==== */

    double output = 0.0;

    for(int l=0;l<L-1;l++){

        for(int j=1;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

            output -= Theta[l](i,j) * Theta[l](i,j);

        }

    }

    output *= 0.5 * LAMBDA;

    for(int i=0;i<X.cols();i++){

        Eigen::VectorXd XTest = X.col(i);
        feedForwardPropagate( XTest );

        for(int k=0;k<K;k++){

            if( k == Y(0,i) ) output += std::log( a[L-1](k) + 1e-15 );
            else output += std::log( 1 - a[L-1](k) + 1e-15 );

            //NEED TO REWRITE THIS

        }

    }

    output *= -1.0/( X.cols() );

    return output;

}

void NeuralNetwork::printStepMonitor(){

    #ifdef PRINT_STEP_MONITOR

        std::cout << counter << "\t" << stepMonitor << "\t" << gradient.norm() << "\t" << rho << std::endl;

    #endif // PRINT_STEP_MONITOR

    return;
}

void NeuralNetwork::setInitialH(){

    H = (1.0/(gradient.norm())) * Eigen::MatrixXd::Identity(funcDimension,funcDimension);

    return;
}

void NeuralNetwork::testGradient(Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    #ifdef FORWARD_DIFFERENCE

        stepMonitor = costFunction(X,Y);

        int k = 0;

        for(int l=0;l<L-1;l++){

            for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

                gradient(k) = -stepMonitor;
                Theta[l](i,j) += eps;
                gradient(k) += costFunction(X,Y);
                Theta[l](i,j) -= eps;
                gradient(k) /= eps;
                k++;

                //if(k>20) return;

            }

        }

    #endif // FORWARD_DIFFERENCE


    #ifdef CENTRAL_DIFFERENCE

        int k = 0;

        for(int l=0;l<L-1;l++){

            for(int j=0;j<s[l]+1;j++) for(int i=0;i<s[l+1];i++){

                Theta[l](i,j) += eps;
                gradient(k) = costFunction(X,Y);
                Theta[l](i,j) -= 2*eps;
                gradient(k) -= costFunction(X,Y);
                Theta[l](i,j) += eps;
                gradient(k) /= 2*eps;
                k++;

                //if(k>20) return;

            }

        }

    #endif // CENTRAL_DIFFERENCE

    return;

}

int NeuralNetwork::returnPrediction(){

    int location;

    a[L-1].maxCoeff(&location);

    return location;


}


double NeuralNetwork::sigmoid( double x ){

    return 1.0 / ( 1.0 + std::exp(-x) );

}

void NeuralNetwork::importFromOctave(std::string filename,Eigen::MatrixXd& dataStruct,int rows,int cols){

    std::ifstream infile(filename.c_str());

    for(int i=0;i<rows;i++) for(int j=0;j<cols;j++){

        infile >> dataStruct(i,j);

    }

    infile.close();

    return;

}

double NeuralNetwork::phi(double& a,Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    alphaPosition = position + a * p;

    setThetaFromPosition(alphaPosition);

    return costFunction(X,Y);

}

double NeuralNetwork::phiPrime(double& a,Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    #ifdef FORWARD_DIFFERENCE

        double aPeps = a + eps;
        return (phi(aPeps,X,Y)-phi(a,X,Y))/eps;

    #endif // FORWARD_DIFFERENCE

    #ifdef CENTRAL_DIFFERENCE

        double aPeps = a + eps;
        double aMeps = a - eps;
        return (phi(aPeps,X,Y)-phi(aMeps,X,Y))/(2.0*eps);

    #endif // CENTRAL_DIFFERENCE

}

double NeuralNetwork::alpha(Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    alpha0 = 0.0;
    alpha1 = 1.0;
    alpha2 = alphaMax;

    phi0 = stepMonitor;
    phiPrime0 = gradient.transpose() * p;

    #ifdef PRINT_STEP_MONITOR

        if(phiPrime0 > 0.0){

            std::cout << "Positive directional derivative in line search..." << std::endl;

        }

    #endif // PRINT_STEP_MONITOR

    phi1 = phi(alpha1,X,Y);

    if(phi1 > phi0 + C1 * alpha1 * phiPrime0){

        return zoom(alpha0,alpha1,phi0,phi1,phiPrime0,X,Y);

    }

    phiPrime1 = phiPrime(alpha1,X,Y);

    if(std::abs(phiPrime1) <= -C2 * phiPrime0){

        return alpha1;

    }

    if(phiPrime1 >= 0){

        return zoom(alpha1,alpha0,phi1,phi0,phiPrime0,X,Y);

    }

    phi2 = phi(alpha2,X,Y);

    if(phi2 > phi0 + C1 * alpha2 * phiPrime0 || phi2 >= phi1){

        return zoom(alpha1,alpha2,phi1,phi2,phiPrime1,X,Y);

    }

    phiPrime2 = phiPrime(alpha2,X,Y);

    if(std::abs(phiPrime2) <= -C2 * phiPrime0){

        return alpha2;

    }

    if(phiPrime2 >= 0){

        return zoom(alpha2,alpha1,phi2,phi1,phiPrime1,X,Y);

    }

    #ifdef PRINT_STEP_MONITOR

        std::cout << "This iteration of BFGS has taken the maximum step size." << std::endl;

    #endif // PRINT_STEP_MONITOR

    maxStepSize = true;

    return alpha2;

}


double NeuralNetwork::zoom(double alphaLow,double alphaHigh,double phiLow,double phiHigh,
                           double phiLowPrime,Eigen::MatrixXd& X,Eigen::MatrixXi& Y){

    double alphaj,phij,phiPrimej;

    #ifdef PRINT_STEP_MONITOR

        std::cout << "Zooming into interval...\n";
        std::cout << alphaLow << "\t" << alphaHigh << std::endl;

    #endif // PRINT_STEP_MONITOR

    int zoomCounter = 0;

    while(true){

        zoomCounter++;

        if(zoomCounter > ZOOM_GUARD) {

            zoomGuard = true;
            return alphaj;

        }

        setAlphaJ(alphaj,alphaLow,alphaHigh,phiLow,phiHigh,phiLowPrime);

        if(secondDerivativeTest < 0.0){

            quadInterpolationFailure = true;

            if(phiHigh < phi0 + C1 * alphaHigh * phiPrime0){

                return alphaHigh;

            }

            else if(phiLow < phi0 + C1 * alphaLow * phiPrime0){

                return alphaLow;

            }

            else{

                wolfeConditionFailure = true;

                return alphaHigh;

            }

        }


        phij = phi(alphaj,X,Y);

        #ifdef PRINT_STEP_MONITOR

            std::cout << "\t\t\t\t" <<  phij << "\t" << phiLowPrime  << ": \t" << alphaj << "\t" << alphaLow << "\t" << alphaHigh << "\t" << phiLow << "\t" << phiHigh << std::endl;

        #endif // PRINT_STEP_MONITOR

        if(phij > phi0 + C1 * alphaj * phiPrime0 || phij >= phiLow){

            alphaHigh = alphaj;
            phiHigh = phij;

        }

        else{

            phiPrimej = phiPrime(alphaj,X,Y);



            if(std::abs(phiPrimej) <= -C2 * phiPrime0){

                return alphaj;

            }

            if(std::abs(phiPrimej) <= C2 * phiPrime0 && phiPrime0 >0){

                #ifdef PRINT_STEP_MONITOR

                    std::cout << "Unexpected Zoom Ending Condition... " << std::endl;

                #endif // PRINT_STEP_MONITOR

                return alphaj;

            }

            if(phiPrimej * (alphaHigh - alphaLow) >= 0.0){

                alphaHigh = alphaLow;
                phiHigh = phiLow;

            }

            alphaLow = alphaj;

            phiLow = phij;

        }

        if(alphaHigh < alphaLow){

            phiLowPrime = phiPrime(alphaHigh,X,Y);

        }

        else{

            phiLowPrime = phiPrime(alphaLow,X,Y);

        }

    }

}


void NeuralNetwork::setAlphaJ(double& alphaj,double& alphaLow,double& alphaHigh,
                              double& phiLow,double& phiHigh,double& phiLowPrime){

    if(alphaLow < alphaHigh){

        alphaj = alphaHigh * alphaHigh * phiLowPrime - alphaLow * (2.0 * phiHigh - 2.0 * phiLow + alphaLow * phiLowPrime);

        if((-phiHigh + phiLow + (alphaHigh - alphaLow) * phiLowPrime) != 0.0){

            alphaj /= 2.0 * (-phiHigh + phiLow + (alphaHigh - alphaLow) * phiLowPrime);

        }

        else assert(alphaj == 0.0);

        secondDerivativeTest = phiHigh - phiLow + (alphaLow-alphaHigh) * phiLowPrime;

        if((alphaHigh-alphaLow) * (alphaHigh-alphaLow) != 0.0){

            secondDerivativeTest /= (alphaHigh-alphaLow) * (alphaHigh-alphaLow);

        }

        else assert(std::abs(secondDerivativeTest) <= 100 * MACHINEPRECISION);

    }

    else{

        alphaj = alphaLow * alphaLow * phiLowPrime - alphaHigh * (2.0 * phiLow - 2.0 * phiHigh + alphaHigh * phiLowPrime);

        if((-phiLow + phiHigh + (alphaLow - alphaHigh) * phiLowPrime) != 0.0){

            alphaj /= 2.0 * (-phiLow + phiHigh + (alphaLow - alphaHigh) * phiLowPrime);

        }

        else assert(alphaj == 0.0);

        secondDerivativeTest = phiLow - phiHigh + (alphaHigh-alphaLow) * phiLowPrime;

        if((alphaLow-alphaHigh) * (alphaLow-alphaHigh) != 0.0){

            secondDerivativeTest /= (alphaLow-alphaHigh) * (alphaLow-alphaHigh);

        }

        else assert(std::abs(secondDerivativeTest) <= 100 * MACHINEPRECISION);

    }


    return;

}


void NeuralNetwork::printErrorReport(){

    #ifdef OPT_WARNING_MESSAGE

        if(zoomGuard) std::cout << "Warning: Zoom Guard Was Hit..." << std::endl  <<  std::endl;

        if(quadInterpolationFailure) std::cout << "Warning: Quadratic Interpolation Failure..." << std::endl << std::endl;

        if(wolfeConditionFailure) std::cout << "Wolfe Condition failure on a step (should be rare)..." << std::endl << std::endl;

        if(maxStepSize) std::cout << "BFGS took the maximum step size for at least one iteration..." << std::endl << std::endl;

        if(maxIterationGuard) std::cout << "BFGS has taken the maximum number of steps imposed by the user..." << std::endl << std::endl;

    #endif // OPT_WARNING_MESSAGE

    return;

}
