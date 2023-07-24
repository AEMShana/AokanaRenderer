#include "aabb.h"
#include "shape.h"
#include <memory>


namespace Asuka {
    bool AABB::hit(const Ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    AABB merge_box(const AABB& box0, const AABB& box1) {
        point3 small(
            fmin(box0.min().x(), box1.min().x()),
            fmin(box0.min().y(), box1.min().y()),
            fmin(box0.min().z(), box1.min().z())
        );

        point3 big(
            fmax(box0.max().x(), box1.max().x()),
            fmax(box0.max().y(), box1.max().y()),
            fmax(box0.max().z(), box1.max().z())
        );

        return AABB(small, big);
    }

    bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis) {
        AABB box_a;
        AABB box_b;

        if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
            std::cerr << "[ERROR] No bounding box in bvh_node constructor.\n";

        return box_a.min()[axis] < box_b.min()[axis];
    }

    bool box_x_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
        return box_compare(a, b, 0);
    }

    bool box_y_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
        return box_compare(a, b, 1);
    }

    bool box_z_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
        return box_compare(a, b, 2);
    }
}