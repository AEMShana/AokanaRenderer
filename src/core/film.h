#pragma once

#include <vector>
#include <string>

#include "vec3.h"

namespace Asuka {

    class Film {
    public:
        const int image_width = 512;
        const int image_height = 512;
        const int channels = 3;
        std::vector<unsigned char> data;

        Film() {
            data = std::vector<unsigned char>(image_width * image_height * channels);
        }
        Film(int width, int height) : image_width(width), image_height(height) {
            data = std::vector<unsigned char>(image_width * image_height * channels);
        }
        void write_color(color pixel_color, int u, int v);
        void save_image(std::string path) const;
    };
}