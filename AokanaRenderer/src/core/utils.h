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

    inline double Degrees(double radians) {
        return (radians * 180) / PI;
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

    // 在长为 length 的数组中寻找一段区间, 使得 pred(arr[i]) 为 true, pred(arr[i + 1]) 为 false, 返回 i 
    // 返回的 i 被 clamp 在 [0, length - 2] 之内
    // 若没有一个 i 使得 pred(arr[i]) 为 true, 则返回 0
    // 若没有一个 i 使得 pred(arr[i]) 为 false, 则返回 length - 2
    template<typename Predicate>
    size_t FindInterval(int length, const Predicate& pred) {
        assert(length >= 2);
        int l = 0;
        int r = length - 2;
        int res = 0;
        // 二分查找
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (pred(mid) == true) { res = mid; l = mid + 1; }
            else { r = mid - 1; }
        }
        return Clamp(res, 0, length - 2);
    }
}