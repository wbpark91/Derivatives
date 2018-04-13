#include <iostream>
#include "plainvanilla_option.h"
#include "marketvariable.h"

int main(void) {
    MarketVariable mktVar(100, 0.02, 0.0, 0.2);
    PlainVanillaOption option(100, 1.0, Call);
    option.setMarketVariable(mktVar);
    std::cout << "BS Price: " << option.bsprice() << std::endl;
    std::cout << "EFDM Price: " << option.fdmprice(300, 50, Exp, 200) << std::endl;
    return 0;
}
