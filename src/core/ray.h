#pragma once

#include "utils.h"
#include "vec.h"

namespace Asuka {

    class Ray {
    public:
        Ray() : time(0), tMax(inf) {}
        Ray(const Point3& origin, const Vector3& direction, double time = 0.0, double tMax = inf) :
            orig(origin), dir(Normalize(direction)), time(time), tMax(inf) {}

        Point3 origin() const { return orig; }
        Vector3 direction() const { return dir; }
        Point3 at(double t) const { return orig + t * dir; }

    public:
        Point3 orig;
        Vector3 dir;
        double time;
        mutable double tMax;
    };

    class RayDifferential : public Ray {
    public:
        RayDifferential(){ hasDifferentials = false; }
        RayDifferential(const Point3& origin, const Vector3& direction, double time=0.0) : Ray(origin, direction, time) {
            hasDifferentials = false;
        }
        RayDifferential(const Ray& ray) : Ray(ray) {
            hasDifferentials = false;
        }
        void ScaleDifferentials(double s) {
            rxOrigin = orig + (rxOrigin - orig) * s;
            ryOrigin = orig + (ryOrigin - orig) * s;
            rxDirection = dir +(rxDirection - dir) * s;
            ryDirection = dir +(ryDirection - dir) * s;
        }
    public:
        bool hasDifferentials;
        Point3 rxOrigin, ryOrigin;
        Vector3 rxDirection, ryDirection;
    };
};