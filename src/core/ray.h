#pragma once

#include "utils.h"
#include "vec3.h"

namespace Asuka {

    class Ray {
    public:
        Ray() {}
        Ray(const point3& origin, const vec3& direction, double time = 0.0) :
            orig(origin), dir(normalize(direction)), tm(time) {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }
        double time() const { return tm; }

        point3 at(double t) const { return orig + t * dir; }

    public:
        point3 orig;
        vec3 dir;
        double tm;
    };

};