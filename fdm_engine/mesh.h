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

    /* print mesh */
    void print();

    /* get imax, jmax, dt, ds */
    int getImax() const;
    int getJmax() const;
    double getdt() const;
    double getds() const;

private:
    double mLower, mUpper, mMaturity;
    unsigned int mImax, mJmax;
    double mPriceStep, mTimeStep;
    std::vector<std::vector<double>> mGrid;
};
#endif
