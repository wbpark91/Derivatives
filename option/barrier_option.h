#ifndef _BARRIER_OPTION_H_
#define _BARRIER_OPTION_H_
#include "option.h"
#include <vector>

class BarrierOption: public Option {
public:
    BarrierOption() {}
    BarrierOption(double strike, double barrier, double maturity, OptionType type,
                    BarrierFeature barrierFeature);
    ~BarrierOption() {}

    virtual double bntprice(unsigned int steps, BinomialType bntType) = 0;

    double getBarrier() const;
protected:
    BarrierFeature barrierFeature_;
    double barrier_;
    double geth1() const;
    double geth2() const;
};

#endif
