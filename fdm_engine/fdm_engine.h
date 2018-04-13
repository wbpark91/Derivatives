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
    void setA(double a);
    void setB(double b);
    void setC(double c);
    void setR(double r);
protected:
    Mesh mMesh;

    /* parameters */
    double a_, b_, c_;
    double r_;
    double dt_;

    /* market variables */
    double mSpot, mMaturity;

    /* FDM variables */
    unsigned int mImax, mJmax;
    double mUpper, mLower;

    /* Boundary set-up boolean */
    bool mBoundary;

    /* calculate value at (i, j) */
    virtual void value(int i, int j) = 0;
};

#endif
