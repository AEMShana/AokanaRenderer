#pragma once

#include "interaction.h"
#include "bounds.h"
#include "material.h"

namespace Asuka {

    class Primitive {
    public:
        virtual bool Intersect(const Ray& r, SurfaceInteraction*) const = 0;
        virtual bool IntersectP(const Ray& r) const = 0;

        // virtual const AreaLight* GetAreaLight() const = 0;
        virtual Bounds3 WorldBound() const = 0;
        virtual const Material* GetMaterial() const = 0;
    };

    class GeometricPrimitive : public Primitive {
    public:
        GeometricPrimitive(
            const std::shared_ptr<Shape>& shape,
            const std::shared_ptr<Material>& material) :
            shape(shape), material(material) {}

        virtual Bounds3 WorldBound() const override;
        virtual bool Intersect(const Ray& r, SurfaceInteraction*) const override;
        virtual bool IntersectP(const Ray& r) const override;

    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
    };


    class PrimitiveList : Primitive {
    public:
        virtual Bounds3 WorldBound() const override;
        virtual bool Intersect(const Ray& r, SurfaceInteraction*) const override;
        virtual bool IntersectP(const Ray& r) const override;
    public:
        std::vector<std::shared_ptr<Shape>> shapes;
    };
}

