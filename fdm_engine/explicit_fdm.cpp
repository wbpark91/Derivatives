#include "explicit_fdm.h"

/* Constructors and destructor */
ExplicitFDM::ExplicitFDM() { }

ExplicitFDM::~ExplicitFDM() { }

ExplicitFDM::ExplicitFDM(double spot, double maturity,
            unsigned int imax, unsigned int jmax,
            double upper, double lower)
            :FDMEngine(spot, maturity, imax, jmax, upper, lower) { }
