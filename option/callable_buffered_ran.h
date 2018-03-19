#ifndef _CALLABLE_BUFFERED_RAN_H_
#define _CALLABLE_BUFFERED_RAN_H_
#include "derivatives.h"
#include "payoff.h"
#include <vector>

class CallableBufferedRANPayoff: public Payoff {
public:
    CallableBufferedRANPayoff(double faceValue, double barrier,
                            double buffer, double initialValue = 100)
        : faceValue_(faceValue), initialValue_(initialValue),
        barrier_(barrier), buffer_(buffer) {}
    virtual double operator()(double spot);
private:
    double initialValue_;       /* Initial underlying price */
    double faceValue_;
    double barrier_;
    double buffer_;
};

class CallableBufferedRAN: public Derivative {
public:
    CallableBufferedRAN(double faceValue, double couponRate, double maturity,
                        std::vector<double> cpnSchedule,
                        double barrier, double buffer);
    ~CallableBufferedRAN() {}

    virtual double bntprice(unsigned int steps, BinomialType bntType);
private:
    double faceValue_;
    double couponRate_;
    std::vector<double> cpnSchedule_;
    std::vector<double> daysBetween_;   /* days between payment dates */
    double barrier_;
    double buffer_;
};

#endif
