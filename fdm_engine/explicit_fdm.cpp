#include "explicit_fdm.h"

/* Constructors and destructor */
ExplicitFDM::ExplicitFDM() { }

ExplicitFDM::~ExplicitFDM() { }

ExplicitFDM::ExplicitFDM(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower)
            :FDMEngine(spot, maturity, imax, jmax, upper, lower) { }

void ExplicitFDM::value(int j, int i) {
    mMesh[j][i] = (a_ * mMesh[j+1][i+1] + b_ * mMesh[j][i+1] + c_ * mMesh[j-1][i+1]) / (1 + r_ * dt_);
}



/* Calculate A, B, C */
void ExplicitFDM::calcA(int j) {
    a_ = (0.5 * (second_ * second_) * (j * j) + 0.5 * first_ * j) * dt_;
}

void ExplicitFDM::calcB(int j) {
    b_ = 1 - (second_ * second_) * (j * j) * dt_;
}

void ExplicitFDM::calcC(int j) {
    c_ = (0.5 * (second_ * second_) * (j * j) - 0.5 * first_ * j) * dt_;
}
