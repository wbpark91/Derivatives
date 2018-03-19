#ifndef _DERIVATIVES_H_
#define _DERIVATIVES_H_
#include "marketvariable.h"
#include "payoff.h"
#include <vector>
#include <memory>

enum BinomialType { CRR, RB, LR, BD };

class Derivative {
public:
    Derivative();
    Derivative(double maturity);
    virtual ~Derivative();

    virtual void setMarketVariable(MarketVariable mktVar);
    virtual double bntprice(unsigned int steps, BinomialType bntType) = 0;
protected:
    /* Market Variables */
    double s_, r_, div_, t_, sigma_;
    MarketVariable mktVar_;

    /* Payoff */
    std::unique_ptr<Payoff> payoff_;

    /* Binomial Tree Paramaters */
    double u_, d_, q_, dt_;

    /* Internal functions */
    virtual std::vector<double> makeTree(unsigned int steps, BinomialType bntType);
};

#endif
