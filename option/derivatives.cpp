#include <iostream>
#include "derivatives.h"
#include <cmath>

Derivative::Derivative() {}

Derivative::Derivative(double maturity): t_(maturity) {}

Derivative::~Derivative() {
}

void Derivative::setMarketVariable(MarketVariable mktVar) {
    mktVar_ = mktVar;
    s_ = mktVar.getSpot();
    r_ = mktVar.getRate();
    div_ = mktVar.getDiv();
    sigma_ = mktVar.getVol();
}

std::vector<double> Derivative::makeTree(unsigned int steps, BinomialType bntType) {
    dt_ = t_ / steps;
    double q1;
    switch (bntType) {
        case CRR:                               /* CRR Tree */
            u_ = exp(sigma_ * sqrt(dt_));                      /* Upward node */
            d_ = 1 / u_;                                       /* Downward node */
            q_ = (exp((r_ - div_) * dt_) - d_) / (u_ - d_);    /* Risk neutral probability */
            break;
        case RB:                                /* Rendleman and Bartter */
            u_ = exp((r_ - div_ - 0.5 * sigma_ * sigma_) * dt_ + sigma_ * sqrt(dt_));
            d_ = u_ / exp(2 * sigma_ * sqrt(dt_));
            q_ = (exp((r_ - div_) * dt_) - d_) / (u_ - d_);    /* Risk neutral probability */
            break;
        default:
            std::cout << "European Option can have 3 methods: CRR, RB, LR" << std::endl;
            exit(1);
    }
    std::vector<double> tree(steps + 1);

    /* Set up final node */
    for (int i = 0; i < tree.size(); ++i) {
        tree[i] = (*payoff_)(s_ * pow(u_, steps - i) * pow(d_, i));
    }

    return tree;
}
