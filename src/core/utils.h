#pragma once

#include <cmath>
#include <limits>
#include <tuple>
#include <random>

namespace Asuka {

    const double inf = std::numeric_limits<double>::max();
    const double pi = acos(-1.0);

    inline double degree_to_radian(double degree) {
        return degree * pi / 180.0;
    }

    inline double random_double() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline double random_double(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline int random_int(int min, int max) {
        static std::uniform_int_distribution<int> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

}