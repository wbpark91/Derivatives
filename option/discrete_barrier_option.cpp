#include "discrete_barrier_option.h"
#include "plainvanilla_payoff.h"
#include <vector>
#include <cmath>
#include <algorithm>

/* Constructor */
DiscreteBarrierOption::DiscreteBarrierOption(double strike, double barrier,
                    double maturity, OptionType type, std::vector<double> date,
                    BarrierFeature barrierFeature)
                    :BarrierOption(strike, barrier, maturity, type, barrierFeature),
                     barrierDate_(date) {
                         /* set a payoff as plain vanilla option */
                         payoff_.reset(new PlainVanillaPayoff(strike, type));
                        /* sort barrierdate to descending order to use for binomial tree */
                        std::sort(barrierDate_.rbegin(), barrierDate_.rend());
                    }

/* Copy Constructors */
DiscreteBarrierOption::DiscreteBarrierOption(DiscreteBarrierOption& option) {
    strike_ = option.strike_;
    barrier_ = option.barrier_;
    t_ = option.t_;
    type_ = option.type_;
    barrierFeature_ = option.barrierFeature_;
    barrierDate_ = option.barrierDate_;
    payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));

    this -> setMarketVariable(option.mktVar_);
}

DiscreteBarrierOption::DiscreteBarrierOption(const DiscreteBarrierOption& option) {
    strike_ = option.strike_;
    barrier_ = option.barrier_;
    t_ = option.t_;
    type_ = option.type_;
    barrierFeature_ = option.barrierFeature_;
    barrierDate_ = option.barrierDate_;
    payoff_.reset(new PlainVanillaPayoff(option.strike_, option.type_));

    this -> setMarketVariable(option.mktVar_);
}

/* Assignment operator */
DiscreteBarrierOption& DiscreteBarrierOption::operator= (DiscreteBarrierOption& option) {
    DiscreteBarrierOption copy(option);
    Swap(*this, copy);
    return *this;
}

DiscreteBarrierOption& DiscreteBarrierOption::operator= (const DiscreteBarrierOption& option) {
    DiscreteBarrierOption copy(option);
    Swap(*this, copy);
    return *this;
}

void DiscreteBarrierOption::Swap(DiscreteBarrierOption& lhs, DiscreteBarrierOption& rhs) {
    BarrierOption::Swap(&lhs, &rhs);
    std::swap(lhs.barrierDate_, rhs.barrierDate_);
}

double DiscreteBarrierOption::bntprice(unsigned int steps, BinomialType bntType) {
    std::vector<double> tree = makeTree(steps, bntType);
    double prevSpot;                /* Spot price at previous period */
    int barrierIdx = 0;             /* Barrier Date index */

    for (int i = tree.size() - 1; i > 0; --i) {
        /* (i - 1) * dt_ = time, since tree.size() - 1 = steps */

        /* If barrier date */
        if (fabs((i - 1) * dt_ - barrierDate_[barrierIdx]) < 1e-06) { /* Considering rounding error */
            /* Apply barrier condition */
            for (int j = 0; j < i; ++j) {
                /* Spot price at each node */
                prevSpot = s_ * pow(u_, i - j - 1) * pow(d_, j);

                switch(barrierFeature_) {       /* For each barrier features */
                    case DO:
                        if (prevSpot <= barrier_) {
                            tree[j] = 0.0;
                            break;
                        }
                        else {
                            tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
                            break;
                        }
                    case DI:
                        if (prevSpot <= barrier_) {
                            tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
                            break;
                        }
                        else {
                            tree[j] = 0.0;
                            break;
                        }
                    case UO:
                        if (prevSpot >= barrier_) {
                            tree[j] = 0.0;
                            break;
                        }
                        else {
                            tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
                            break;
                        }
                    case UI:
                        if (prevSpot >= barrier_) {
                            tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
                            break;
                        }
                        else {
                            tree[j] = 0.0;
                            break;
                        }
                }
            }
            /* Increase barrierIdx for next barrier */
            barrierIdx++;
        }
        /* If not barrier date */
        else {
            /* discount */
            for (int j = 0; j < i; ++j) {
                tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
            }
        }
    }
    return tree[0];
}
