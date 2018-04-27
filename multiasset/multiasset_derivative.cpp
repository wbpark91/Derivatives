#include "multiasset_derivative.h"
#include <iostream>

/* Constructors and destructor */
MADerivative::MADerivative() { }

MADerivative::MADerivative(double maturity, unsigned int numAsset)
    :mMaturity(maturity), mNumAsset(numAsset) {
    mCov = Matrix<double>(numAsset, numAsset, 0.0);
    mCurrentAsset = 0;
}

MADerivative::~MADerivative() { }

void MADerivative::addMarketVariable(MarketVariable mktVar) {
    if (mCurrentAsset == mNumAsset) {
        std::cout << "Error: Number of asset is already full." << std::endl;
        std::cout << "Number of asset: " << mNumAsset << std::endl;
        std::cout << "Current number of asset: " << mCurrentAsset << std::endl;
        exit(1);
    }

    mMktVar.push_back(mktVar);

    /* Fill covariance matrix */
    mCov(numAsset, numAsset) = (mktVar.getVol()) * (mktVar.getVol());
    mCurrentAsset++;
}

void MADerivative::setCov(double cov, unsigned int num1, unsigned int num2) {
    mCov(num1, num2) = cov;
    mCov(num2, num1) = cov;
}

void MADerivative::print() const {
    std::cout << "==== Underlying Information ====" << std::endl;
    std::cout << "Number of current asset: " << mNumAsset << std::endl;
    for (int i = 0; i < mMktVar.size(); ++i) {
        mMktVar[i].print();
    }
    std::cout << "Maturity: " << mMaturity << std::endl;
    std::cout << "Covariance matrix" << std::endl;
    mCov.print();
}
