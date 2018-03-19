#ifndef _PLAINVANILLA_OPTION_H_
#define _PLAINVANILLA_OPTION_H_
#include "european_option.h"

class PlainVanillaOption: public EuropeanOption {
public:
    /* Constructor and Destructor */
    PlainVanillaOption() {}
    PlainVanillaOption(PlainVanillaOption& option);
    PlainVanillaOption(double strike, double maturity, OptionType type);

    /* Assignment Operator */
    PlainVanillaOption& operator=(PlainVanillaOption option);
    PlainVanillaOption& operator=(const PlainVanillaOption& option);

    virtual ~PlainVanillaOption() {};
    virtual double bsprice();

    virtual double delta();
private:
};

#endif
