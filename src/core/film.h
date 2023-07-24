#pragma once

#include <vector>
#include <string>

#include "vec3.h"

namespace Asuka {

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
        const int block_num = 16;
        std::vector<unsigned char> data;
        std::vector<FilmTile> tiles;

        Film() {
            data = std::vector<unsigned char>(image_width * image_height * channels);
            divide_tiles(block_num);
        }
        Film(int width, int height) : image_width(width), image_height(height) {
            data = std::vector<unsigned char>(image_width * image_height * channels);
            divide_tiles(block_num);
        }
        void divide_tiles(int num) {
            int tile_width = (image_width - 1) / num + 1;
            int tile_height = (image_height - 1) / num + 1;

            for (int i = 0;i < num;++i) {
                int u0 = tile_width * i;
                int u1 = std::min(u0 + tile_width - 1, image_width - 1);
                for (int j = 0;j < num;++j) {
                    int v0 = tile_height * j;
                    int v1 = std::min(v0 + tile_height - 1, image_height - 1);
                    FilmTile tile(u0, v0, u1, v1);
                    tiles.push_back(tile);
                }
            }
        }
        void write_color(color pixel_color, int u, int v);
        void save_image(std::string path) const;
    };
}