#pragma once

#include <cmath>
#include <limits>
#include <tuple>
#include <random>

namespace Aokana {

    const double INF = std::numeric_limits<double>::max();
    const double PI = acos(-1.0);

    inline double Radians(double degree) {
        return degree * PI / 180.0;
    }

    namespace Random {
        inline double RandomDouble(double min = 0, double max = 1) {
            static std::uniform_real_distribution<double> distribution(min, max);
            static std::mt19937 generator;
            return distribution(generator);
        }

        inline int RandomInt(int min, int max) {
            static std::uniform_int_distribution<int> distribution(min, max);
            static std::mt19937 generator;
            return distribution(generator);
        }
    }

    //inline double SafeASin(double x) { return std::asin(Clamp(x, -1, 1)); }
    //inline double SafeACos(double x) { return std::acos(Clamp(x, -1, 1)); }
}