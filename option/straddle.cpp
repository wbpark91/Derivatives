#include "straddle.h"
#include "u_math.h"
#include <random>
#include <iostream>
#include "normal.h"

StraddlePayoff::StraddlePayoff(double strike): strike_(strike) {}

double StraddlePayoff::operator()(double s) {
    return MAX(s - strike_, 0) + MAX(strike_ - s , 0);
}

Straddle::Straddle(double strike, double maturity): strike_(strike), Derivative(maturity) {
    payoff_.reset(new StraddlePayoff(strike));
    call_ = new PlainVanillaOption(strike, maturity, Call);
    put_ = new PlainVanillaOption(strike, maturity, Put);
}

Straddle::~Straddle() {
    delete call_;
    delete put_;
}

void Straddle::setMarketVariable(MarketVariable mktVar) {
    mktVar_ = mktVar;
    s_ = mktVar.getSpot();
    r_ = mktVar.getRate();
    div_ = mktVar.getDiv();
    sigma_ = mktVar.getVol();

    /* Set marketvariable to each option */
    call_ -> setMarketVariable(mktVar);
    put_ -> setMarketVariable(mktVar);
}

double Straddle::bsprice() {
    return call_ -> bsprice() + put_ -> bsprice();
}

double Straddle::delta() {
    return call_ -> delta() + put_ -> delta();
}

/* Calculate expected payoff using dynamic replication (Physical measure) */
/* mu = expected return under physical measure */
/* Need to moved into derivative class */
std::vector<double> Straddle::hedgeSimulation(unsigned int numPath, unsigned int steps, double mu) {
    /* Result */
    std::vector<double> result;

    /* Payoffs */
    std::vector<double> payoffStream;

    /* Random Number Generator */
    std::mt19937_64 gen;
    std::normal_distribution<double> engine(0.0, 1.0);
    std::random_device rd;
    gen.seed(rd());

    /* Initialize parameters */
    double dt = t_ / steps;
    double diffusion = sigma_ * sqrt(dt);
    double expret = exp((mu - 0.5 * sigma_ * sigma_) * dt);   /* Expected Return */

    double delta;
    double cash;
    double payoff;

    for (int i = 0; i < numPath; ++i) {
        /* Initialize Spot, Delta, Cash Stream */
        std::vector<double> spotStream;
        spotStream.push_back(s_);
        std::vector<double> deltaStream;
        deltaStream.push_back(this -> delta());
        std::vector<double> cashStream;
        cashStream.push_back(- s_ * deltaStream[0]);

        /* at each path */
        /* Might be better to separate from the whole function */
        for (int j = 1; j <= steps; ++j) {
            /* Create random number */
            double e = engine(gen);

            /* new stock price */
            double s = spotStream[j-1] * expret * exp(diffusion * e);

            if (j != steps) {
                /* change spot and maturity for getting delta */
                call_ -> setSpot(s);
                call_ -> setMaturity(t_ - j * dt);
                put_ -> setSpot(s);
                put_ -> setMaturity(t_ - j * dt);

                /* new delta */
                delta = call_ -> delta() + put_ -> delta();

                /* new cash */
                cash = cashStream[j-1] * exp(r_ * dt) + s * (deltaStream[j-1] - delta);

                /* update stream */
                spotStream.push_back(s);
                deltaStream.push_back(delta);
                cashStream.push_back(cash);

                // std::cout << j << ", " << s << ", " << delta << ", " << cash << ", " << payoff << std::endl;
            }
            else {
                payoff = deltaStream[j-1] * s + cashStream[j-1] * exp(r_ * dt);
                payoffStream.push_back(payoff);
            }
        }
    }
    /* Calculate average, standard deviation */
    double avg = mean(payoffStream);
    double dev = stdev(payoffStream) / sqrt(numPath);
    result.push_back(avg);
    result.push_back(dev);

    return result;
}

double Straddle::expectedPayoff(double mu) {
    double d1 = (log(s_ / strike_) + (mu + 0.5 * sigma_ * sigma_) * t_) / (sigma_ * sqrt(t_));
    double d2 = d1 - sigma_ * sqrt(t_);
    double nd1_call = normcdf(d1);
    double nd2_call = normcdf(d2);
    double nd1_put = normcdf(-d1);
    double nd2_put = normcdf(-d2);

    double call = s_ * exp(mu * t_) * nd1_call - strike_ * nd2_call;
    double put = strike_ * nd2_put - s_ * exp(mu * t_) * nd1_put;

    return call + put;
}
