#pragma once

#include "ray.h"
#include "interaction.h"

namespace Asuka {
    class SurfaceInteraction;

    class Material {
    public:
        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const = 0;
    };


    class Lambertian : public Material {
    public:
        Lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const override;
    public:
        color albedo;
    };


    class Metal : public Material {
    public:
        Metal(const color& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const override;

    public:
        color albedo;
        double fuzz;
    };


    class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const override;

    public:
        double ir; // Index of Rafraction
    private:
        static double reflectance(double consine, double ref_idx);
    };
}