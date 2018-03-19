#ifndef _CONTINUOUS_BARRIER_OPTION_H_
#define _CONTINUOUS_BARRIER_OPTION_H_
#include "barrier_option.h"

class ContinuousBarrierOption: public BarrierOption {
public:
    ContinuousBarrierOption() {}
    ContinuousBarrierOption(double strike, double barrier, double maturity, OptionType type,
                    BarrierFeature barrierFeature);
    ~ContinuousBarrierOption() {}

    virtual double bntprice(unsigned int steps, BinomialType bntType);
    virtual double bsprice();
private:
};
#endif
