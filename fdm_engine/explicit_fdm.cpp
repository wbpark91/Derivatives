#include "explicit_fdm.h"
#include <iostream>

/* Constructors and destructor */
ExplicitFDM::ExplicitFDM() { }

ExplicitFDM::~ExplicitFDM() { }

ExplicitFDM::ExplicitFDM(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower)
            :FDMEngine(spot, maturity, imax, jmax, upper, lower) { }

void ExplicitFDM::value(int j, int i) {
    double a = (0.5 * (second_ * second_) * (j * j) + 0.5 * first_ * j) * dt_;
    double b = 1 - (second_ * second_) * (j * j) * dt_;
    double c = (0.5 * (second_ * second_) * (j * j) - 0.5 * first_ * j) * dt_;
    mMesh[j][i] = (a * mMesh[j+1][i+1] + b * mMesh[j][i+1] + c * mMesh[j-1][i+1]) / (1 + r_ * dt_);
}

void ExplicitFDM::calcPrice() {
    if (mBoundary == false) {
        std::cout << "Boundary is not set yet." << std::endl;
        exit(1);
    }

    for (int i = mImax - 1; i >= 0; --i) {
        for (int j = 1; j <= mJmax - 1; ++j) {
            value(j, i);
        }
    }
    mCalc = true;
}
