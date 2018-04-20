#ifndef _PAYOFF_H
#define _PAYOFF_H

class Payoff {
public:
    virtual ~Payoff() {}
    virtual double operator()(double s) = 0;
    virtual void setTrigger(bool trigger) { }
    virtual void setSpot(double spot) { }
};

#endif
