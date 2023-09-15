#include "integrator.h"
#include <chrono>
#include <thread>

namespace Asuka {

    // color SamplerIntegrator::Li(const Ray& ray, int depth) {
    //     if (depth <= 0) return color(0, 0, 0);

    //     SurfaceInteraction hit_point;
    //     if (scene->bvh->hitP(ray, hit_point)) {
    //         Ray scattered;
    //         color attenuation;
    //         if (hit_point.material->scatter(ray, hit_point, attenuation, scattered))
    //             return attenuation * Li(scattered, depth - 1);
    //         return color(0, 0, 0);
    //     }

    //     vec3 unit_dir = normalize(ray.dir);
    //     double t = 0.5 * (unit_dir.y() + 1.0);
    //     return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    // }

    color SamplerIntegrator::Li(const Ray& ray, const color& background, int depth) {
        if (depth <= 0) return color(0, 0, 0);
        SurfaceInteraction hit_point;
        if (!scene->bvh->hitP(ray, hit_point)) return background;
        Ray scattered;
        color attenuation;
        color emitted = hit_point.material->emitted(hit_point.u, hit_point.v, hit_point.p);
        if (!hit_point.material->scatter(ray, hit_point, attenuation, scattered))
            return emitted;

        return emitted + attenuation * Li(scattered, background, depth - 1);
    }


    void SamplerIntegrator::Render(const Camera& camera) {
        auto start_time = std::chrono::system_clock::now();

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
                    color r = Li(ray, scene->background, max_depth);
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

        auto end_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

        std::cout << "[INFO] Render finished. Rendering take " << duration.count() << " seconds." << std::endl;

    }

    void SamplerIntegrator::RenderOneTile(const Camera& camera, const FilmTile& tile) {
        std::shared_ptr<Film> film = camera.film;

        for (int j = tile.v_min; j <= tile.v_max;++j) {
            for (int i = tile.u_min;i <= tile.u_max;++i) {
                color radiance;
                auto samples = sampler->sample();
                for (auto& sample : samples) {
                    sample.u = static_cast<double>(i + sample.u) / static_cast<double>(film->image_width - 1);
                    sample.v = static_cast<double>(j + sample.v) / static_cast<double>(film->image_height - 1);
                    Ray ray = camera.get_ray(sample);
                    color r = Li(ray, scene->background, max_depth);
                    radiance += r;
                }
                radiance /= static_cast<double>(sampler->samples_per_pixel);
                radiance = clamp(radiance);
                film->write_color(radiance, i, j);
            }
        }

    }

    void SamplerIntegrator::RenderWithMultithreading(const Camera& camera) {
        std::cout << "[INFO] Rendering start." << std::endl;
        auto start_time = std::chrono::system_clock::now();

        std::shared_ptr<Film> film = camera.film;
        std::vector<std::thread> threads_vec;

        for (const auto& tile : film->tiles) {
            threads_vec.push_back(std::thread(&SamplerIntegrator::RenderOneTile, this, camera, tile));
        }

        for (auto& thread : threads_vec)
            thread.join();

        auto end_time = std::chrono::system_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        // std::cout << "[INFO] Render finished. Rendering take " << duration.count() << " seconds." << std::endl;

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "[INFO] Render finished. Rendering took " << duration.count() << " ms." << std::endl;

    }

}