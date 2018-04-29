#include "autocall_yieldnote_least_perform_payoff.h"
#include <algorithm>
#include <cmath>
#include <iostream>

ACYNLeastPerformPayoff::ACYNLeastPerformPayoff() { }

ACYNLeastPerformPayoff::ACYNLeastPerformPayoff(double principal, double cpnRate,
    CouponFreq freq, double trigger, double maturity,
    std::vector<double> ACSchedule)
    :MAACPayoff(trigger, maturity, ACSchedule), mPrincipal(principal),
     mCpnRate(cpnRate), mFreq(freq) { mCoupon = cpnRate * principal / freq; }

ACYNLeastPerformPayoff::~ACYNLeastPerformPayoff() { }

double ACYNLeastPerformPayoff::operator()(std::vector<double> spot, double t) {
    /* Check Discount rate setting */
    if (mRateSet == false) {
        std::cout << "Discount rate has not been set to payoff class." << std::endl;
        exit(1);
    }

    /* If maturity */
    if (t == mMaturity) {
        /* If not autocall */
        if (std::any_of(spot.cbegin(), spot.cend(),
            Compare(mTrigger))) {

            /* Find the least performed value */
            std::vector<double>::iterator leastValue
                = std::min_element(std::begin(spot), std::end(spot));

            /* return payoff */
            return exp(-mDiscountRate * t) \
                * (mPrincipal + mPrincipal * ((*leastValue) - 100) / 100.0);    // need to be generalized

        }
        else {
            return exp(-mDiscountRate * t) * (mPrincipal + mCoupon * t * mFreq);
        }
    }

    /* If not maturity */
    if (std::any_of(spot.cbegin(), spot.cend(), /* If not autocall */
        Compare(100.0))) {
            return 0;
        }
    else {                                      /* If autocall */
        setAutoCall(true);
        return exp(-mDiscountRate * t) * (mPrincipal + mCoupon * t / mFreq);
    }
}
