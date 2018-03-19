#ifndef _BARRIER_PAYOFF_H_
#define _BARRIER_PAYOFF_H_
#include "payoff.h"
#include "option.h"

class BarrierPayoff: public Payoff {
public:
    BarrierPayoff(double strike, double barrier, OptionType type, BarrierFeature barrierFeature);
    ~BarrierPayoff() {}
    virtual double operator()(double spot);

private:
    double strike_;
    double barrier_;
    OptionType type_;
    BarrierFeature barrierFeature_;
};
#endif
