#ifndef _MULTIASSET_AUTOCALL_H_
#define _MUITLASSET_AUTOCALL_H_
#include "multiasset_derivative.h"
#include "multiasset_autocall_payoff.h"
#include <vector>
#include <utility>

class MAAutoCall: public MADerivative {
public:
    MAAutoCall();
    MAAutoCall(double maturity, double trigger, std::vector<double> ACSchedule,
        unsigned int numAsset);
    virtual ~MAAutoCall();

    std::pair<double> mcPrice(unsigned int numPath);
protected:
    double mTrigger;
    std::vector<double> mACSchedule;
    MAACPayoff* mPayoff;
};
#endif
