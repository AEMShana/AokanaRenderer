#pragma once

#include "ray.h"
#include "interaction.h"
#include "texture.h"

namespace Asuka {
    class SurfaceInteraction;

    class Material {
    public:
        virtual color emitted(double u, double v, const point3& p) const {
            return color(0, 0, 0);
        }

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const = 0;
    };


    class Lambertian : public Material {
    public:
        Lambertian(const color& a) : albedo(std::make_shared<SolidColor>(a)) {}
        Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const override;
    public:
        std::shared_ptr<Texture> albedo;
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

    class DiffuseLight : public Material {
    public:
        DiffuseLight(std::shared_ptr<Texture> tex) : emit(tex) {}
        DiffuseLight(color c) : emit(std::make_shared<SolidColor>(c)) {}

        virtual bool scatter(const Ray& ray_in, const SurfaceInteraction& hit_point,
            color& attenuation, Ray& scattered) const override {
            return false;
        }

        virtual color emitted(double u, double v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        std::shared_ptr<Texture> emit;
    };
}