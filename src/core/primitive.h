#pragma once

#include "interaction.h"
#include "bounds.h"
#include "material.h"
#include "shape.h"

namespace Asuka {

    class SurfaceInteraction;
    class Shape;

    class Primitive {
    public:
        virtual bool Intersect(const Ray& ray, Interval ray_t = Interval::Universe) const = 0;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& isect, Interval ray_t = Interval::Universe) const = 0;

        virtual Bounds3 WorldBound(double time_0 = 0, double time_1 = inf) const = 0;
        virtual const Material* GetMaterial() const = 0;
    };

    class GeometricPrimitive : public Primitive {
    public:
        GeometricPrimitive(
            const std::shared_ptr<Shape>& shape,
            const std::shared_ptr<Material>& material) :
            shape(shape), material(material) {}

        virtual bool Intersect(const Ray& ray, Interval ray_t = Interval::Universe) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& isect, Interval ray_t = Interval::Universe) const override;
        virtual Bounds3 WorldBound(double time_0 = 0, double time_1 = inf) const override;
        virtual const Material* GetMaterial() const override;

    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
    };


    class Aggregate : public Primitive {
    public:
        Aggregate() = default;
        Aggregate(const std::vector<std::shared_ptr<Primitive>>& primitives);

        virtual bool Intersect(const Ray& ray, Interval ray_t = Interval::Universe) const override;
        virtual bool IntersectP(const Ray& ray, SurfaceInteraction& isect, Interval ray_t = Interval::Universe) const override;
        virtual Bounds3 WorldBound(double time_0 = 0, double time_1 = inf) const override;
        virtual const Material* GetMaterial() const override;

        void AddPrimitive(const std::shared_ptr<Primitive>& primitive);
        void AddPrimitives(const std::vector<std::shared_ptr<Primitive>>& primitive_list);
        void AddObj(const std::string& filepath, const std::shared_ptr<Material>& material, bool filp_x_axis = true);

    public:
        std::vector<std::shared_ptr<Primitive>> primitives;
        Bounds3 world_bound;
    };
}

