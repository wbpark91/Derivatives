#ifndef _CN_FDM_CONTCPN_H_
#define _CN_FDM_CONTCPN_H_
#include "fdm_engine.h"
#include "derivatives.h"

class CrankNicolsonFDMContCpn: public FDMEngine {
public:
    /* Constructors and destructor */
    CrankNicolsonFDMContCpn();
    CrankNicolsonFDMContCpn(double spot, double maturity, double barrier,
                CpnFreq freq, unsigned int imax, unsigned int jmax,
                double upper, double lower = 0.0);
    ~CrankNicolsonFDMContCpn();

    /* calculate price by using fdm */
    virtual void calcPrice();

    /* set functions */
    void setCpnIdx(std::vector<double> cpnSchedule);
    void setCoupon(double coupon);
private:
    /* calculate price at time i */
    void value(int i);

    double mBarrier;
    double mCoupon;
    CpnFreq mFreq;

    std::vector<unsigned int> mCpnIdx;    // time index in which coupon date
    int mBarrierIdx;
};

#endif
