#ifndef _MULTIASSET_AUTOCALL_PAYOFF_H_
#define _MULTIASSET_AUTOCALL_PAYOFF_H_
#include <vector>

class MAACPayoff {
public:
    MAACPayoff() { }
    MAACPayoff(double trigger, double maturity, std::vector<double> ACSchedule)
        :mTrigger(trigger), mMaturity(maturity), mACSchedule(ACSchedule) {
        mACCount.resize(ACSchedule.size(), 0);
        mAutoCall = false;
        mRateSet = false;
    }
    virtual ~MAACPayoff() { }

    virtual double operator()(std::vector<double> spot, double t) = 0;

    /* set functions */
    void setAutoCall(bool autoCall) { mAutoCall = autoCall; }
    void setDiscountRate(double rate) { mDiscountRate = rate; mRateSet = true; }

    /* get functions */
    bool getAutoCall() const { return mAutoCall; }
    std::vector<int> getACCount() const { return mACCount; }

    /* add autocall count */
    void addAutoCallCount(int i) { mACCount[i] += 1; }

    /* initialize autocall count */
    void initAutoCallCount() { std::fill(mACCount.begin(), mACCount.end(), 0); }
protected:
    double mTrigger;
    double mMaturity;
    std::vector<double> mACSchedule;
    std::vector<int> mACCount;

    /* discount rate */
    double mDiscountRate;
    bool mRateSet;

    /* true if autoacalled */
    bool mAutoCall;
};

#endif
