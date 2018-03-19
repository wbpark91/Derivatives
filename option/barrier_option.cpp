#include "barrier_option.h"
#include "barrier_payoff.h"
#include <cmath>
#include <vector>

BarrierOption::BarrierOption(double strike, double barrier, double maturity, OptionType type,
                            BarrierFeature barrierFeature)
                            :Option(strike, maturity, type), barrier_(barrier), barrierFeature_(barrierFeature) {
                                payoff_.reset(new BarrierPayoff(strike, barrier, type, barrierFeature));
                            }

double BarrierOption::getBarrier() const {
    return barrier_;
}

double BarrierOption::geth1() const {
    return (log(barrier_ * barrier_ / (strike_ * s_)) + (r_ - div_ + 0.5 * sigma_ * sigma_) * t_) / (sigma_ * sqrt(t_));
}

double BarrierOption::geth2() const {
    return geth1() - sigma_ * sqrt(t_);
}
