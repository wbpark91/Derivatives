#include <cmath>
#include "plainvanilla_option.h"
#include "plainvanilla_payoff.h"
#include "normal.h"

/* Constructor */
PlainVanillaOption::PlainVanillaOption(double strike, double maturity, OptionType type)
        : EuropeanOption(strike, maturity, type) {
    payoff_.reset(new PlainVanillaPayoff(strike, type));
}

/* Copy constructor */
PlainVanillaOption::PlainVanillaOption(PlainVanillaOption& option) {
    if (&option != this) {
        strike_ = option.strike_;
        t_ = option.t_;
        type_ = option.type_;
        this -> setMarketVariable(option.mktVar_);
        payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));
    }
}

/* Assignment operator */
PlainVanillaOption& PlainVanillaOption::operator= (PlainVanillaOption option) {
    if (&option != this) {
        strike_ = option.strike_;
        t_ = option.t_;
        type_ = option.type_;
        this -> setMarketVariable(option.mktVar_);
        payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));
    }

    return *this;
}

PlainVanillaOption& PlainVanillaOption::operator= (const PlainVanillaOption& option) {
    if (&option != this) {
        strike_ = option.strike_;
        t_ = option.t_;
        type_ = option.type_;
        this -> setMarketVariable(option.mktVar_);
        payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));
    }

    return *this;
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
