#include "integrator.h"

namespace Asuka {

    color SamplerIntegrator::Li(const Ray& ray, int depth) {
        if (depth <= 0) return color(0, 0, 0);

        SurfaceInteraction hit_point;
        if (scene->objects->hitP(ray, hit_point)) {
            Ray scattered;
            color attenuation;
            if (hit_point.material->scatter(ray, hit_point, attenuation, scattered))
                return attenuation * Li(scattered, depth - 1);
            return color(0, 0, 0);
        }

        vec3 unit_dir = normalize(ray.dir);
        double t = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }

    void SamplerIntegrator::Render(const Camera& camera) {
        std::shared_ptr<Film> film = camera.film;

        const int total_pixel = film->image_width * film->image_height;

        for (int j = 0; j < film->image_height;++j) {
            for (int i = 0;i < film->image_width;++i) {
                color radiance;
                auto samples = sampler->sample();
                for (auto& sample : samples) {
                    sample.u = static_cast<double>(i + sample.u) / static_cast<double>(film->image_width - 1);
                    sample.v = static_cast<double>(j + sample.v) / static_cast<double>(film->image_height - 1);
                    Ray ray = camera.get_ray(sample);
                    color r = Li(ray, max_depth);
                    radiance += r;
                }
                radiance /= static_cast<double>(sampler->samples_per_pixel);
                radiance = clamp(radiance);
                film->write_color(radiance, i, j);

                int index = j * film->image_width + i;
                if ((index + 1) % (total_pixel / 20) == 0) {
                    double progress = static_cast<double>(index + 1) / static_cast<double>(total_pixel);
                    std::cout << "[INFO] Render Progress: " << static_cast<int>(progress * 100.0) << "%" << std::endl;
                }
            }
        }
        std::cout << "[INFO] Render finished." << std::endl;
    }

}