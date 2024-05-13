#pragma once

#include <limits>

#include "ray.h"
#include "vec.h"

namespace Aokana {

    class Bounds3 {
    public:
        Bounds3() {
            double minNum = std::numeric_limits<double>::lowest();
            double maxNum = std::numeric_limits<double>::max();
            pMin = Point3(maxNum, maxNum, maxNum);
            pMax = Point3(minNum, minNum, minNum);
        }
        Bounds3(const Point3& p) : pMin(p), pMax(p) {}
        Bounds3(const Point3& p1, const Point3& p2) :
            pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
            pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}

        const Point3& operator[](int index) const {
            assert(0 <= index && index <= 1);
            if (index == 0) return pMin;
            return pMax;
        }

        Point3& operator[](int index) {
            assert(0 <= index && index <= 1);
            if (index == 0) return pMin;
            return pMax;
        }

        Point3 min() const { return pMin; }
        Point3 max() const { return pMax; }
        Point3 corner(int index) const;

        Vector3 diagonal() const { return pMax - pMin; }

        double surface_area() const {
            auto d = diagonal();
            return 2.0 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }

        double volume() const {
            auto d = diagonal();
            return d.x * d.y * d.z;
        }

        int maximum_extent() const { // 返回最长轴的索引
            auto d = diagonal();
            if (d.x > d.y && d.x > d.z) return 0;
            if (d.y > d.z) return 1;
            return 2;
        }

        Point3 lerp(const Point3& t) const {
            return Point3(
                Lerp(t.x, pMin.x, pMax.x),
                Lerp(t.y, pMin.y, pMax.y),
                Lerp(t.z, pMin.z, pMax.z)
            );
        }

        Vector3 offset(const Point3& p) const {
            Vector3 o = p - pMin;
            if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
            if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
            return o;
        }

        void bounding_sphere(Point3* center, double* radius) const {
            *center = (pMin + pMax) / 2;
            *radius = inside(*center, *this) ? Distance(*center, pMax) : 0;
        }

        bool hit(const Ray& r, double t_min, double t_max) const;

        static Bounds3 merge(const Bounds3& b1, const Bounds3& b2);
        static Bounds3 merge(const Bounds3& b, const Point3& p);
        static Bounds3 intersect(const Bounds3& b1, const Bounds3& b2);
        static bool inside(const Point3& p, const Bounds3& b);
        static bool inside_exclusive(const Point3& p, const Bounds3& b);
        static Bounds3 expand(const Bounds3& b, double delta);

    public:
        Point3 pMin;
        Point3 pMax;
    };
}