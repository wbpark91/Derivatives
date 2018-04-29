#ifndef _AUTOCALL_YIELDNOTE_LEAST_PERFORM_PAYOFF_H_
#define _AUTOCALL_YIELDNOTE_LEAST_PERFORM_PAYOFF_H_
#include "multiasset_autocall_payoff.h"
#include "cpn_freq.h"
#include <vector>

class ACYNLeastPerformPayoff: public MAACPayoff {
public:
    /* Constructors and destructor */
    ACYNLeastPerformPayoff();
    ACYNLeastPerformPayoff(double principal, double cpnRate, CouponFreq freq,
        double trigger, double maturity, std::vector<double> ACSchedule);
    ~ACYNLeastPerformPayoff();

    virtual double operator()(std::vector<double> spot, double t);
private:
    double mPrincipal;
    double mCoupon;
    double mCpnRate;
    CouponFreq mFreq;
};

struct Compare {
    double trigger;
    Compare(double const &value): trigger(value) { }

    bool operator()(double const i) {
        return (i < trigger);
    }
};

#endif
