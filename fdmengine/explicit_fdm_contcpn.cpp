#include "explicit_fdm_contcpn.h"
#include <iostream>

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
void ExplicitFDMContCpn::setCpnIdx(std::vector<unsigned int> cpnIdx) {
    mCpnIdx = cpnIdx;
}

void ExplicitFDMContCpn::setCoupon(double coupon) { mCoupon = coupon; }
