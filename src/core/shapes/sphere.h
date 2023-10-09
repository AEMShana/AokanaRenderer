#pragma once

#include "../shape.h"

namespace Asuka::ShapeV2 {
    
    class Sphere : public Shape {
    public:
        Sphere(const Transform *object_to_world, const Transform *world_to_object,
        bool reverse_orientation, double radius, double z_min, double z_max, double phi_max) :
            Shape(object_to_world, world_to_object, reverse_orientation),
            radius(radius),
            z_min(Clamp(std::min(z_min, z_max), -radius, radius)),
            z_max(Clamp(std::max(z_min, z_max), -radius, radius)),
            theta_min(std::acos(Clamp(z_min / radius, -1, 1))),
            theta_max(std::acos(Clamp(z_max / radius, -1, 1))),
            phi_max(Radians(Clamp(phi_max, 0, 360))) {}

        Bounds3 ObjectBound() const override {
            return Bounds3(
                Point3(-radius, -radius, z_min),
                Point3(radius, radius, z_max)
            );
        }

        bool Intersect(const Ray& r, double *t_hit, SurfaceInteraction *isect, bool test_alpha_texture = true) const override {
            double phi;
            Point3 p_hit;

            // Vector3 o_err, d_err;
            Ray ray = world_to_object->Apply(r);
            
            double ox = ray.direction().x;
            double oy = ray.direction().y;
            double oz = ray.direction().z;

            double dx = ray.direction().x;
            double dy = ray.direction().y;
            double dz = ray.direction().z;

            double a = Square(dx) + Square(dy) + Square(dz);
            double b = 2 * (dx * ox + dy * oy + dz * oz);
            double c = Square(ox) + Square(oy) + Square(oz) - Square(radius);

            double t0, t1;
            if(!Quadratic(a, b, c, &t0, &t1)) return false;
            if(t0 > ray.tMax || t1 <= 0) return false;
            double t_shape_hit = t0;
            if(t_shape_hit <= 0) {
                t_shape_hit = t1;
                if(t_shape_hit > ray.tMax) return false;
            }


        }        

    private:
        const double radius;
        // 设置截断
        const double z_min, z_max;
        const double theta_min, theta_max, phi_max; // 均使用弧度制
        // theta in [0, pi]
        // phi in [0, 2pi]
    };


}