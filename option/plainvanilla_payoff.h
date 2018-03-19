#ifndef _PLAINVANILLA_PAYOFF_H_
#define _PLAINVANILLA_PAYOFF_H_
#include "payoff.h"
#include "option.h"

class PlainVanillaPayoff: public Payoff {
public:
    PlainVanillaPayoff(double strike, OptionType type);
    virtual double operator()(double s);
private:
    double strike_;
    OptionType type_;
};

#endif
