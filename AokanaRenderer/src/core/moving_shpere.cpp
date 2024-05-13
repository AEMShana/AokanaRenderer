#include "moving_sphere.h"

namespace Aokana {

    Point3 MovingSphere::center(double time) const {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

    bool MovingSphere::Intersect(const Ray& ray, double t_min, double t_max) const {
        Vector3 oc = ray.origin() - center(ray.time);
        double a = ray.direction().LengthSquare();
        double half_b = Dot(oc, ray.direction());
        double c = oc.LengthSquare() - radius * radius;
        double discriminant = half_b * half_b - a * c;
        return (discriminant > 0);
    }


    bool MovingSphere::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        Vector3 oc = ray.origin() - center(ray.time);
        double a = ray.direction().LengthSquare();
        double half_b = Dot(oc, ray.direction());
        double c = oc.LengthSquare() - radius * radius;
        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);
        double root = (-half_b - sqrtd) / a;

        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) return false;
        }

        isect.time = root;
        isect.p = ray.at(isect.time);
        Normal3 outward_normal = Normal3(isect.p - center(ray.time)) / radius;
        isect.set_face_normal(ray, outward_normal);

        return true;
    }

    bool MovingSphere::bounding_box(double _time0, double _time1, Bounds3& output_box) const {
        Bounds3 box0(center(_time0) - Vector3(radius, radius, radius), center(_time0) + Vector3(radius, radius, radius));
        Bounds3 box1(center(_time1) - Vector3(radius, radius, radius), center(_time1) + Vector3(radius, radius, radius));
        output_box = Bounds3::merge(box0, box1);
        return true;
    }

    Bounds3 MovingSphere::WorldBound(double time0, double time1) const {
        Bounds3 box0(center(time0) - Vector3(radius, radius, radius), center(time0) + Vector3(radius, radius, radius));
        Bounds3 box1(center(time1) - Vector3(radius, radius, radius), center(time1) + Vector3(radius, radius, radius));
        return Bounds3::merge(box0, box1);
    }
}