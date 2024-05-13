#pragma once

#include "utils.h"
#include "vec.h"

namespace Aokana {

    class Ray {
    public:
        Ray() : time(0), tMin(0), tMax(INF) {}
        Ray(const Point3& origin, const Vector3& direction, double time = 0.0, double tMin = 0.0, double tMax = INF) :
            orig(origin), dir(Normalize(direction)), time(time), tMin(tMin), tMax(INF) {}

        Point3 origin() const { return orig; }
        Vector3 direction() const { return dir; }
        Point3 at(double t) const { return orig + t * dir; }

        bool operator<(const Ray& rhs) const {
            if(time < rhs.time) return true;
            return false;
        }

    public:
        Point3 orig;
        Vector3 dir;
        double time;
        mutable double tMin;
        mutable double tMax;
    };

    class RayDifferential : public Ray {
    public:
        RayDifferential() { hasDifferentials = false; }
        RayDifferential(const Point3& origin, const Vector3& direction, double time = 0.0) : Ray(origin, direction, time) {
            hasDifferentials = false;
        }
        RayDifferential(const Ray& ray) : Ray(ray) {
            hasDifferentials = false;
        }
        void ScaleDifferentials(double s) {
            rxOrigin = orig + (rxOrigin - orig) * s;
            ryOrigin = orig + (ryOrigin - orig) * s;
            rxDirection = dir + (rxDirection - dir) * s;
            ryDirection = dir + (ryDirection - dir) * s;
        }
    public:
        bool hasDifferentials;
        Point3 rxOrigin, ryOrigin;
        Vector3 rxDirection, ryDirection;
    };
};