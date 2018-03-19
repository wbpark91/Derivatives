#include <cmath>
#include "plainvanilla_option.h"
#include "plainvanilla_payoff.h"
#include "normal.h"

PlainVanillaOption::PlainVanillaOption(double strike, double maturity, OptionType type)
        : EuropeanOption(strike, maturity, type) {
    payoff_.reset(new PlainVanillaPayoff(strike, type));
}

double PlainVanillaOption::bsprice() {
    double d1 = getd1();
    double d2 = getd2();
    double nd1 = normcdf(type_ * d1);
    double nd2 = normcdf(type_ * d2);

    return type_ * (s_ * exp(-div_ * t_) * nd1 - strike_ * exp(-r_ * t_) * nd2);
}

double PlainVanillaOption::delta() {
    return type_ * normcdf(type_ * getd1());
}
