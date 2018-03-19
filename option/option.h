#ifndef _OPTION_H_
#define _OPTION_H_
#include "payoff.h"
#include "marketvariable.h"
#include "derivatives.h"
#include <vector>

enum OptionType { Call = 1, Put = -1 };

/* Barrier feature : In, Out */
enum BarrierFeature { UI, UO, DI, DO };

class Option: public Derivative {
public:
    Option() {};
    Option(double strike, double maturity, OptionType type);
    ~Option();

    /* Get the closest value below and above of input value from the tree */
    double getLambda(double value, unsigned int steps, unsigned int node, BinomialType bntType);

    /* Set function */
    void setSpot(double s);
    void setRate(double r);
    void setMaturity(double t);

    virtual double bntprice(unsigned int steps, BinomialType bntType) = 0;
protected:
    /* Strike */
    double strike_;

    /* Option Characteristic */
    OptionType type_;

    /* Internal functions */
    double getd1();
    double getd2();
    double h(double x, double n);
    virtual std::vector<double> makeTree(unsigned int steps, BinomialType bntType);
};

#endif
