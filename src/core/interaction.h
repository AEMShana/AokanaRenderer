#pragma once

#include "vec.h"
#include "ray.h"
#include "material.h"
#include "shape.h"

namespace Asuka {
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
        SurfaceInteraction() {}
        SurfaceInteraction(const Point3& p, const Normal3& n, const Point2& uv, double time, bool front_face/*, const Shape* shape*/) :
            Interaction(p, n, time, front_face), uv(uv)/*, shape(shape)*/ {}

    public:
        Point2 uv;
        // const Shape* shape = nullptr;
        std::shared_ptr<Material> material;
    };
}