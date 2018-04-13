#include <cmath>
#include <algorithm>
#include "plainvanilla_option.h"
#include "plainvanilla_payoff.h"
#include "normal.h"
#include "explicit_fdm.h"

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

/* fdm price */
double PlainVanillaOption::fdmprice(unsigned int imax, unsigned int jmax, FDMType fdmtype,
                double upper, double lower) {
    if (fdmtype == Exp) {
        mFDMEngine.reset(new ExplicitFDM(s_, t_, imax, jmax, upper ,lower));
    }
    setBoundary(mFDMEngine -> getMesh());
    mFDMEngine -> boundarySet();

    mFDMEngine -> setFirst(r_ - div_);
    mFDMEngine -> setSecond(sigma_);
    mFDMEngine -> setR(r_);

    mFDMEngine -> calcPrice();

    return mFDMEngine -> getMesh()[mFDMEngine -> getSpotIdx()][0];
}

/* Boundary setting */
void PlainVanillaOption::setBoundary(Mesh& mesh) {
    int imax = mesh.getImax();
    int jmax = mesh.getJmax();
    double dt = mesh.getdt();

    /* upper and lower boundary */
    for (int i = 0; i < imax; ++i) {
        mesh[0][i] = 0.0;
        mesh[jmax][i] = mesh[jmax][i] - strike_ * exp(-r_ * (t_ - i * dt));
    }

    /* at maturity */
    for (int j = 0; j <= jmax; ++j) {
        mesh[j][imax] = (*payoff_)(mesh[j][imax]);
    }
}

/* Swap function */
void PlainVanillaOption::Swap(PlainVanillaOption& lhs, PlainVanillaOption& rhs) {
    Option::Swap(&lhs, &rhs);
}
