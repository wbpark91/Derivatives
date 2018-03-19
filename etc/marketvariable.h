#ifndef _MARKETVARIABLE_H_
#define _MARKETVARIABLE_H_

class MarketVariable {
public:
    MarketVariable() {}
    MarketVariable(double spot, double r, double dividend, double sigma)
        :spot_(spot), r_(r), div_(dividend), sigma_(sigma) {}
    ~MarketVariable() {}

    /* Get Function */
    double getSpot() const { return spot_; }
    double getRate() const { return r_; }
    double getDiv() const { return div_; }
    double getVol() const { return sigma_; }

    /* Set Function */
    void setSpot(double spot) { spot_ = spot; }
    void setRate(double r) { r_ = r; }
    void setDiv(double dividend) { div_ = dividend; }
    void setVol(double sigma) { sigma_ = sigma; }
private:
    double spot_, r_, div_, sigma_;
};

#endif
