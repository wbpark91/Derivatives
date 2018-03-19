#include "callable_buffered_ran.h"
#include "u_math.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

double CallableBufferedRANPayoff::operator()(double spot) {
    if (spot >= barrier_)
        return faceValue_;
    else
        return faceValue_ * ((spot / initialValue_) + buffer_);
}

CallableBufferedRAN::CallableBufferedRAN(double faceValue, double couponRate,
    double maturity, std::vector<double> cpnSchedule,
    double barrier, double buffer): faceValue_(faceValue), couponRate_(couponRate),
                                    Derivative(maturity),
                                    cpnSchedule_(cpnSchedule),
                                    barrier_(barrier), buffer_(buffer) {

    /* Payoff setting */
    payoff_.reset(new CallableBufferedRANPayoff(faceValue, barrier, buffer));

    /* Make daysBetween */
    daysBetween_.push_back(cpnSchedule[0]); /* from day 0 to first cpn date */
    for (int i = 0; i < cpnSchedule.size() - 1; ++i) {
        daysBetween_.push_back(cpnSchedule[i+1] - cpnSchedule[i]);
    }
}

double CallableBufferedRAN::bntprice(unsigned int steps, BinomialType bntType) {
    std::vector<double> tree = makeTree(steps, bntType);

    /* Set up output stream */
    std::ofstream outfile;
    outfile.open("result.csv", std::ios::app);

    /* Backward induction */
    double prevSpot;                /* Spot price at previous period */
    double exValue, contValue;      /* Exercise Value and Continuation Value */
    int cpnIndex = cpnSchedule_.size() - 2; /* Coupon Schedule Idx, starts at last */
    int dayIndex = daysBetween_.size() - 1; /* Days Index, starts at last */
    int days = daysBetween_[dayIndex];

    for (int i = tree.size() - 1; i > 0; --i) {
        int k = tree.size() - i;    /* Remaining steps until maturity */

        /* coupon: day count convention - 30/360 */ /* Need to be adjusted */
        double coupon = exp(-r_ * dt_ * k) * faceValue_ * couponRate_ * (1 / 250.0);
        std::cout << coupon << std::endl;
        // std::cout << i << ", " << days << ", " << dayIndex << std::endl;

        for (int j = 0; j < i; ++j) {
            prevSpot = s_ * pow(u_, i - j - 1) * pow(d_, j);
            contValue = exp(-r_ * dt_) * (q_ * tree[j] + (1 - q_) * tree[j + 1]);

            /* Barrier */
            if (prevSpot >= barrier_)
                tree[j] = contValue + coupon;
            else
                tree[j] = contValue;

            /* Call */
            if ((i - 1) * dt_ - cpnSchedule_[cpnIndex] / 250.0 < 1e-06 && i - 1 > 0) {
                // std::cout << "Call: " << (i - 1) * dt_ * 250.0 << ", " << cpnIndex << ", " << cpnSchedule_[cpnIndex] << std::endl;
                tree[j] = MIN(faceValue_, contValue);

                cpnIndex--;
                dayIndex--;
                days = daysBetween_[dayIndex];
            }
            outfile << tree[j] << ",";
        }
        outfile << std::endl;
    }
    outfile.close();
    return tree[0];
}
