#ifndef _AUTOCALL_YIELDNOTE_LEAST_PERFORM_H_
#define _AUTOCALL_YIELDNOTE_LEAST_PERFORM_H_
#include "multiasset_autocall.h"
#include "cpn_freq.h"
#include <vector>

class ACYNLeastPerform: public MAAutoCall {
public:
    /* Constructors and destructor */
    ACYNLeastPerform();
    ACYNLeastPerform(double principal, double maturity, double cpnRate,
        CouponFreq freq, double trigger, std::vector<double> ACSchedule,
        unsigned int numAsset);
    ~ACYNLeastPerform();
private:
    double mPrincipal;
    double mCpnRate;
    CouponFreq mFreq;
};


#endif
