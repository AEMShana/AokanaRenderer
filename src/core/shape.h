#pragma once

#include <memory>

#include "ray.h"
#include "bounds.h"
#include "interaction.h"
#include "transform.h"

namespace Asuka {
    class Shape {
    public:
        Shape() = default;
        Shape(const Transform *object_to_world, const Transform *world_to_object, bool reverse_orientation) :
            object_to_world(object_to_world),
            world_to_object(world_to_object),
            reverse_orientation(reverse_orientation),
            transform_swaps_handedness(object_to_world->SwapsHandedness()) {}

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const = 0;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const = 0;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const = 0;
        
        virtual bool Intersect(const Ray& r, double *t_hit, SurfaceInteraction *isect, bool test_alpha_texture = true) const = 0;        
        virtual bool IntersectP(const Ray& r, bool test_alpha_texture = true) const = 0;        
        
        virtual Bounds3 ObjectBound() const = 0;   // 物体空间的边界框
        virtual Bounds3 WorldBound() const {       // 世界空间的边界框
            return object_to_world->Apply(ObjectBound());
        }

        virtual double area() const = 0;

    public:
        const Transform *object_to_world;
        const Transform *world_to_object;
        const bool reverse_orientation;          // 法线方向是否与默认的相反
        const bool transform_swaps_handedness;    // 变换是否改变了惯用手
    };

    class ShapeList : public Shape {
    public:
        ShapeList() = default;
        ShapeList(std::shared_ptr<Shape> shape) { add(shape); }

        void clean() { shapes.clear(); }
        void add(std::shared_ptr<Shape> shape) { shapes.push_back(shape); }

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
        void load_obj(const std::string& filepath, std::shared_ptr<Material> material);

    public:
        std::vector<std::shared_ptr<Shape>> shapes;
        Transform transform;
    };

    class Sphere : public Shape {
    public:
        Point3 center;
        double radius;
        std::shared_ptr<Material> material;

        Sphere() : center(0, 0, 0), radius(0.5) {}
        Sphere(Point3 center, double radius, std::shared_ptr<Material> m)
            : center(center), radius(radius), material(m) {}

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;

    private:
        static void get_sphere_uv(const Normal3& p, double& u, double& v);

    };

    class Triangle : public Shape {
    public:
        Triangle(Point3 _a, Point3 _b, Point3 _c, std::shared_ptr<Material> m) : a(_a), b(_b), c(_c), material(m) {}

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, Bounds3& output_box) const override;
    private:
        // void get_uv(const Point3& p, double& u, double& v) const;
    public:
        Point3 a, b, c;
        double u_a, v_a;
        double u_b, v_b;
        double u_c, v_c;
        std::shared_ptr<Material> material;
    };

}