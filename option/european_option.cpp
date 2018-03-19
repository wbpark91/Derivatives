#include "european_option.h"
#include <cmath>
#include <vector>

EuropeanOption::EuropeanOption(double strike, double maturity, OptionType type)
    :Option(strike, maturity, type) {}

double EuropeanOption::bntprice(unsigned int steps, BinomialType bntType) {
    std::vector<double> tree = makeTree(steps, bntType);

    /* Backward discount */
    for (int i = tree.size() - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            tree[j] = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);
        }
    }
    return tree[0];
}
