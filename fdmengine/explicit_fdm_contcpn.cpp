#include "explicit_fdm_contcpn.h"
#include <iostream>
#include <algorithm>

/* Constructors and destructor */
ExplicitFDMContCpn::ExplicitFDMContCpn() { }

ExplicitFDMContCpn::ExplicitFDMContCpn(double spot, double maturity,
            double barrier, CpnFreq freq, unsigned int imax, unsigned int jmax,
            double upper, double lower)
            :ExplicitFDM(spot, maturity, imax, jmax, upper, lower),
                mBarrier(barrier), mFreq(freq) {
    mBarrierIdx = mMesh.findIndex(barrier, 0);
    if (mBarrierIdx == -1) {
        std::cout << "Initialization error: cannot find barrier." << std::endl;
        exit(1);
    }
}

ExplicitFDMContCpn::~ExplicitFDMContCpn() { }

/* set functions */
void ExplicitFDMContCpn::setCpnIdx(std::vector<double> cpnSchedule) {
    if (mCpnIdx.empty() == true) {
        for (int i = 0; i < cpnSchedule.size(); ++i) {
            unsigned int idx = (int)(cpnSchedule[i] / dt_);
            mCpnIdx.push_back(idx);
        }
    }
    else {
        std::cout << "Initialization error: coupon index vector is not empty." << std::endl;
        exit(1);
    }
}

void ExplicitFDMContCpn::setCoupon(double coupon) { mCoupon = coupon; }

void ExplicitFDMContCpn::calcPrice() {
    if (mBoundary == false) {
        std::cout << "Boundary is not set yet." << std::endl;
        exit(1);
    }

    bool stability = true;
    double limit;

    for (int i = mImax - 1; i >= 0; --i) {
        for (int j = mBarrierIdx + 1; j < mJmax; ++j) {
            double detectValue = 1.0 / (second_ * second_ * j * j);
            if (detectValue < dt_ && stability == true) {
                stability = false;
                limit = detectValue;
            }

            value(j, i);
        }

        /* if coupon date */
        if (std::find(mCpnIdx.begin(), mCpnIdx.end(), i) != mCpnIdx.end()) {
            for (int j = mBarrierIdx + 1; j <= mJmax; ++j) {
                mMesh[j][i] += mCoupon;
            }
        }
    }

    if (stability == false) {
        std::cout << "Warning: stability may not be guaranteed." << std::endl;
        std::cout << "dt: " << dt_ << " Limit: " << limit << std::endl;
    }

    mCalc = true;
}
