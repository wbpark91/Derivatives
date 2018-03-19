#ifndef _U_MATH_H_
#define _U_MATH_H_
#include <vector>
#include <cmath>

#define MAX(x, y)   (((x) > (y)) ? (x) : (y))

#define MIN(x, y)   (((x) > (y)) ? (y) : (x))

/* Just for binomial tree: need to be generalized */
/* order: order of error, default: 1 */
template <typename T>
T extrapolate(T x1, T y1, T x2, T y2, int order = 1) {
    return (y2 * pow(x2, order) - y1 * pow(x1, order)) / (pow(x2, order) - pow(x1, order));
}

/* Find the closest below and above value */
std::vector<double> findBAValue(std::vector<double> vec, double value);

/* Find average value of vector */
double mean(std::vector<double> vec);

/* Find standard deviation of vector */
double stdev(std::vector<double> vec);
#endif
