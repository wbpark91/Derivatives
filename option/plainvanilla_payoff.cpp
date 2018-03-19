#include "plainvanilla_payoff.h"
#include "u_math.h"

PlainVanillaPayoff::PlainVanillaPayoff(double strike, OptionType type)
    :strike_(strike), type_(type) {}

double PlainVanillaPayoff::operator()(double s) {
    return MAX(type_ * (s - strike_), 0.0);
}
