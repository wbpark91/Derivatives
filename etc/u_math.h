#ifndef _U_MATH_H_
#define _U_MATH_H_
#include <vector>
#include <cmath>
#include "matrix.h"

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

/* Cholesky Decomposition */
template <typename T>
Matrix<T> cholDcomp(Matrix<T> mat) {
    unsigned rows = mat.get_rows();
    unsigned cols = mat.get_cols();

    if (rows != cols) {
        std::cout << "Error: Matrix is not square." << std::endl;
        exit(1);
    }

    if (!(mat.transpose() == mat)) {
        std::cout << "Error: Matrix is not symmetric." << std::endl;
        exit(1);
    }

    Matrix<T> result(rows, cols, 0.0);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j <= i; ++j) {
            T sum = 0;

            for (int k = 0; k < j; ++k) {
                sum += result(i, k) * result(j, k);
            }

            if (i == j) {
                result(i, j) = sqrt(mat(i, i) - sum);
            }
            else {
                result(i, j) = (mat(i, j) - sum) / result(j, j);
            }
        }
    }

    return result;
}

#endif
