#pragma once

#include "utils.h"
#include "vec.h"

namespace Aokana {

    class Ray {
    public:
        Ray() : time(0), tMin(0), tMax(INF) {}
        Ray(const Point3& origin, const Vector3& direction, double time = 0.0, double tMin = 0.0, double tMax = INF) :
            origin(origin), direction(Normalize(direction)), time(time), tMin(tMin), tMax(INF) {}
        Point3 At(double t) const { return origin + t * direction; }

    public:
        Point3 origin;
        Vector3 direction;
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
            rxOrigin = origin + (rxOrigin - origin) * s;
            ryOrigin = origin + (ryOrigin - origin) * s;
            rxDirection = direction + (rxDirection - direction) * s;
            ryDirection = direction + (ryDirection - direction) * s;
        }
    public:
        bool hasDifferentials;
        Point3 rxOrigin, ryOrigin;
        Vector3 rxDirection, ryDirection;
    };
};