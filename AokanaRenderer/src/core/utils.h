#pragma once

#include <cmath>
#include <limits>
#include <random>

namespace Aokana {
    const double INF = std::numeric_limits<double>::max();
    const double PI = acos(-1.0);

    inline double Radians(double degree) {
        return degree * PI / 180.0;
    }

    inline int Clamp(int x, int min, int max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    inline double Clamp(double x, double min, double max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    inline double Clamp01(double x) {
        if (x < 0.0) return 0.0;
        if (x > 1.0) return 1.0;
        return x;
    }

    inline double Square(double value) { return value * value; }

    inline double SafeASin(double x) { return std::asin(Clamp(x, -1.0, 1.0)); }
    inline double SafeACos(double x) { return std::acos(Clamp(x, -1.0, 1.0)); }
    inline double SafeSqrt(double x) { return std::sqrt(std::max(0.0, x)); }

    inline double RandomDoubleInRange(double min, double max) {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline double RandomDoubleIn01() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline int RandomIntInRange(int min, int max) {
        static std::uniform_int_distribution<int> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }
}