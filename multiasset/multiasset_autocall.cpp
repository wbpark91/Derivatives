#include "multiasset_autocall.h"
#include "u_math.h"
#include <random>
#include <cmath>
#include "matrix.h"
#include <iostream>

MAAutoCall::MAAutoCall() { }

MAAutoCall::MAAutoCall(double maturity, double trigger,
    std::vector<double> ACSchedule, unsigned int numAsset)
    :MADerivative(maturity, numAsset), mTrigger(trigger),
        mACSchedule(ACSchedule) {
    mFeature = AUTOCALL;
}

MAAutoCall::~MAAutoCall() {
    delete mPayoff;
}

std::pair< std::vector<double>, std::vector<int> > MAAutoCall::mcPrice(unsigned int numPath) {
    /* Simulation result vector */
    std::vector<double> simResult(numPath, 0.0);

    /* Price, standard error and autocall information */
    std::pair< std::vector<double>, std::vector<int> > result;

    /* Random Number Generator */
    std::mt19937_64 gen;
    std::normal_distribution<double> engine(0.0, 1.0);
    std::random_device rd;
    gen.seed(rd());

    /* Initialize drift */
    std::vector<double> drift;
    for (int i = 0; i < mMktVar.size(); ++i) {
        /* Applying Ito correction to drift */
        drift.push_back(mMktVar[i].getRate() \
            - 0.5 * mMktVar[i].getVol() * mMktVar[i].getVol());
    }
    double dt;

    /* Initialize autocall count */
    mPayoff -> initAutoCallCount();

    /* Cholesky Decomposition */
    Matrix<double> cholCov = cholDcomp(mCov);

    /* Simulation */
    for (int i = 0; i < numPath; ++i) {
        /* Initialize spot */
        std::vector<double> spot;
        for (int n = 0; n < mMktVar.size(); ++n) {
            spot.push_back(mMktVar[n].getSpot());
        }
        /* not autocalled at initial point */
        mPayoff -> setAutoCall(false);
        for (int j = 0; j < mACSchedule.size(); ++j) {

            /* if autoacalled, terminate creating path */
            if ((mPayoff -> getAutoCall()) == true) {
                break;
            }

            /* dt setup */
            if (j == 0)
                dt = mACSchedule[0];
            else
                dt = mACSchedule[j] - mACSchedule[j - 1];

            /* Generate random number */
            std::vector<double> diffusion;
            for (int k = 0; k < mNumAsset; ++k) {
                diffusion.push_back(engine(gen));
            }

            diffusion = cholCov * diffusion;

            /* Calculate spot price */
            for (int k = 0; k < mNumAsset; ++k) {
                spot[k] = spot[k] * exp(drift[k] * dt + diffusion[k] * sqrt(dt));
            }

            simResult[i] += (*mPayoff)(spot, mACSchedule[j]);

        }
    }

    /* calculate value and standard deviation */
    result.first.push_back(mean(simResult));
    result.first.push_back(stdev(simResult) / sqrt(numPath));
    result.second = (mPayoff -> getACCount());

    return result;
}

void MAAutoCall::setMCDiscount(double rate) {
    mPayoff -> setDiscountRate(rate);
}
