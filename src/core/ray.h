#pragma once

#include "utils.h"
#include "vec3.h"

namespace Asuka {

    class Ray {
    public:
        Ray() {}
        Ray(const point3& origin, const vec3& direction) : orig(origin), dir(normalize(direction)) {}

        point3 origin() const { return orig; }
        vec3 direction() const { return dir; }

        point3 at(double t) const { return orig + t * dir; }

    public:
        point3 orig;
        vec3 dir;
    };

};