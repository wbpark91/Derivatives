#ifndef _FDM_ENGINE_H_
#define _FDM_ENGINE_H_
#include "mesh.h"

class FDMEngine {
public:
    /* Constructors and destructor */
    FDMEngine();
    FDMEngine(double spot, double maturity,
                unsigned int imax, unsigned int jmax,
                double upper, double lower = 0.0);
    ~FDMEngine();

    /* calculate price by using FDM */
    virtual void calcPrice() = 0;

    /* set mBoundary = true if boundary is set up */
    void boundarySet();

    /* set functions */
    void setFirst(double first);
    void setSecond(double second);
    void setR(double r);
protected:
    Mesh mMesh;

    /* parameters */
    double first_, second_; // coefficient of first and second derivative
    double r_;
    double dt_, ds_;

    /* index of current spot */
    int spotIdx;

    /* market variables */
    double mSpot, mMaturity;

    /* FDM variables */
    unsigned int mImax, mJmax;
    double mUpper, mLower;

    /* Boundary set-up boolean */
    bool mBoundary;

    /* Price calculated boolean */
    bool mCalc;

    /* calculate value at (i, j) */
    virtual void value(int j, int i) = 0;
};

#endif
