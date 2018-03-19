#include "american_option.h"
#include "plainvanilla_payoff.h"
#include "plainvanilla_option.h"
#include "u_math.h"
#include <cmath>
#include <iostream>

AmericanOption::AmericanOption(double strike, double maturity, OptionType type)
    :Option(strike, maturity, type) {
    payoff_.reset(new PlainVanillaPayoff(strike, type));
}

double AmericanOption::bntprice(unsigned int steps, BinomialType bntType) {
    std::vector<double> tree;
    if (bntType == BD) {                    /* Broadie and Detemple Method */
        tree.resize(steps);    /* Tree at N - 1 */

        /* Use parameters of CRR */
        dt_ = t_ / steps;
        u_ = exp(sigma_ * sqrt(dt_));                      /* Upward node */
        d_ = 1 / u_;                                       /* Downward node */
        q_ = (exp((r_ - div_) * dt_) - d_) / (u_ - d_);    /* Risk neutral probability */

        PlainVanillaOption vOption(strike_, dt_, type_);    /* European option for calculating last node */
        vOption.setMarketVariable(mktVar_);

        /* Payoff setting at last node */
        for (int i = 0; i < tree.size(); ++i) {
            double spot = s_ * pow(u_, steps - i - 1) * pow(d_, i);
            vOption.setSpot(spot);
            tree[i] = MAX(vOption.bsprice(), (*payoff_)(spot));
        }
    }
    else {                              /* Other Method */
        tree = makeTree(steps, bntType);
    }

    /* Backward induction */
    double prevSpot;                /* Spot price at previous period */
    double exValue, contValue;      /* Exercise Value and Continuation Value */
    exerTree_.clear();              /* Initialize exercise tree */

    for (int i = tree.size() - 1; i > 0; --i) {
        std::vector<ExerDummy> exer;

        for (int j = 0; j < i; ++j) {
            prevSpot = s_ * pow(u_, i - j - 1) * pow(d_, j);
            exValue = (*payoff_)(prevSpot);
            contValue = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);

            /* Write exercise tree at time i */
            if (exValue > contValue)
                exer.push_back(Exercise);
            else
                exer.push_back(Continue);
            tree[j] = MAX(exValue, contValue);
        }

        /* Write exercise tree */
        exerTree_.push_back(exer);
    }
    return tree[0];
}

void AmericanOption::printExer() {
    for (int i = 0; i < exerTree_.size(); ++i) {
        for (int j = 0; j < exerTree_[i].size(); ++j) {
            std::cout << exerTree_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<double> AmericanOption::exerciseBound(unsigned int steps, BinomialType bntType) {
    bntprice(steps, bntType);

    std::vector<double> bound;
    for (int i = 0; i < exerTree_.size(); ++i) {
        for (int j = 0; j < exerTree_[i].size() - 1; ++j) {
            /* If no early exercise */
            if (exerTree_[i][exerTree_[i].size() - 1] == 0) {
                bound.push_back(0);
                break;
            }

            /* If early exercise */
            if (exerTree_[i][j] == 0 && exerTree_[i][j+1] == 1) {
                bound.push_back(s_ * pow(u_, exerTree_[i].size() - (j + 1)) * pow(d_, j + 1));
                break;
            }
        }
    }
    return bound;
}
