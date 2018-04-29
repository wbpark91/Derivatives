#ifndef _MULTIASSET_DERIVATIVE_H_
#define _MULTIASSET_DERIVATIVE_H_
#include <vector>
#include "matrix.h"
#include "marketvariable.h"

enum DerivativeFeature { EUROPEAN, AMERICAN, CONTBARRIER, DISCBARRIER,
    AUTOCALL, CALLABLE };

/* MultiAssetDerivative class */
class MADerivative {
public:
    /* Constructors and destructor */
    MADerivative();
    MADerivative(double maturity, unsigned int numAsset);
    virtual ~MADerivative();

    void addMarketVariable(MarketVariable mktVar);
    void setCov(double cov, unsigned int num1, unsigned int num2);

    /* Print current information */
    virtual void print() const;
protected:
    std::vector<MarketVariable> mMktVar;
    double mMaturity;
    Matrix<double> mCov;

    DerivativeFeature mFeature;
    unsigned int mNumAsset;
    unsigned int mCurrentAsset;
};

#endif
