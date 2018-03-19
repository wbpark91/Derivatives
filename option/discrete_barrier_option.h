#ifndef _DISCRETE_BARRIER_OPTION_H_
#define _DISCRETE_BARRIER_OPTION_H_
#include "barrier_option.h"
#include <vector>

class DiscreteBarrierOption: public BarrierOption {
public:
    DiscreteBarrierOption() {}
    ~DiscreteBarrierOption() {}
    DiscreteBarrierOption(double strike, double barrier, double maturity,
                        OptionType type, std::vector<double> date,
                        BarrierFeature barrierFeature);
    virtual double bntprice(unsigned int steps, BinomialType bntType);
private:
    /* Discrete barrier date */
    std::vector<double> barrierDate_;
};

#endif
