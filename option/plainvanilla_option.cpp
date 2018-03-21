#include <cmath>
#include <algorithm>
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
    strike_ = option.strike_;
    t_ = option.t_;
    type_ = option.type_;
    this -> setMarketVariable(option.mktVar_);
    payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));
}

PlainVanillaOption::PlainVanillaOption(const PlainVanillaOption& option) {
    strike_ = option.strike_;
    t_ = option.t_;
    type_ = option.type_;
    this -> setMarketVariable(option.mktVar_);
    payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));
}

/* Assignment operator */
PlainVanillaOption& PlainVanillaOption::operator= (PlainVanillaOption& option) {
    PlainVanillaOption copy(option);
    Swap(*this, copy);

    return *this;
}

PlainVanillaOption& PlainVanillaOption::operator= (const PlainVanillaOption& option) {
    PlainVanillaOption copy(option);
    Swap(*this, copy);

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


/* Swap function */
void PlainVanillaOption::Swap(PlainVanillaOption& lhs, PlainVanillaOption& rhs) {
    /* Option properties swap */
    std::swap(lhs.strike_, rhs.strike_);
    std::swap(lhs.t_, rhs.t_);
    std::swap(lhs.type_, rhs.type_);
    std::swap(lhs.payoff_, rhs.payoff_);

    /* Market Variable swap */
    std::swap(lhs.mktVar_, rhs.mktVar_);
    std::swap(lhs.s_, rhs.s_);
    std::swap(lhs.r_, rhs.r_);
    std::swap(lhs.div_, rhs.div_);
    std::swap(lhs.sigma_, rhs.sigma_);
}
