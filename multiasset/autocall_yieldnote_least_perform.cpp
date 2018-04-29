#include "autocall_yieldnote_least_perform.h"
#include "autocall_yieldnote_least_perform_payoff.h"

/* Constructors and destructor */
ACYNLeastPerform::ACYNLeastPerform() { }

ACYNLeastPerform::ACYNLeastPerform(double principal, double maturity,
    double cpnRate, CouponFreq freq, double trigger,
    std::vector<double> ACSchedule, unsigned int numAsset)
    :MAAutoCall(maturity, trigger, ACSchedule, numAsset),
     mPrincipal(principal), mCpnRate(cpnRate), mFreq(freq) {
    mPayoff = new ACYNLeastPerformPayoff(principal, cpnRate, freq,
        trigger, maturity, ACSchedule);
}

ACYNLeastPerform::~ACYNLeastPerform() {

}
