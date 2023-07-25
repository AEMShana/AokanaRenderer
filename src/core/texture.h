#pragma once

#include <memory>
#include "vec3.h"
#include "perlin.h"

namespace Asuka {

    class Texture {
    public:
        virtual color value(double u, double v, const point3& p) const = 0;
    };

    class SolidColor : public Texture {
    public:
        SolidColor() = default;
        SolidColor(color c) : color_value(c) {}
        SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}

        virtual color value(double u, double v, const point3& p) const override {
            return color_value;
        }

    private:
        color color_value;
    };

    class CheckerTexture : public Texture {
    public:
        CheckerTexture() = default;
        CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) :
            even(_even), odd(_odd) {}
        CheckerTexture(color c1, color c2) :
            even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2)) {}

        virtual color value(double u, double v, const point3& p) const override {
            auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0) return odd->value(u, v, p);
            else return even->value(u, v, p);
        }

    public:
        std::shared_ptr<Texture> odd;
        std::shared_ptr<Texture> even;
    };

    class NoiseTexture : public Texture {
    public:
        NoiseTexture() = default;
        NoiseTexture(double sc) : scale(sc) {}
        virtual color value(double u, double v, const point3& p) const override {
            // return color(1) * noise.turb(scale * p);
            return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
        }

    public:
        Perlin noise;
        double scale;
    };

    class ImageTexture : public Texture {
    public:
        static const int bytes_per_pixel = 3;

        ImageTexture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
        ImageTexture(const std::string& filename);
        ~ImageTexture() { delete data; }
        virtual color value(double u, double v, const point3& p) const override;

    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;
    };
}