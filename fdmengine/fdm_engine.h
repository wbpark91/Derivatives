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
    virtual ~FDMEngine();

    /* calculate price by using FDM */
    virtual void calcPrice() = 0;
    double delta();
    double gamma();

    /* set mBoundary = true if boundary is set up */
    void boundarySet();

    /* set functions */
    void setFirst(double first);
    void setSecond(double second);
    void setR(double r);
    virtual void setCpnIdx(std::vector<double> cpnSchedule) { }
    virtual void setCoupon(double coupon) { }
    /* get functions */
    Mesh& getMesh();
    Mesh getMesh() const;
    int getSpotIdx() const;

protected:
    Mesh mMesh;

    /* parameters */
    double first_, second_; // coefficient of first and second derivative
    double r_;
    double dt_, ds_;

    /* index of current spot */
    int mSpotIdx;

    /* market variables */
    double mSpot, mMaturity;

    /* FDM variables */
    unsigned int mImax, mJmax;
    double mUpper, mLower;

    /* Boundary set-up boolean */
    bool mBoundary;

    /* Price calculated boolean */
    bool mCalc;
};

#endif
