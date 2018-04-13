#include "fdm_engine.h"

/* Constructors and destructor */
FDMEngine::FDMEngine() { }

FDMEngine::~FDMEngine() { }

FDMEngine::FDMengine(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower = 0.0)
            : mSpot(spot), mMaturity(maturity), mImax(imax), mJmax(jmax),
                mUpper(upper), mLower(lower) {
    mBoundary = false;
    dt_ = maturity / imax;

    /* initialize mesh */
    mMesh = Mesh(lower, upper, maturity, imax, jmax);
}

void FDMEngine::boundarySet() {
    mBoundary = true;
}

/* set functions */
void FDMEngine::setA(double a) { a_ = a; }
void FDMEngine::setB(double b) { b_ = b; }
void FDMEngine::setC(double c) { c_ = c; }
void FDMEngine::setR(double r) { r_ = r; }
