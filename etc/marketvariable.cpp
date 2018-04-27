#include "marketvariable.h"
#include <iostream>

/* Quanto adjustment */
MarketVariable::MarketVariable(double spot, Forex forex, double dividend,
    double corr):spot_(spot), div_(dividend), sigma_(sigma) {
    r_ = forex.mRate - corr * sigma * forex.mVol;
}

void MarketVariable::print() const {
    std::cout << "Spot: " << spot_ << " ";
    std::cout << "Rate(Risk-Neutral): " << r_ << " ";
    std::cout << "Dividend: " << div_ << " ";
    std::cout << "Volatility: " << spot_ << " ";
}
