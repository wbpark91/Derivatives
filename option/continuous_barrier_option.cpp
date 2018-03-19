#include "continuous_barrier_option.h"
#include <cmath>
#include "normal.h"

ContinuousBarrierOption::ContinuousBarrierOption(double strike, double barrier,
                double maturity, OptionType type, BarrierFeature barrierFeature)
                :BarrierOption(strike, barrier, maturity, type, barrierFeature)
                {}

/* Only for Down and Out Call */
double ContinuousBarrierOption::bsprice() {
    if (barrierFeature_ == DO && type_ == Call) {
        double nd1 = normcdf(getd1());
        double nd2 = normcdf(getd2());
        double nh1 = normcdf(geth1());
        double nh2 = normcdf(geth2());

        return s_ * exp(-div_ * t_) * nd1 - strike_ * exp(-r_ * t_) * nd2 \
            - pow(barrier_ / s_, 1 + 2 * r_ / (sigma_ * sigma_)) * s_ * nh1 \
            + pow(barrier_ / s_, -1 + 2 * r_ / (sigma_ * sigma_)) * strike_ * exp(-r_ * t_) * nh2;
    }
    else {
        return 0;
    }
}

double ContinuousBarrierOption::bntprice(unsigned int steps, BinomialType bntType) {
    std::vector<double> tree = makeTree(steps, bntType);
    double prevSpot;                /* Spot price at previous period */

    for (int i = tree.size() - 1; i > 0; --i) {
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
    }
    return tree[0];
}
