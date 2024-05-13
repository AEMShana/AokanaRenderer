#pragma once

#include "vec.h"
#include "ray.h"
#include "material.h"
#include "primitive.h"

namespace Aokana {

    class Primitive;
    class Material;

    class Interaction {
    public:
        Interaction() = default;
        Interaction(const Point3& p, const Normal3& n, double time, bool front_face) :
            p(p), normal(n), time(time), front_face(front_face) {}

    public:
        Point3 p;
        Normal3 normal;
        double time;
        bool front_face;

        void set_face_normal(const Ray& ray, const Normal3& outward_normal) {
            front_face = Dot(ray.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

    class SurfaceInteraction : public Interaction {
    public:
        SurfaceInteraction() : primitive(nullptr), material(nullptr) {}
        SurfaceInteraction(
            const Point3& p, const Normal3& n, const Point2& uv,
            double time, bool front_face,
            const Primitive* primitive,
            const Material* material) :
            Interaction(p, n, time, front_face),
            uv(uv), primitive(primitive), material(material) {}

    public:
        Point2 uv;
        const Primitive* primitive;
        const Material* material;
    };
}