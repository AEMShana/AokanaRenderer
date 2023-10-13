#pragma once

#include "material.h"
#include "shape.h"

namespace Asuka {

    class MovingSphere : public Shape {
    public:
        MovingSphere() = default;

        MovingSphere(
            Point3 cen0, Point3 cen1, double _time0, double _time1, double r) :
            center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r) {};

        Point3 center(double time) const;

        virtual bool Intersect(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double _time0, double _time1, Bounds3& output_box) const override;
        virtual Bounds3 WorldBound(double time0 = 0, double time1 = inf) const override;

    public:
        Point3 center0, center1;
        double time0, time1;
        double radius;
    };

}