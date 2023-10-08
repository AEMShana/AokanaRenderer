#pragma once

#include "utils.h"
#include "vec.h"

namespace Asuka {

    class Ray {
    public:
        Ray() {}
        Ray(const Point3& origin, const Vector3& direction, double time = 0.0) :
            orig(origin), dir(Normalize(direction)), tm(time) {}

        Point3 origin() const { return orig; }
        Vector3 direction() const { return dir; }
        double time() const { return tm; }

        Point3 at(double t) const { return orig + t * dir; }

    public:
        Point3 orig;
        Vector3 dir;
        double tm;
    };

};