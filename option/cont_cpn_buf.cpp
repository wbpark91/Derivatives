#include "cont_cpn_buf.h"
#include "explicit_fdm_contcpn.h"
#include "cn_fdm_contcpn.h"
#include <cmath>
#include <iostream>

/* Security functions */

/* Constructors and destructor */
ContCpnBuf::ContCpnBuf() { }

ContCpnBuf::ContCpnBuf(double principal, double coupon, double maturity,
    double barrier, double buffer, CpnFreq freq)
        :mPrincipal(principal), mCoupon(coupon), Derivative(maturity),
        mBarrier(barrier), mBuffer(buffer), mFreq(freq) {
        /* initialize payoff */
        payoff_.reset( new ContCpnBufPayoff(principal, barrier, coupon,
            buffer, freq) );

        /* trigger = 0 at initialized time */
        mTrigger = false;

        /* coupon payment date */
        for (int i = 1; i < freq * maturity; ++i) {
            mCpnSchedule.push_back(i / (double)freq);
        }
}

ContCpnBuf::~ContCpnBuf() {
    if (mFDMEngine != NULL) {
        delete mFDMEngine;
    }
}

/* Calculate price */
double ContCpnBuf::fdmprice(unsigned int imax, unsigned int jmax,
    FDMType fdmType, double upper, double lower) {
        double drift = mForex.mRate - div_ - mCorr * sigma_ * mForex.mVol;

        if (fdmType == Exp) {
            mFDMEngine =
                new ExplicitFDMContCpn(s_, t_, mBarrier, mFreq, imax, jmax,
                    upper, lower);
        }

        if (fdmType == CN) {
            mFDMEngine =
                new CrankNicolsonFDMContCpn(s_, t_, mBarrier, mFreq, imax, jmax,
                    upper, lower);
        }

        setBoundary(mFDMEngine -> getMesh());
        mFDMEngine -> boundarySet();

        mFDMEngine -> setFirst(drift);
        mFDMEngine -> setSecond(sigma_);
        mFDMEngine -> setR(r_);
        mFDMEngine -> setCoupon(mPrincipal * mCoupon / mFreq);
        mFDMEngine -> setCpnIdx(mCpnSchedule);

        mFDMEngine -> calcPrice();

        return mFDMEngine -> getMesh()[mFDMEngine -> getSpotIdx()][0];
}

double ContCpnBuf::fdmdelta() {
    return mFDMEngine -> delta();
}

double ContCpnBuf::fdmgamma() {
    return mFDMEngine -> gamma();
}

double ContCpnBuf::bntprice(unsigned int steps, BinomialType bntType) {
    return 0;
}

/* set functions */
void ContCpnBuf::setTrigger(bool trigger) {
    mTrigger = trigger;
    payoff_ -> setTrigger(trigger);
}

void ContCpnBuf::setForex(Forex forex) {
    mForex = forex;
}

void ContCpnBuf::setCorr(double corr) {
    mCorr = corr;
}

void ContCpnBuf::setMarketVariable(MarketVariable mktVar) {
    Derivative::setMarketVariable(mktVar);
    payoff_ -> setSpot(mktVar.getSpot());
}

void ContCpnBuf::setBoundary(Mesh& mesh) {
    int imax = mesh.getImax();
    int jmax = mesh.getJmax();
    int jbarrier = mesh.findIndex(mBarrier, 0);
    double drift = mForex.mRate - div_ - mCorr * sigma_ * mForex.mVol;
    double dt = mesh.getdt();

    for (int i = 0; i <= imax; ++i) {
        mesh[jbarrier][i] = exp(-r_ * (imax - i) * dt) * (
            mPrincipal + mPrincipal * (exp(drift * t_) - 1 + mBuffer)
        );


        mesh[jmax][i]  = exp(-r_ * (imax - i) * dt) * (
            mPrincipal + mPrincipal * mCoupon / mFreq
        );
    }

    for (int j = jbarrier + 1; j < jmax; ++j) {
        mesh[j][imax] = (*payoff_)(mesh[j][imax]);
    }
}

/* Payoff functions */
ContCpnBufPayoff::ContCpnBufPayoff() { }

ContCpnBufPayoff::ContCpnBufPayoff(double principal,
    double barrier, double coupon, double buffer, CpnFreq freq)
    : mPrincipal(principal), mBarrier(barrier), mCoupon(coupon),
        mBuffer(buffer), mFreq(freq) { mTrigger = false; }

ContCpnBufPayoff::~ContCpnBufPayoff() { }

void ContCpnBufPayoff::setTrigger(bool trigger) {
    mTrigger = trigger;
}

void ContCpnBufPayoff::setSpot(double spot) {
    mSpot = spot;
}

double ContCpnBufPayoff::operator()(double s) {
    if (s < mBarrier) {
        mTrigger = true;
    }

    /* If trigger is not hit */
    if (mTrigger == false) {
        double coupon = mPrincipal * mCoupon * (1.0 / mFreq);
        return mPrincipal + coupon;
    }
    /* If trigger is hit */
    else {
        double idxReturn = (s - mSpot) / mSpot;
        return mPrincipal + mPrincipal * (idxReturn + mBuffer);
    }
}
