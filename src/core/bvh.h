#pragma once

#include <algorithm>
#include <memory>
#include "shape.h"
#include "bounds.h"

namespace Asuka {

    class BVHNode : public Shape {
    public:
        BVHNode() = delete;
        BVHNode(const std::shared_ptr<ShapeList>& list, double time0, double time1) :
            BVHNode(list->shapes, 0, list->shapes.size(), time0, time1) {}
        BVHNode(const std::vector<std::shared_ptr<Shape>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
        virtual Bounds3 WorldBound() const override;

    public:
        std::shared_ptr<Shape> left;
        std::shared_ptr<Shape> right;
        Bounds3 box;
    };

}