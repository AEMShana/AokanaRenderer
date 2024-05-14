#include "film.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

namespace Aokana {

	void Film::DivideTiles() {
		for (int i = 0; i < (image_width - 1) / tile_size + 1; ++i) {
			int u0 = tile_size * i;
			int u1 = std::min(u0 + tile_size - 1, image_width - 1);
			for (int j = 0; j < (image_height - 1) / tile_size + 1; ++j) {
				int v0 = tile_size * j;
				int v1 = std::min(v0 + tile_size - 1, image_height - 1);
				FilmTile tile(u0, v0, u1, v1);
				tiles.push_back(tile);
			}
		}
	}

	void Film::WriteColor(Color PIxel_color, int u, int v) {
		v = image_height - 1 - v;
		int index = (v * image_width + u) * 3;

		// Divide the Color by the number of samples and gamma-correct for gamma=2.0.
		double r = PIxel_color.x;
		double g = PIxel_color.y;
		double b = PIxel_color.z;

		r = sqrt(r);
		g = sqrt(g);
		b = sqrt(b);

		data[index + 0] = static_cast<unsigned char>(255.99 * r);
		data[index + 1] = static_cast<unsigned char>(255.99 * g);
		data[index + 2] = static_cast<unsigned char>(255.99 * b);
	}

	void Film::SaveImage(std::string path) const {
		std::cout << "[INFO] Film was successfully written to " << "\"" << path << "\"." << std::endl;
		stbi_write_png(path.c_str(), image_width, image_height, 3, data.data(), 0);
	}
}