#ifndef _PAYOFF_H
#define _PAYOFF_H

class Payoff {
public:
    virtual ~Payoff() {}
    virtual double operator()(double s) = 0;
};

#endif
