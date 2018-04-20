#include "fdm_engine.h"
#include <iostream>

/* Constructors and destructor */
FDMEngine::FDMEngine() { }

FDMEngine::~FDMEngine() { }

FDMEngine::FDMEngine(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower)
            : mSpot(spot), mMaturity(maturity), mImax(imax), mJmax(jmax),
                mUpper(upper), mLower(lower) {
    mBoundary = false;
    mCalc = false;
    dt_ = maturity / imax;
    ds_ = (upper - lower) / jmax;

    /* initialize mesh */
    mMesh = Mesh(lower, upper, maturity, imax, jmax);

    mSpotIdx = mMesh.findIndex(spot, 0);
    if (mSpotIdx == -1) {
        std::cout << "Initialization error: cannot find spot." << std::endl;
        exit(1);
    }
}

double FDMEngine::delta() {
    if (mCalc != true) {
        calcPrice();
    }

    double upper = mMesh[mSpotIdx + 1][0];
    double lower = mMesh[mSpotIdx - 1][0];

    return (upper - lower) / (2 * ds_);
}

double FDMEngine::gamma() {
    if (mCalc != true) {
        calcPrice();
    }

    double upper = mMesh[mSpotIdx + 1][0];
    double lower = mMesh[mSpotIdx - 1][0];

    return (upper - 2 * mMesh[mSpotIdx][0] + lower) / (ds_ * ds_);
}

void FDMEngine::boundarySet() {
    mBoundary = true;
}

/* set functions */
void FDMEngine::setFirst(double first) { first_ = first; }
void FDMEngine::setSecond(double second) { second_ = second; }
void FDMEngine::setR(double r) { r_ = r; }

/* get functions */
Mesh& FDMEngine::getMesh() { return mMesh; }
Mesh FDMEngine::getMesh() const { return mMesh; }
int FDMEngine::getSpotIdx() const { return mSpotIdx; }
