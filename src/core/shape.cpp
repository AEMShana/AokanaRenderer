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
                closest_so_far = hit_point.t;
            }
        }
        return hit_anything;
    }

    bool ShapeList::bounding_box(double time0, double time1, AABB& output_box) const {
        if (shapes.empty()) return false;
        AABB temp_box;
        bool first_box = true;

        for (const auto& shape : shapes) {
            if (!shape->bounding_box(time0, time1, temp_box)) return false;
            output_box = first_box ? temp_box : merge_box(output_box, temp_box);
            first_box = false;
        }
        return true;
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

        hit_point.t = root;
        hit_point.p = ray.at(hit_point.t);
        vec3 outward_normal = (hit_point.p - center) / radius;
        hit_point.set_face_normal(ray, outward_normal);
        get_sphere_uv(outward_normal, hit_point.u, hit_point.v);
        hit_point.material = material;

        return true;
    }

    bool Sphere::bounding_box(double time0, double time1, AABB& output_box) const {
        output_box = AABB(center - vec3(radius), center + vec3(radius));
        return true;
    }

    void Sphere::get_sphere_uv(const point3& p, double& u, double& v) {
        double theta = std::acos(-p.y());
        double phi = std::atan2(-p.z(), p.x()) + pi;
        u = phi / (2.0 * pi);
        v = theta / pi;
    }
}