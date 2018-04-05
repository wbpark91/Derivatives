#include "mesh.h"
#include <algorithm>

Mesh::Mesh(double lower, double upper, double maturity,
            unsigned int imax, unsigned int jmax)
            :mLower(lower), mUpper(upper), mMaturity(maturity),
                mImax(imax), mJmax(jmax) {
    double ds = (upper - lower) / jmax;

    for (int i = 0; i < imax; ++i)
        for (int j = 0; j < jmax; ++j)
            mGrid[j][i] = lower + j * ds;
}

Mesh::~Mesh() { }

Mesh::Mesh() { }

double& Mesh::operator[][] (unsigned int j, unsigned int i) {
    return mGrid[j][i];
}

int Mesh::findIndex(double value, unsigned int timeindex) {
    for (int j = 0; j < mGrid[j].size(); ++j) {
        if (value == mGrid[j][timeindex])
            return j;
    }
    return -1;
}
