#define _USE_MATH_DEFINES       // PI를 사용하기 위해 적용(math.h 위에 써줘야 함)

#include <cmath>    // C++에서 사용하는 라이브러리 (C++ 사용시에는 cmath를 사용하는 것이 권장된다)
#include "normal.h"
//#include <math.h>   // C에서 사용하던 라이브러리

// Normal Distribution의 pdf와 cdf
// 인자의 모양을 signiture라고 부른다
double normpdf(double x, double mu, double sigma)       // 함수 정의(definition)
{
    return 1.0/sqrt(2*M_PI)/sigma*exp(-0.5*(x-mu)*(x-mu)/(sigma*sigma));
}

double normcdf(double x, double mu, double sigma)
{
    double v = (x - mu) / sigma;
    return 0.5 * erfc(-v * M_SQRT1_2);
}

