#include "shape.h"
#include "../obj_loader/OBJ_Loader.h"

#include <algorithm>
#include <string>

namespace Asuka {

    bool Sphere::Intersect(const Ray& ray, double t_min, double t_max) const {
        Vector3 oc = ray.origin() - center;
        double a = ray.direction().LengthSquare();
        double half_b = Dot(oc, ray.direction());
        double c = oc.LengthSquare() - radius * radius;
        double discriminant = half_b * half_b - a * c;
        return (discriminant > 0);
    }

    bool Sphere::IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        Vector3 oc = ray.origin() - center;
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

        hit_point.time = root;
        hit_point.p = ray.at(hit_point.time);
        Normal3 outward_normal = Normal3((hit_point.p - center) / radius);
        hit_point.set_face_normal(ray, outward_normal);
        get_sphere_uv(outward_normal, hit_point.uv.x, hit_point.uv.y);

        return true;
    }

    bool Sphere::bounding_box(double time0, double time1, Bounds3& output_box) const {
        output_box = Bounds3(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
        return true;
    }

    Bounds3 Sphere::WorldBound() const {
        return Bounds3(
            center - Vector3(radius, radius, radius),
            center + Vector3(radius, radius, radius)
        );
    }

    // Bounds3 Sphere::object_bound() const {
    //     return Bounds3(
    //         Point3(-radius, -radius, -radius),
    //         Point3(radius, radius, radius)
    //     );
    // }

    void Sphere::get_sphere_uv(const Normal3& p, double& u, double& v) {
        double theta = std::acos(-p.y);
        double phi = std::atan2(-p.z, p.x) + pi;
        u = phi / (2.0 * pi);
        v = theta / pi;
    }

    bool Triangle::Intersect(const Ray& ray, double t_min, double t_max) const {
        // Moller Trumbore Algorithm
        // o + t * d = (1 - p1 - p2) * a + p1 * b + p2 * c 
        auto e1 = b - a;
        auto e2 = c - a;
        auto s = ray.origin() - a;
        auto s1 = Cross(ray.direction(), e2);
        auto s2 = Cross(s, e1);
        auto div = 1.0 / Dot(s1, e1);
        auto t = div * Dot(s2, e2);
        auto p1 = div * Dot(s1, s);
        auto p2 = div * Dot(s2, ray.direction());
        auto p0 = 1.0 - p1 - p2;

        if (t < t_min || t > t_max) return false;
        if (p0 < 0 || p0 > 1 || p1 < 0 || p1 > 1 || p2 < 0 || p2 > 1) return false;

        return true;
    }

    bool Triangle::IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        // Moller Trumbore Algorithm
        // o + t * d = (1 - p1 - p2) * a + p1 * b + p2 * c 
        // a -> b -> c  clock-wise is front face

        auto e1 = b - a;
        auto e2 = c - a;
        auto s = ray.origin() - a;
        auto s1 = Cross(ray.direction(), e2);
        auto s2 = Cross(s, e1);
        auto div = 1.0 / Dot(s1, e1);
        auto t = div * Dot(s2, e2);
        auto p1 = div * Dot(s1, s);
        auto p2 = div * Dot(s2, ray.direction());
        auto p0 = 1.0 - p1 - p2;

        if (t < t_min || t > t_max) return false;
        if (p0 < 0 || p0 > 1 || p1 < 0 || p1 > 1 || p2 < 0 || p2 > 1) return false;

        hit_point.time = t;
        hit_point.p = ray.at(t);
        hit_point.uv.x = p0 * u_a + p1 * u_b + p2 * u_c;
        hit_point.uv.y = p0 * v_a + p1 * v_b + p2 * v_c;
        Normal3 outward_normal = Normal3(Normalize(Cross(b - a, c - a)));
        hit_point.set_face_normal(ray, outward_normal);

        return true;
    }

    bool Triangle::bounding_box(double time0, double time1, Bounds3& output_box) const {
        // add an offset, avoid the box become a plane
        Point3 p_min(
            std::min({ a.x, b.x, c.x }) - 0.0001,
            std::min({ a.y, b.y, c.y }) - 0.0001,
            std::min({ a.z, b.z, c.z }) - 0.0001);
        Point3 p_max(
            std::max({ a.x, b.x, c.x }) + 0.0001,
            std::max({ a.y, b.y, c.y }) + 0.0001,
            std::max({ a.z, b.z, c.z }) + 0.0001);
        output_box = Bounds3(p_min, p_max);
        return true;
    }

    Bounds3 Triangle::WorldBound() const {
        Point3 p_min(
            std::min({ a.x, b.x, c.x }) - 0.0001,
            std::min({ a.y, b.y, c.y }) - 0.0001,
            std::min({ a.z, b.z, c.z }) - 0.0001);
        Point3 p_max(
            std::max({ a.x, b.x, c.x }) + 0.0001,
            std::max({ a.y, b.y, c.y }) + 0.0001,
            std::max({ a.z, b.z, c.z }) + 0.0001);
        return Bounds3(p_min, p_max);
    }


    // Bounds3 Triangle::object_bound() const {
    //     Point3 p_min(
    //         std::min({ a.x, b.x, c.x }) - 0.0001,
    //         std::min({ a.y, b.y, c.y }) - 0.0001,
    //         std::min({ a.z, b.z, c.z }) - 0.0001);
    //     Point3 p_max(
    //         std::max({ a.x, b.x, c.x }) + 0.0001,
    //         std::max({ a.y, b.y, c.y }) + 0.0001,
    //         std::max({ a.z, b.z, c.z }) + 0.0001);
    //     return Bounds3(p_min, p_max);
    // }

    // Bounds3 Triangle::world_bound() const {
    //     Point3 world_a = object_to_world->Apply(a);
    //     Point3 world_b = object_to_world->Apply(b);
    //     Point3 world_c = object_to_world->Apply(c);

    //     Point3 p_min(
    //         std::min({ world_a.x, world_b.x, world_c.x }) - 0.0001,
    //         std::min({ world_a.y, world_b.y, world_c.y }) - 0.0001,
    //         std::min({ world_a.z, world_b.z, world_c.z }) - 0.0001);
    //     Point3 p_max(
    //         std::max({ world_a.x, world_b.x, world_c.x }) + 0.0001,
    //         std::max({ world_a.y, world_b.y, world_c.y }) + 0.0001,
    //         std::max({ world_a.z, world_b.z, world_c.z }) + 0.0001);

    //     return Bounds3(p_min, p_max);
    // }

    bool ShapeList::Intersect(const Ray& ray, double t_min, double t_max) const {
        for (const auto& shape : shapes) {
            if (shape->Intersect(ray, t_min, t_max)) return true;
        }
        return false;
    }

    bool ShapeList::IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        double closest_so_far = t_max;
        bool hit_anything = false;

        for (const auto& shape : shapes) {
            if (shape->IntersectP(ray, hit_point, t_min, closest_so_far)) {
                hit_anything = true;
                closest_so_far = hit_point.time;
            }
        }
        return hit_anything;
    }

    bool ShapeList::bounding_box(double time0, double time1, Bounds3& output_box) const {
        if (shapes.empty()) return false;
        Bounds3 temp_box;
        bool first_box = true;

        for (const auto& shape : shapes) {
            if (!shape->bounding_box(time0, time1, temp_box)) return false;
            output_box = first_box ? temp_box : Bounds3::merge(output_box, temp_box);
            first_box = false;
        }
        return true;
    }

    Bounds3 ShapeList::WorldBound() const {
        Bounds3 result{};
        bool first_box = true;
        for (const auto& shape : shapes) {
            result = first_box ? shape->WorldBound() : Bounds3::merge(result, shape->WorldBound());
            first_box = false;
        }
        return result;
    }

    void ShapeList::load_obj(const std::string& filepath, std::shared_ptr<Material> material) {
        objl::Loader objLoader;
        objLoader.LoadFile(filepath);
        for (auto& mesh : objLoader.LoadedMeshes) {
            for (int i = 2;i < mesh.Indices.size();i += 3) {
                Point3 a(
                    mesh.Vertices[mesh.Indices[i - 2]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Z);

                Point3 b(
                    mesh.Vertices[mesh.Indices[i - 1]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Z);

                Point3 c(
                    mesh.Vertices[mesh.Indices[i]].Position.X,
                    mesh.Vertices[mesh.Indices[i]].Position.Y,
                    mesh.Vertices[mesh.Indices[i]].Position.Z);

                shapes.push_back(std::make_shared<Triangle>(a, b, c, material));
            }
        }
    }
}