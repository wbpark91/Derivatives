#include "u_math.h"
#include <iostream>
#include <numeric>
#include <cmath>

std::vector<double> findBAValue(std::vector<double> vec, double value) {
    /* Input vector vec should be reverse - sorted */

    std::vector<double> result;
    for (int i = 0; i < vec.size() - 1; ++i) {
        if (value < vec[i] && value >= vec[i+1]) {
            result.push_back(vec[i+1]);
            result.push_back(vec[i]);
            return result;
        }
    }
    std::cout << "Cannot find the closest below and above value." << std::endl;
    exit(1);
}

double mean(std::vector<double> vec) {
    double result = std::accumulate(vec.begin(), vec.end(), 0.0 / vec.size());
    return result / vec.size();
}

double stdev(std::vector<double> vec) {
    double average = mean(vec);
    double result = 0;
    for (int i = 0; i < vec.size(); ++i) {
        result += (vec[i] - average) * (vec[i] - average);
    }
    return sqrt(result / vec.size());
}
