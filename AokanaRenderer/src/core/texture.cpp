#pragma once

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace Aokana {

    ImageTexture::ImageTexture(const std::string& filename) {
        auto channels_per_pixel = bytes_per_pixel;
        data = stbi_load(filename.c_str(), &width, &height, &channels_per_pixel, channels_per_pixel);
        if (!data) {
            std::cerr << "[ERROR] Could not load texture image file \"" << filename << "\".\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }

    Color ImageTexture::value(double u, double v, const Point3& p) const {
        // If we have no texture data, then return solid purple as a debugging aid.
        if (data == nullptr) return Color(1, 0, 1);
        u = Clamp(u, 0.0, 1.0);
        v = 1.0 - Clamp(v, 0.0, 1.0); // flip v to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto Color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Color(Color_scale * pixel[0], Color_scale * pixel[1], Color_scale * pixel[2]);
    }
}