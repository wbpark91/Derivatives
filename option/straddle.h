#ifndef _STRADDLE_H_
#define _STRADDLE_H_
#include "plainvanilla_option.h"
#include "marketvariable.h"
#include "payoff.h"
#include <vector>

class StraddlePayoff: public Payoff {
public:
    StraddlePayoff(double strike);
    virtual double operator()(double s);
private:
    double strike_;
};

class Straddle: public Derivative {
public:
    Straddle(double strike, double maturity);
    ~Straddle();
    virtual void setMarketVariable(MarketVariable mktVar);
    double bsprice();
    double delta();
    virtual double bntprice(unsigned int steps, BinomialType bntType) { return 0; }

    std::vector<double> hedgeSimulation(unsigned int numPath, unsigned int steps, double mu);
    double expectedPayoff(double mu);
private:
    double strike_;
    PlainVanillaOption* call_;
    PlainVanillaOption* put_;
};
#endif
