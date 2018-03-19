#include "option.h"
#include "u_math.h"
#include <cmath>
#include <iostream>

Option::Option(double strike, double maturity, OptionType type)
    :strike_(strike), type_(type), Derivative(maturity) { }

Option::~Option() {}

std::vector<double> Option::makeTree(unsigned int steps, BinomialType bntType) {
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
        case LR:                                /* Leisen and Reimer */
            q_ = h(getd2(), steps);
            q1 = h(getd1(), steps);
            u_ = exp((r_ - div_) * dt_) * q1 / q_;
            d_ = (exp((r_ - div_) * dt_) - q_ * u_) / (1 - q_);
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

double Option::getLambda(double value, unsigned int steps, unsigned int node, BinomialType bntType) {
    double dt = t_ / steps;
    double u, d, q, q1;
    double lambda;
    std::vector<double> tree(node + 1);
    std::vector<double> cValue;     /* Closest value vector */

    /* Choose u, d, q corresponding method of binomial tree */
    switch (bntType) {
        case CRR:                               /* CRR Tree */
            u = exp(sigma_ * sqrt(dt));                      /* Upward node */
            d = 1 / u;                                       /* Downward node */
            q = (exp((r_ - div_) * dt) - d) / (u - d);    /* Risk neutral probability */
            break;
        case RB:                                /* Rendleman and Bartter */
            u = exp((r_ - div_ - 0.5 * sigma_ * sigma_) * dt + sigma_ * sqrt(dt));
            d = u / exp(2 * sigma_ * sqrt(dt));
            q = (exp((r_ - div_) * dt) - d) / (u - d);    /* Risk neutral probability */
            break;
        case LR:                                /* Leisen and Reimer */
            q = h(getd2(), steps);
            q1 = h(getd1(), steps);
            u = exp((r_ - div_) * dt) * q1 / q;
            d = (exp((r_ - div_) * dt) - q * u) / (1 - q);
            break;
        default:
            std::cout << "European Option can have 3 methods: CRR, RB, LR" << std::endl;
            exit(1);
    }

    /* Spot tree construct */
    for (int i = 0; i < tree.size(); ++i) {
        tree[i] = s_ * pow(u, steps - i) * pow(d, i);
    }

    /* Find below and above value */
    cValue = findBAValue(tree, value);

    /* Calculation of lambda */
    lambda = (cValue[1] - value) / (cValue[1] - cValue[0]);
    return lambda;
}

double Option::getd1() {
    return (log(s_ / strike_) + (r_ - div_ + 0.5 * sigma_ * sigma_) * t_) / (sigma_ * sqrt(t_));
}

double Option::getd2() {
    return getd1() - sigma_ * sqrt(t_);
}

double Option::h(double x, double n) {
    int sgnx;            /* sign of x */
    /* Implement sign(x) */
    if (x >= 0)
        sgnx = 1;
    else
        sgnx = -1;
    double result = 0.5 + sgnx * sqrt(0.25 - 0.25 * exp(-(x / (n + 1 / 3.0)) *
                            (x / (n + 1 / 3.0)) * (n + 1 / 6.0)));
    return result;
}

void Option::setSpot(double s) {
    s_ = s;
}

void Option::setMaturity(double t) {
    t_ = t;
}

void Option::setRate(double r) {
    r_ = r;
}
