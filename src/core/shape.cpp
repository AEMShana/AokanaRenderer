#include "shape.h"
#include "../obj_loader/OBJ_Loader.h"

#include <algorithm>
#include <string>

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

    void ShapeList::load_obj(const std::string& filepath, std::shared_ptr<Material> material) {
        objl::Loader objLoader;
        objLoader.LoadFile(filepath);
        for (auto& mesh : objLoader.LoadedMeshes) {
            for (int i = 2;i < mesh.Indices.size();i += 3) {
                point3 a(
                    mesh.Vertices[mesh.Indices[i - 2]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Z);

                point3 b(
                    mesh.Vertices[mesh.Indices[i - 1]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Z);

                point3 c(
                    mesh.Vertices[mesh.Indices[i]].Position.X,
                    mesh.Vertices[mesh.Indices[i]].Position.Y,
                    mesh.Vertices[mesh.Indices[i]].Position.Z);

                shapes.push_back(std::make_shared<Triangle>(a, b, c, material));
            }
        }
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

    bool Triangle::hit(const Ray& ray, double t_min, double t_max) const {
        // Moller Trumbore Algorithm
        // o + t * d = (1 - p1 - p2) * a + p1 * b + p2 * c 
        auto e1 = b - a;
        auto e2 = c - a;
        auto s = ray.origin() - a;
        auto s1 = cross(ray.direction(), e2);
        auto s2 = cross(s, e1);
        auto div = 1.0 / dot(s1, e1);
        auto t = div * dot(s2, e2);
        auto p1 = div * dot(s1, s);
        auto p2 = div * dot(s2, ray.direction());
        auto p0 = 1.0 - p1 - p2;

        if (t < t_min || t > t_max) return false;
        if (p0 < 0 || p0 > 1 || p1 < 0 || p1 > 1 || p2 < 0 || p2 > 1) return false;

        return true;
    }

    bool Triangle::hitP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        // Moller Trumbore Algorithm
        // o + t * d = (1 - p1 - p2) * a + p1 * b + p2 * c 
        // a -> b -> c  clock-wise is front face

        auto e1 = b - a;
        auto e2 = c - a;
        auto s = ray.origin() - a;
        auto s1 = cross(ray.direction(), e2);
        auto s2 = cross(s, e1);
        auto div = 1.0 / dot(s1, e1);
        auto t = div * dot(s2, e2);
        auto p1 = div * dot(s1, s);
        auto p2 = div * dot(s2, ray.direction());
        auto p0 = 1.0 - p1 - p2;

        if (t < t_min || t > t_max) return false;
        if (p0 < 0 || p0 > 1 || p1 < 0 || p1 > 1 || p2 < 0 || p2 > 1) return false;

        hit_point.t = t;
        hit_point.p = ray.at(t);
        hit_point.u = p0 * u_a + p1 * u_b + p2 * u_c;
        hit_point.v = p0 * v_a + p1 * v_b + p2 * v_c;
        hit_point.material = material;
        vec3 outward_normal = normalize(cross(b - a, c - a));
        hit_point.set_face_normal(ray, outward_normal);

        return true;
    }

    bool Triangle::bounding_box(double time0, double time1, AABB& output_box) const {
        // add an offset, avoid the box become a plane
        point3 small(
            std::min({ a.x(), b.x(), c.x() }) - 0.0001,
            std::min({ a.y(), b.y(), c.y() }) - 0.0001,
            std::min({ a.z(), b.z(), c.z() }) - 0.0001);
        point3 big(
            std::max({ a.x(), b.x(), c.x() }) + 0.0001,
            std::max({ a.y(), b.y(), c.y() }) + 0.0001,
            std::max({ a.z(), b.z(), c.z() }) + 0.0001);
        output_box = AABB(small, big);
        return true;
    }

}