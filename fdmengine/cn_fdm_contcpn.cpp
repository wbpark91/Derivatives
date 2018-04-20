#include "cn_fdm_contcpn.h"
#include <vector>
#include <iostream>

CrankNicolsonFDMContCpn::CrankNicolsonFDMContCpn() { }

CrankNicolsonFDMContCpn::CrankNicolsonFDMContCpn(double spot, double maturity,
            double barrier, CpnFreq freq, unsigned int imax, unsigned int jmax,
            double upper, double lower)
            :FDMEngine(spot, maturity, imax, jmax, upper, lower),
             mBarrier(barrier), mFreq(freq) {
     mBarrierIdx = mMesh.findIndex(barrier, 0);
     if (mBarrierIdx == -1) {
         std::cout << "Initialization error: cannot find barrier." << std::endl;
         exit(1);
     }
 }

 CrankNicolsonFDMContCpn::~CrankNicolsonFDMContCpn() { }

 /* calculation functions */
 void CrankNicolsonFDMContCpn::value(int i) {
     /* Initialization */
     std::vector<double> aVec, bVec, cVec, dVec;
     std::vector<double> alphaVec, sVec;

     aVec.push_back(0.0);
     bVec.push_back(1.0);
     cVec.push_back(0.0);
     dVec.push_back(mMesh[mBarrierIdx][i]);

     alphaVec.push_back(bVec[0]);
     sVec.push_back(dVec[0]);

     for (int j = mBarrierIdx + 1; j < mJmax; ++j) {
         double sigmaSqJ = second_ * second_ * j * j;
         double a = 0.25 * (sigmaSqJ - first_ * j);
         double b = -0.5 * (sigmaSqJ + r_) - 1.0 / dt_;
         double c = 0.25 * (sigmaSqJ + first_ * j);
         double d = -a * mMesh[j-1][i+1] - (-0.5 * (sigmaSqJ + r_) + 1.0 / dt_) * mMesh[j][i+1] - c * mMesh[j+1][i+1];

         aVec.push_back(a);
         bVec.push_back(b);
         cVec.push_back(c);
         dVec.push_back(d);
     }

     aVec.push_back(0.0);
     bVec.push_back(1.0);
     cVec.push_back(0.0);
     dVec.push_back(mMesh[mJmax][i]);

     /* Calculate alpha and s */
     for (int j = 1; j <= mJmax - mBarrierIdx; ++j) {
         double alpha = bVec[j] - aVec[j] * cVec[j-1] / alphaVec[j-1];
         double s = dVec[j] - aVec[j] * sVec[j-1] / alphaVec[j-1];

         alphaVec.push_back(alpha);
         sVec.push_back(s);
     }

     /* calculate V */
     mMesh[mJmax][i] = sVec[mJmax - mBarrierIdx] / alphaVec[mJmax - mBarrierIdx];
     for (int j = mJmax - 1; j >= mBarrierIdx; --j) {
         mMesh[j][i] = (sVec[j - mBarrierIdx] - cVec[j - mBarrierIdx] * mMesh[j+1][i]) / alphaVec[j - mBarrierIdx];
     }
 }

 void CrankNicolsonFDMContCpn::calcPrice() {
     if (mBoundary == false) {
         std::cout << "Boundary is not set yet." << std::endl;
         exit(1);
     }

     for (int i = mImax - 1; i >= 0; --i) {
         value(i);

         if (std::find(mCpnIdx.begin(), mCpnIdx.end(), i) != mCpnIdx.end()) {
             for (int j = mBarrierIdx + 1; j <= mJmax; ++j) {
                 mMesh[j][i] += mCoupon;
             }
         }
     }
     mCalc = true;
 }

 /* set functions */
 void CrankNicolsonFDMContCpn::setCpnIdx(std::vector<double> cpnSchedule) {
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

void CrankNicolsonFDMContCpn::setCoupon(double coupon) { mCoupon = coupon; }
