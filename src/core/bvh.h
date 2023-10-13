#pragma once

#include <algorithm>
#include <memory>
#include "primitive.h"
#include "bounds.h"

namespace Asuka {

    class BVHNode : public Primitive {
    public:
        BVHNode() = delete;
        BVHNode(const std::shared_ptr<Aggregate>& aggregate, double time0, double time1) :
            BVHNode(aggregate->primitives, 0, aggregate->primitives.size(), time0, time1) {}
        BVHNode(const std::vector<std::shared_ptr<Primitive>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool Intersect(const Ray& ray, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min = 0.0001, double t_max = inf) const override;
        // virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
        virtual Bounds3 WorldBound(double time_0 = 0, double time_1 = 0) const override;
        virtual const Material* GetMaterial() const override;


    public:
        std::shared_ptr<Primitive> left;
        std::shared_ptr<Primitive> right;
        Bounds3 box;
    };

}