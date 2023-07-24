#include "material.h"

namespace Asuka {
    bool Lambertian::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        color& attenuation, Ray& scattered) const {

        vec3 scatter_direction = normalize(hit_point.normal + random_unit_vector());

        if (scatter_direction.near_zero())
            scatter_direction = hit_point.normal;

        scattered = Ray(hit_point.p, scatter_direction, ray_in.time());
        attenuation = albedo;
        return true;
    }

    bool Metal::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        color& attenuation, Ray& scattered) const {

        vec3 reflected = reflect(normalize(ray_in.direction()), hit_point.normal);
        scattered = Ray(hit_point.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), hit_point.normal) > 0);
    }

    bool Dielectric::scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
        color& attenuation, Ray& scattered) const {

        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = hit_point.front_face ? (1.0 / ir) : ir;
        vec3 unit_direction = normalize(ray_in.direction());

        double cos_theta = std::min(dot(-unit_direction, hit_point.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) direction = reflect(unit_direction, hit_point.normal);
        else direction = refract(unit_direction, hit_point.normal, refraction_ratio);

        scattered = Ray(hit_point.p, direction, ray_in.time());
        return true;
    }

    double Dielectric::reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
    }

}