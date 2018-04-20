#ifndef _CONT_CPN_BUF_H_
#define _CONT_CPN_BUF_H_
#include "derivatives.h"
#include "mesh.h"
#include "payoff.h"
#include "forex.h"
#include "explicit_fdm_contcpn.h"
#include <vector>

class ContCpnBuf: public Derivative {
public:
    /* Constructors and Destructor */
    ContCpnBuf();
    ContCpnBuf(double principal, double coupon, double maturity, double barrier,
            double buffer, CpnFreq freq);
    ~ContCpnBuf();

    /* set functions */
    void setTrigger(bool trigger);
    void setForex(Forex forex);
    void setCorr(double corr);
    virtual void setMarketVariable(MarketVariable mktVar);

    /* pricing functions */
    virtual double bntprice(unsigned int steps, BinomialType bntType);
    double fdmprice(unsigned int imax, unsigned int jmax, FDMType fdmType,
                    double upper, double lower = 0.0);
    double fdmdelta();
    double fdmgamma();
private:
    double mPrincipal;
    double mCoupon;
    double mBarrier;
    double mBuffer;

    std::vector<double> mCpnSchedule;
    CpnFreq mFreq;
    bool mTrigger;

    /* correlation */
    double mCorr;

    /* Foreign exchange rate*/
    Forex mForex;

    /* Boundary setting function for FDM */
    void setBoundary(Mesh& mesh);

    /* FDM engine */
    FDMEngine* mFDMEngine;
};

class ContCpnBufPayoff: public Payoff {
public:
    /* Constructors and Destructor */
    ContCpnBufPayoff();
    ContCpnBufPayoff(double principal, double barrier,
        double coupon, double buffer, CpnFreq freq);
    ~ContCpnBufPayoff();

    virtual double operator()(double s);

    /* set functions */
    virtual void setTrigger(bool trigger);
    virtual void setSpot(double spot);
private:
    double mSpot;
    double mPrincipal;
    double mBarrier;
    double mCoupon;
    double mBuffer;

    CpnFreq mFreq;

    bool mTrigger;

    /* set boundary function */
    void setBoundary(Mesh& mesh);
};

#endif
