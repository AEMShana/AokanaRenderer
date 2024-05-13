#include "film.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

namespace Aokana {
    void Film::write_color(Color pixel_color, int u, int v) {
        v = image_height - 1 - v;
        int index = (v * image_width + u) * 3;

        // Divide the Color by the number of samples and gamma-correct for gamma=2.0.
        double r = pixel_color.x;
        double g = pixel_color.y;
        double b = pixel_color.z;

        r = sqrt(r);
        g = sqrt(g);
        b = sqrt(b);

        data[index + 0] = static_cast<unsigned char>(255.99 * r);
        data[index + 1] = static_cast<unsigned char>(255.99 * g);
        data[index + 2] = static_cast<unsigned char>(255.99 * b);
    }

    void Film::save_image(std::string path) const {
        std::cout << "[INFO] Film was successfully written to " << "\"" << path << "\"." << std::endl;
        stbi_write_png(path.c_str(), image_width, image_height, 3, data.data(), 0);
    }
}