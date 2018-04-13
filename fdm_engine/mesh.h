#ifndef _MESH_H_
#define _MESH_H_
#include <vector>
// #include "payoff.h"

class Mesh {
public:
    /* Constructor and Destructor */
    Mesh();
    Mesh(double lower, double upper, double maturity,
            unsigned int imax, unsigned int jmax);
    ~Mesh();

    /* operator overloading */
    std::vector<double>& operator[] (unsigned int j);

    /* Find a certain value at a specified time index */
    int findIndex(double value, unsigned int timeindex);

    void print();

    /* get upper-spot index and maturity index */
    int getImax();
    int getJmax();

private:
    double mLower, mUpper, mMaturity;
    unsigned int mImax, mJmax;
    double mPriceStep, mTimeStep;
    std::vector<std::vector<double>> mGrid;
};
#endif
