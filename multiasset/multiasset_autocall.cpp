#include "multiasset_autocall.h"
#include "u_math.h"
#include <random>
#include <cmath>
#include "matrix.h"

MAAutoCall::MAAutoCall() { }

MAAutoCall::MAAutoCall(double maturity, double trigger,
    std::vector<double> ACSchedule, unsigned int numAsset)
    :MADerivative(maturity, numAsset), mTrigger(trigger),
        mACSchedule(ACSchedule) {
    mPayoff = new MAACPayoff(trigger, maturity, ACSchedule);
    mFeature = AUTOCALL;
}

MAAutoCall::~MAAutoCall() {
    delete mPayoff;
}

std::pair<double> mcPrice(unsigned int numPath) {
    /* Simulation result vector */
    std::vector<double> simResult(0.0, numPath);

    /* Price and standard error */
    std::pair<double> result;

    /* Random Number Generator */
    std::mt19937_64 gen;
    std::normal_distribution<double> engine(0.0, 1.0);
    std::random_device rd;
    gen.seed(rd());

    /* Initialize path */
    std::vector<double> spot;
    std::vector<double drift;
    for (int i = 0; i < mMktVar.size(); ++i) {
        spot.push_back(mMktVar[i].getSpot());

        /* Applying Ito correction to drift */
        drift.push_back(mMktVar[i].getRate() \
            - 0.5 * mMktVar[i].getVol() * mMktVar[i].getVol());
    }
    double dt;

    /* Cholesky Decomposition */
    Matrix<double> cholCov = cholDcomp(mCov);

    /* Simulation */
    for (int i = 0; i < numPath; ++i) {
        /* not autocalled at initial point */
        mPayoff -> setAutoCall(false);
        for (int j = 0; j < mACSchedule.size(); ++i) {

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
    result.first = mean(simResult);
    result.second = stdev(simResult) / sqrt(numPath);

    return result;
}
