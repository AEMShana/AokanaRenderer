#include "material.h"
#include "interaction.h"

namespace Aokana {
    bool Lambertian::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        Color& attenuation, Ray& scattered) const {

        Vector3 scatter_direction = Normalize(Vector3(hit_point.normal) + Vector3::RandomUnitVector());

        if (scatter_direction.near_zero())
            scatter_direction = Vector3(hit_point.normal);

        scattered = Ray(hit_point.p, scatter_direction, ray_in.time);
        attenuation = albedo->value(hit_point.uv.x, hit_point.uv.y, hit_point.p);
        return true;
    }

    bool Metal::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        Color& attenuation, Ray& scattered) const {

        Vector3 reflected = Reflect(Normalize(ray_in.direction()), hit_point.normal);
        scattered = Ray(hit_point.p, reflected + fuzz * Vector3::RandomInUnitSphere(), ray_in.time);
        attenuation = albedo;
        return (Dot(scattered.direction(), hit_point.normal) > 0);
    }

    bool Dielectric::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        Color& attenuation, Ray& scattered) const {

        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = hit_point.front_face ? (1.0 / ir) : ir;
        Vector3 unit_direction = Normalize(ray_in.direction());

        double cos_theta = std::min(Dot(-unit_direction, hit_point.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) direction = Reflect(unit_direction, hit_point.normal);
        else direction = Refract(unit_direction, hit_point.normal, refraction_ratio);

        scattered = Ray(hit_point.p, direction, ray_in.time);
        return true;
    }

    double Dielectric::reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
    }

}