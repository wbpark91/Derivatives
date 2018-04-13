#ifndef _PLAINVANILLA_OPTION_H_
#define _PLAINVANILLA_OPTION_H_
#include "european_option.h"
#include "mesh.h"

class PlainVanillaOption: public EuropeanOption {
public:
    /* Constructor and Destructor */
    PlainVanillaOption() {}
    PlainVanillaOption(PlainVanillaOption& option);
    PlainVanillaOption(const PlainVanillaOption& option);
    PlainVanillaOption(double strike, double maturity, OptionType type);

    /* Assignment Operator */
    PlainVanillaOption& operator=(PlainVanillaOption& option);
    PlainVanillaOption& operator=(const PlainVanillaOption& option);

    virtual ~PlainVanillaOption() {};
    virtual double bsprice();
    double fdmprice(unsigned int imax, unsigned int jmax, FDMType fdmtype,
                    double upper, double lower = 0.0);

    virtual double delta();
private:
    void Swap(PlainVanillaOption& lhs, PlainVanillaOption& rhs);
    void setBoundary(Mesh& mesh);
};

#endif
