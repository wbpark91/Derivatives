#ifndef _PLAINVANILLA_OPTION_H_
#define _PLAINVANILLA_OPTION_H_
#include "european_option.h"

class PlainVanillaOption: public EuropeanOption {
public:
    /* Constructor and Destructor */
    PlainVanillaOption() {}
    PlainVanillaOption(PlainVanillaOption& option);
    PlainVanillaOption(double strike, double maturity, OptionType type);

    virtual ~PlainVanillaOption() {};
    virtual double bsprice();

    virtual double delta();
private:
};

#endif
