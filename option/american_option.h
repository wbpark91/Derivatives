#ifndef _AMERICAN_OPTION_H_
#define _AMERICAN_OPTION_H_
#include "option.h"
#include <vector>

enum ExerDummy { Continue = 0, Exercise = 1 };

class AmericanOption: public Option {
public:
    AmericanOption() {}
    AmericanOption(double strike, double maturity, OptionType type);
    ~AmericanOption() {}
    double bntprice(unsigned int steps, BinomialType bntType);

    void printExer();
    std::vector<double> exerciseBound(unsigned int steps, BinomialType bntType);
private:
    std::vector< std::vector<ExerDummy> > exerTree_;
};

#endif
