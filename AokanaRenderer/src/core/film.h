#pragma once

#include <vector>
#include <string>

#include "vec.h"

namespace Aokana {

    class FilmTile {
    public:
        FilmTile() = default;
        FilmTile(double u0, double v0, double u1, double v1) :
            u_min(u0), v_min(v0), u_max(u1), v_max(v1) {}
        double u_min, u_max;
        double v_min, v_max;
    };

    class Film {
    public:
        const int image_width = 512;
        const int image_height = 512;
        const int channels = 3;
        const int block_num = 2;
        const int tile_size = 16;
        std::vector<unsigned char> data;
        std::vector<FilmTile> tiles;

        Film() {
            data = std::vector<unsigned char>(image_width * image_height * channels);
            DivideTiles();
        }
        Film(int width, int height) : image_width(width), image_height(height) {
            data = std::vector<unsigned char>(image_width * image_height * channels);
            DivideTiles();
        }
        void DivideTiles();
        void WriteColor(Color PIxel_color, int u, int v);
        void SaveImage(std::string path) const;
    };
}