#pragma once

#include <memory>

#include "ray.h"
#include "aabb.h"
#include "interaction.h"
#include "transform.h"

namespace Asuka {
    class AABB;

    class Shape {
    public:
        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const = 0;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const = 0;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
    };

    class ShapeList : public Shape {
    public:
        ShapeList() = default;
        ShapeList(std::shared_ptr<Shape> shape) { add(shape); }

        void clean() { shapes.clear(); }
        void add(std::shared_ptr<Shape> shape) { shapes.push_back(shape); }

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
        void load_obj(const std::string& filepath, std::shared_ptr<Material> material);

    public:
        std::vector<std::shared_ptr<Shape>> shapes;
        Transform transform;
    };

    class Sphere : public Shape {
    public:
        point3 center;
        double radius;
        std::shared_ptr<Material> material;

        Sphere() : center(0, 0, 0), radius(0.5) {}
        Sphere(point3 center, double radius, std::shared_ptr<Material> m)
            : center(center), radius(radius), material(m) {}

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

    private:
        static void get_sphere_uv(const point3& p, double& u, double& v);

    };

    class Triangle : public Shape {
    public:
        Triangle(point3 _a, point3 _b, point3 _c, std::shared_ptr<Material> m) : a(_a), b(_b), c(_c), material(m) {}

        virtual bool hit(const Ray& ray, double t_min = 0, double t_max = inf) const override;
        virtual bool hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min = 0.0001, double t_max = inf) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
    private:
        // void get_uv(const point3& p, double& u, double& v) const;
    public:
        point3 a, b, c;
        double u_a, v_a;
        double u_b, v_b;
        double u_c, v_c;
        std::shared_ptr<Material> material;
    };

}