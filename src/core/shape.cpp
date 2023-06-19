#include "shape.h"

namespace Asuka {
    bool ShapeList::hit(const Ray& ray, double t_min, double t_max) const {
        for (const auto& shape : shapes) {
            if (shape->hit(ray, t_min, t_max)) return true;
        }
        return false;
    }

    bool ShapeList::hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        double closest_so_far = t_max;
        bool hit_anything = false;

        for (const auto& shape : shapes) {
            if (shape->hitP(ray, hit_point, t_min, closest_so_far)) {
                hit_anything = true;
                closest_so_far = hit_point.time;
            }
        }
        return hit_anything;
    }

    bool Sphere::hit(const Ray& ray, double t_min, double t_max) const {
        vec3 oc = ray.origin() - center;
        double a = ray.direction().length_squared();
        double half_b = dot(oc, ray.direction());
        double c = oc.length_squared() - radius * radius;
        double discriminant = half_b * half_b - a * c;
        return (discriminant > 0);
    }

    bool Sphere::hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        vec3 oc = ray.origin() - center;
        double a = ray.direction().length_squared();
        double half_b = dot(oc, ray.direction());
        double c = oc.length_squared() - radius * radius;
        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);
        double root = (-half_b - sqrtd) / a;

        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) return false;
        }

        hit_point.time = root;
        hit_point.p = ray.at(hit_point.time);
        vec3 outward_normal = (hit_point.p - center) / radius;
        hit_point.set_face_normal(ray, outward_normal);
        hit_point.material = material;

        return true;
    }
}