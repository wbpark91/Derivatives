#ifndef _MULTIASSET_AUTOCALL_PAYOFF_H_
#define _MULTIASSET_AUTOCALL_PAYOFF_H_
#include <vector>

class MAACPayoff {
public:
    MAACPayoff() { }
    MAACPayoff(double trigger, double maturity, std::vector<double> ACSchedule)
        :mTrigger(trigger), mMaturity(maturity), mACSchedule(ACSchedule) {
        mAutoCall = false;
        mRateSet = false;
    }
    virtual ~MAACPayoff() { }

    virtual double operator()(std::vector<double> spot, double t) = 0;

    /* set functions */
    void setAutoCall(bool autoCall) { mAutoCall = autoCall; }
    void setDiscountRate(double rate) { mDiscountRate = rate; mRateSet = true; }

    bool getAutoCall() const { return mAutoCall; }
protected:
    double mTrigger;
    double mMaturity;
    std::vector<double> mACSchedule;

    /* discount rate */
    double mDiscountRate;
    bool mRateSet;

    /* true if autoacalled */
    bool mAutoCall;
};

#endif
