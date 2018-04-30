#ifndef _MULTIASSET_AUTOCALL_H_
#define _MULTIASSET_AUTOCALL_H_
#include "multiasset_derivative.h"
#include "multiasset_autocall_payoff.h"
#include <vector>
#include <utility>

class MAAutoCall: public MADerivative {
public:
    /* Constructors and destructor */
    MAAutoCall();
    MAAutoCall(double maturity, double trigger, std::vector<double> ACSchedule,
        unsigned int numAsset);
    virtual ~MAAutoCall();

    std::pair< std::vector<double>, std::vector<int> > mcPrice(unsigned int numPath);

    /* set MC discount rate */
    void setMCDiscount(double rate);
protected:
    double mTrigger;
    std::vector<double> mACSchedule;
    MAACPayoff* mPayoff;
};
#endif
