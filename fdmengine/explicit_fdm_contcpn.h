#ifndef _EXPLICIT_FDM_CONTCPN_H_
#define _EXPLICIT_FDM_CONTCPN_H_
#include "explicit_fdm.h"
#include "derivatives.h"
#include <vector>

class ExplicitFDMContCpn: public ExplicitFDM {
public:
    /* Constructors and destructor */
    ExplicitFDMContCpn();
    ExplicitFDMContCpn(double spot, double maturity, double barrier,
                CpnFreq freq, unsigned int imax, unsigned int jmax,
                double upper, double lower = 0.0);
    ~ExplicitFDMContCpn();

    virtual void calcPrice();

    /* set functions */
    void setCpnIdx(std::vector<double> cpnSchedule);
    void setCoupon(double coupon);
private:
    double mBarrier;
    double mCoupon;
    CpnFreq mFreq;

    std::vector<unsigned int> mCpnIdx;    // time index in which coupon date
    int mBarrierIdx;
};

#endif
