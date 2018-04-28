#ifndef _MULTIASSET_AUTOCALL_PAYOFF_H_
#define _MULTIASSET_AUTOCALL_PAYOFF_H_
#include <vector>

class MAACPayoff {
public:
    MAACPayoff() { }
    MAACPayoff(double trigger, double maturity, std::vector<double> ACSchedule);
        :mTrigger(trigger), mMaturity(maturity), mACSchedule(ACSchedule) {
        mAutocall = false;
    }
    virtual ~MAACPayoff() { }

    virtual double operator(std::vector<double> spot, double time) = 0;
    void setAutoCall(bool autoCall) { mAutoCall = autoCall; }

    bool getAutoCall() const { return mAutoCall; }
protected:
    double mTrigger;
    double mMaturity;
    std::vector<double> mACSchedule;

    /* true if autoacalled */
    bool mAutoCall;
};

#endif
