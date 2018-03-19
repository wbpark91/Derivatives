#ifndef _EUROPEAN_OPTION_H_
#define _EUROPEAN_OPTION_H_
#include "option.h"

class EuropeanOption: public Option {
public:
    EuropeanOption() {}
    EuropeanOption(double strike, double maturity, OptionType type);
    ~EuropeanOption() {}
    virtual double bsprice() = 0;
    virtual double bntprice(unsigned int steps, BinomialType bntType);

    virtual double delta() = 0;
protected:
};
#endif
