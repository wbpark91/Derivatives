#include "mesh.h"
#include <algorithm>
#include <iostream>

Mesh::Mesh(double lower, double upper, double maturity,
            unsigned int imax, unsigned int jmax)
            :mLower(lower), mUpper(upper), mMaturity(maturity),
                mImax(imax), mJmax(jmax) {
    mTimeStep = maturity / imax;
    mPriceStep = (upper - lower) / jmax;

    /* number of grid = (imax + 1) * (jmax + 1) */

    for (int j = 0; j <= jmax; ++j) {
        std::vector<double> temp;
        for (int i = 0; i <= imax; ++i) {
            temp.push_back(lower + j * mPriceStep);
        }
        mGrid.push_back(temp);
    }
}

Mesh::~Mesh() { }

Mesh::Mesh() { }

std::vector<double>& Mesh::operator[] (unsigned int j) {
    return mGrid[j];
}

int Mesh::findIndex(double value, unsigned int timeindex) {
    for (int j = 0; j < mGrid[j].size(); ++j) {
        if (value == mGrid[j][timeindex])
            return j;
    }
    return -1;
}

/* get functions */
int Mesh::getImax() const {
    return mImax;
}

int Mesh::getJmax() const {
    return mJmax;
}

double Mesh::getdt() const {
    return mTimeStep;
}

double Mesh::getds() const {
    return mPriceStep;
}

void Mesh::print() {
    std::cout << "  ";
    for (int i = 0; i < mGrid[0].size(); ++i) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "  ";
    for (int i = 0; i < mGrid[0].size(); ++i) {
        std::cout << "--";
    }
    std::cout << std::endl;

    for (int i = 0; i < mGrid.size(); ++i) {
        std::cout << i << "|";
        for (int j = 0; j < mGrid[i].size(); ++j) {
            std::cout << mGrid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
