#pragma once

#include <memory>

#include "bounds.h"
#include "interaction.h"
// #include "transform.h"

namespace Asuka {

    class Material;
    class SurfaceInteraction;

    class Shape {
    public:
        Shape() : reverse_orientation(false), transform_swaps_handedness(false) {}
        // Shape(const Transform* object_to_world, const Transform* world_to_object, bool reverse_orientation) :
        //     object_to_world(object_to_world),
        //     world_to_object(world_to_object),
        //     reverse_orientation(reverse_orientation),
        //     transform_swaps_handedness(object_to_world->SwapsHandedness()) {}

        virtual bool Intersect(const Ray& ray, double t_min = 0.0001, double t_max = 1.0) const = 0;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = 1.0) const = 0;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const = 0;
        virtual Bounds3 WorldBound(double time0 = 0.0001, double time1 = 1.0) const = 0;

        // virtual Bounds3 object_bound() const = 0;   // 物体空间的边界框
        // virtual Bounds3 world_bound() const {       // 世界空间的边界框
        //     return object_to_world->Apply(object_bound());
        // }

        // virtual double area() const = 0;

    public:
        // const Transform* object_to_world;
        // const Transform* world_to_object;
        const bool reverse_orientation;             // 法线方向是否与默认的相反
        const bool transform_swaps_handedness;      // 变换是否改变了惯用手
    };

    class Sphere : public Shape {
    public:
        Sphere() : center(0, 0, 0), radius(0.5) {}
        Sphere(const Point3& center, double radius) :
            center(center), radius(radius) {}

        // Sphere(Point3 center, double radius,
        //     std::shared_ptr<Material> m,
        //     Transform* object_to_world,
        //     Transform* world_to_object,
        //     bool reverse_orientation = false) :
        //     Shape(object_to_world, world_to_object, reverse_orientation),
        //     center(center), radius(radius), material(m) {}

        virtual bool Intersect(const Ray& ray, double t_min = 0.0001, double t_max = 1.0) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = 1.0) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
        virtual Bounds3 WorldBound(double time0 = 0.0001, double time1 = 1.0) const override;

        // virtual Bounds3 object_bound() const override;

    public:
        Point3 center;
        double radius;

    private:
        static void get_sphere_uv(const Normal3& p, double& u, double& v);

    };

    class Triangle : public Shape {
    public:
        Triangle(const Point3& a, const Point3& b, const Point3& c) :
            a(a), b(b), c(c) {}

        // Triangle(
        //     Point3 _a, Point3 _b, Point3 _c,
        //     std::shared_ptr<Material> m,
        //     Transform* object_to_world,
        //     Transform* world_to_object,
        //     bool reverse_orientation) :
        //     Shape(object_to_world, world_to_object, reverse_orientation),
        //     a(_a), b(_b), c(_c), material(m) {}

        virtual bool Intersect(const Ray& ray, double t_min = 0.0001, double t_max = 1.0) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = 1.0) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
        virtual Bounds3 WorldBound(double time0 = 0.0001, double time1 = 1.0) const override;

        // virtual Bounds3 object_bound() const override;
        // virtual Bounds3 world_bound() const override;
    private:
        // void get_uv(const Point3& p, double& u, double& v) const;
    public:
        Point3 a, b, c;
        double u_a, v_a;
        double u_b, v_b;
        double u_c, v_c;
    };

}