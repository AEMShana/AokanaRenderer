#pragma once

#include "utils.h"
#include "vec.h"

namespace Asuka {

    class Ray {
    public:
        Ray() : tm(0) {}
        Ray(const Point3& origin, const Vector3& direction, double time = 0.0) :
            orig(origin), dir(Normalize(direction)), tm(time) {}

        Point3 origin() const { return orig; }
        Vector3 direction() const { return dir; }
        double time() const { return tm; }

        Point3 at(double t) const { return orig + t * dir; }

    public:
        Point3 orig;
        Vector3 dir;
    private:
        double tm;
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