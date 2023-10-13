#pragma once

#include "utils.h"

namespace Asuka {

    class Interval {
    public:
        Interval() :min(inf), max(-inf) {}
        Interval(double _min, double _max) : min(_min), max(_max) {}

        bool Contains(double x) const {
            return min <= x && x <= max;
        }

        bool Surrounds(double x) const {
            return min < x && x < max;
        }

        static const Interval Empty;
        static const Interval Universe;

    public:
        double min, max;
    };

    static const Interval Empty(inf, -inf);
    static const Interval Universe(0.00001, inf);

}