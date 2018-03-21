#ifndef _CONTINUOUS_BARRIER_OPTION_H_
#define _CONTINUOUS_BARRIER_OPTION_H_
#include "barrier_option.h"
#include "barrier_payoff.h"

class ContinuousBarrierOption: public BarrierOption {
public:
    /* Constructors and Destructor */
    ContinuousBarrierOption() {}
    ContinuousBarrierOption(double strike, double barrier, double maturity, OptionType type,
                    BarrierFeature barrierFeature);
    ContinuousBarrierOption(ContinuousBarrierOption& option);
    ContinuousBarrierOption(const ContinuousBarrierOption& option);
    ~ContinuousBarrierOption() {}

    /* Assignment operators */
    ContinuousBarrierOption& operator= (ContinuousBarrierOption& option);
    ContinuousBarrierOption& operator= (const ContinuousBarrierOption& option);

    virtual double bntprice(unsigned int steps, BinomialType bntType);
    virtual double bsprice();
private:
    void Swap(ContinuousBarrierOption& lhs, ContinuousBarrierOption& rhs);
};
#endif
