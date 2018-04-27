#include "marketvariable.h"

/* Quanto adjustment */
MarketVariable::MarketVariable(double spot, Forex forex, double dividend,
    double corr):spot_(spot), div_(dividend), sigma_(sigma) {
    r_ = forex.mRate - corr * sigma * forex.mVol;
}
