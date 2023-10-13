#include "bounds.h"
#include <memory>


namespace Asuka {

    Point3 Bounds3::corner(int index) const {
        return Point3(
            (*this)[(index & 1)].x,
            (*this)[(index & 2) ? 1 : 0].y,
            (*this)[(index & 4) ? 1 : 0].z
        );
    }

    bool Bounds3::Intersect(const Ray& r, Interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            auto t_min = ray_t.min;
            auto t_max = ray_t.max;

            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    Bounds3 Bounds3::merge(const Bounds3& b1, const Bounds3& b2) {
        Point3 pMin(
            std::min(b1.min().x, b2.min().x),
            std::min(b1.min().y, b2.min().y),
            std::min(b1.min().z, b2.min().z)
        );

        Point3 pMax(
            std::max(b1.max().x, b2.max().x),
            std::max(b1.max().y, b2.max().y),
            std::max(b1.max().z, b2.max().z)
        );

        return Bounds3(pMin, pMax);
    }

    Bounds3 Bounds3::merge(const Bounds3& b, const Point3& p) {
        Point3 pMin(
            std::min(b.min().x, p.x),
            std::min(b.min().y, p.y),
            std::min(b.min().z, p.z)
        );

        Point3 pMax(
            std::max(b.max().x, p.x),
            std::max(b.max().y, p.y),
            std::max(b.max().z, p.z)
        );

        return Bounds3(pMin, pMax);
    }

    Bounds3 Bounds3::intersect(const Bounds3& b1, const Bounds3& b2) {
        return Bounds3(
            Point3(
                std::max(b1.min().x, b2.min().x),
                std::max(b1.min().y, b2.min().y),
                std::max(b1.min().z, b2.min().z)
            ),
            Point3(
                std::min(b1.min().x, b2.min().x),
                std::min(b1.min().y, b2.min().y),
                std::min(b1.min().z, b2.min().z)
            )
        );
    }

    bool Bounds3::inside(const Point3& p, const Bounds3& b) {
        return (
            p.x >= b.min().x && p.x <= b.max().x &&
            p.y >= b.min().y && p.y <= b.max().y &&
            p.z >= b.min().z && p.z <= b.max().z
            );
    }

    bool Bounds3::inside_exclusive(const Point3& p, const Bounds3& b) {
        return (
            p.x >= b.min().x && p.x < b.max().x &&
            p.y >= b.min().y && p.y < b.max().y &&
            p.z >= b.min().z && p.z < b.max().z
            );
    }

    Bounds3 Bounds3::expand(const Bounds3& b, double delta) {
        return Bounds3(
            b.min() - Vector3(delta, delta, delta),
            b.max() + Vector3(delta, delta, delta));
    }
}