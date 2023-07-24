#pragma once

#include "vec3.h"
#include "ray.h"
#include "material.h"

namespace Asuka {
    class Material;

    class Interaction {
    public:
        point3 p;
        vec3 normal;
        double t;
        double u;
        double v;
        bool front_face;

        void set_face_normal(const Ray& ray, const vec3& outward_normal) {
            front_face = dot(ray.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

    class SurfaceInteraction : public Interaction {
    public:
        std::shared_ptr<Material> material;

    };
}