#pragma once

#include <memory>
#include "vec.h"
#include "perlin.h"

namespace Aokana {

    class Texture {
    public:
        virtual Color Value(double u, double v, const Point3& p) const = 0;
    };

    class SolidColor : public Texture {
    public:
        SolidColor() = default;
        SolidColor(Color c) : color_value(c) {}
        SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

        virtual Color Value(double u, double v, const Point3& p) const override {
            return color_value;
        }

    private:
        Color color_value;
    };

    class CheckerTexture : public Texture {
    public:
        CheckerTexture() = default;
        CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) :
            even(_even), odd(_odd) {}
        CheckerTexture(Color c1, Color c2) :
            even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2)) {}

        virtual Color Value(double u, double v, const Point3& p) const override {
            auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
            if (sines < 0) return odd->Value(u, v, p);
            else return even->Value(u, v, p);
        }

    public:
        std::shared_ptr<Texture> odd;
        std::shared_ptr<Texture> even;
    };

    class NoiseTexture : public Texture {
    public:
        NoiseTexture() = default;
        NoiseTexture(double sc) : scale(sc) {}
        virtual Color Value(double u, double v, const Point3& p) const override {
            // return Color(1) * noise.turb(scale * p);
            return Color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z + 10 * noise.turb(p)));
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
        virtual Color Value(double u, double v, const Point3& p) const override;

    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;
    };
}