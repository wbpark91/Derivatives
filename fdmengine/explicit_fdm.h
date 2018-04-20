#ifndef _EXPLICIT_FDM_H_
#define _EXPLICIT_FDM_H_
#include "fdm_engine.h"

class ExplicitFDM: public FDMEngine {
public:
    /* Constructors and destructor */
    ExplicitFDM();
    ExplicitFDM(double spot, double maturity,
                unsigned int imax, unsigned int jmax,
                double upper, double lower = 0.0);
    ~ExplicitFDM();

    /* calculate price by using EFDM */
    virtual void calcPrice();
protected:
    /* calculate value at (j, i) */
    void value(int j, int i);
};

#endif
