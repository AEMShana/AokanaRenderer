#pragma once

#include "material.h"
#include "shape.h"

namespace Asuka {

    class MovingSphere : public Shape {
    public:
        MovingSphere() = default;

        MovingSphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<Material> m) :
            center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), material(m) {};

        point3 center(double time) const;

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        std::shared_ptr<Material> material;
    };

}