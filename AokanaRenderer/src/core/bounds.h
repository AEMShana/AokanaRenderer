#pragma once

#include <limits>

#include "ray.h"
#include "vec.h"

namespace Aokana {

    class Bounds3 {
    public:
        Bounds3() {
            constexpr double min_num = std::numeric_limits<double>::lowest();
            constexpr double max_num = std::numeric_limits<double>::max();
            p_min = Point3(max_num, max_num, max_num);
            p_max = Point3(min_num, min_num, min_num);
        }
        Bounds3(const Point3& p) : p_min(p), p_max(p) {}
        Bounds3(const Point3& p1, const Point3& p2) :
            p_min(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
            p_max(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}

        const Point3& operator[](int index) const;
        Point3& operator[](int index);

        Point3 Min() const { return p_min; }
        Point3 Max() const { return p_max; }
        Point3 Corner(int index) const;
        Vector3 Diagonal() const { return p_max - p_min; }
        double SurfaceArea() const;
        double Volume() const;
        int MaximumExtent() const;  // 返回最长轴的索引
        Point3 lerp(const Point3& t) const;
        Vector3 Offset(const Point3& p) const;
        void BoundingSphere(Point3* center, double* radius) const;
        bool Hit(const Ray& r, double t_min, double t_max) const;

        static Bounds3 Merge(const Bounds3& b1, const Bounds3& b2);
        static Bounds3 Merge(const Bounds3& b, const Point3& p);
        static Bounds3 Intersect(const Bounds3& b1, const Bounds3& b2);
        static bool Inside(const Point3& p, const Bounds3& b);
        static bool InsideExclusive(const Point3& p, const Bounds3& b);
        static Bounds3 Expand(const Bounds3& b, double delta);

    public:
        Point3 p_min;
        Point3 p_max;
    };
}