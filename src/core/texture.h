#pragma once

#include <memory>
#include "vec3.h"

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
}