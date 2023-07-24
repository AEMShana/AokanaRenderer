#pragma once

#include "shape.h"
#include "ray.h"
#include <memory>

namespace Asuka {
    class Shape;

    class AABB {
    public:
        AABB() = default;
        AABB(const point3& a, const point3& b) {
            minimum = a;
            maximum = b;
        }

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        bool hit(const Ray& r, double t_min, double t_max) const;

        point3 minimum;
        point3 maximum;
    };

    /// @brief merge the box0 and box1
    /// @param box0 bounding box 0
    /// @param box1 bounding box 1
    /// @return merge result
    AABB merge_box(const AABB& box0, const AABB& box1);

    bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis);
    bool box_x_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
    bool box_y_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
    bool box_z_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
}