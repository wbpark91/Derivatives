#ifndef _DISCRETE_BARRIER_OPTION_H_
#define _DISCRETE_BARRIER_OPTION_H_
#include "barrier_option.h"
#include <vector>

class DiscreteBarrierOption: public BarrierOption {
public:
    /* Constructor and Destructors */
    DiscreteBarrierOption() {}
    DiscreteBarrierOption(double strike, double barrier, double maturity,
                        OptionType type, std::vector<double> date,
                        BarrierFeature barrierFeature);
    DiscreteBarrierOption(DiscreteBarrierOption& option);
    DiscreteBarrierOption(const DiscreteBarrierOption& option);
    ~DiscreteBarrierOption() {}

    /* Assignment operator */
    DiscreteBarrierOption& operator= (DiscreteBarrierOption& option);
    DiscreteBarrierOption& operator= (const DiscreteBarrierOption& option);

    virtual double bntprice(unsigned int steps, BinomialType bntType);
private:
    /* Discrete barrier date */
    std::vector<double> barrierDate_;

    /* Swap function */
    void Swap(DiscreteBarrierOption& lhs, DiscreteBarrierOption& rhs);
};

#endif
