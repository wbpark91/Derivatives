#include "fdm_engine.h"
#include <iostream>

/* Constructors and destructor */
FDMEngine::FDMEngine() { }

FDMEngine::~FDMEngine() { }

FDMEngine::FDMengine(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower = 0.0)
            : mSpot(spot), mMaturity(maturity), mImax(imax), mJmax(jmax),
                mUpper(upper), mLower(lower) {
    mBoundary = false;
    mCalc = false;
    dt_ = maturity / imax;
    ds_ = (upper - lower) / jmax;

    /* initialize mesh */
    mMesh = Mesh(lower, upper, maturity, imax, jmax);

    spotIdx = mMesh.findIndex(spot, 0);

    if (spotIdx == -1) {
        std::cout << "Initialization error: cannot find spot." << std::endl;
        exit(1);
    }
}

void FDMEngine::boundarySet() {
    mBoundary = true;
}

/* set functions */
void FDMEngine::setFirst(double first) { first_ = first; }
void FDMEngine::setSecond(double second) { second_ = second; }
void FDMEngine::setR(double r) { r_ = r; }
