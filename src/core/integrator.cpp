#include "integrator.h"
#include "thread_pool.h"

#include "ui/ui.h"

#include <chrono>
#include <thread>

namespace Asuka {

    // Color SamplerIntegrator::Li(const Ray& ray, int depth) {
    //     if (depth <= 0) return Color(0, 0, 0);

    //     SurfaceInteraction hit_point;
    //     if (scene->bvh->hitP(ray, hit_point)) {
    //         Ray scattered;
    //         Color attenuation;
    //         if (hit_point.material->scatter(ray, hit_point, attenuation, scattered))
    //             return attenuation * Li(scattered, depth - 1);
    //         return Color(0, 0, 0);
    //     }

    //     vec3 unit_dir = normalize(ray.dir);
    //     double t = 0.5 * (unit_dir.y() + 1.0);
    //     return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    // }

    Color SamplerIntegrator::Li(const Ray& ray, const Color& background, int depth) {
        if (depth <= 0) return Color(0, 0, 0);
        SurfaceInteraction hit_point;
        if (!scene->bvh->hitP(ray, hit_point)) return background;
        Ray scattered;
        Color attenuation;
        Color emitted = hit_point.material->emitted(hit_point.uv.x, hit_point.uv.y, hit_point.p);
        if (!hit_point.material->scatter(ray, hit_point, attenuation, scattered))
            return emitted;

        return emitted + PairwiseMul(attenuation, Li(scattered, background, depth - 1));
    }


    void SamplerIntegrator::Render(const Camera& camera) {
        auto start_time = std::chrono::system_clock::now();

        std::shared_ptr<Film> film = camera.film;

        const int total_pixel = film->image_width * film->image_height;

        for (int j = 0; j < film->image_height;++j) {
            for (int i = 0;i < film->image_width;++i) {
                Color radiance;
                auto samples = sampler->sample();
                for (auto& sample : samples) {
                    sample.u = static_cast<double>(i + sample.u) / static_cast<double>(film->image_width - 1);
                    sample.v = static_cast<double>(j + sample.v) / static_cast<double>(film->image_height - 1);
                    Ray ray = camera.get_ray(sample);
                    Color r = Li(ray, scene->background, max_depth);
                    radiance += r;
                }
                radiance /= static_cast<double>(sampler->samples_per_pixel);
                radiance = Clamp(radiance);
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
                Color radiance;
                auto samples = sampler->sample();
                for (auto& sample : samples) {
                    sample.u = static_cast<double>(i + sample.u) / static_cast<double>(film->image_width - 1);
                    sample.v = static_cast<double>(j + sample.v) / static_cast<double>(film->image_height - 1);
                    Ray ray = camera.get_ray(sample);
                    Color r = Li(ray, scene->background, max_depth);
                    radiance += r;
                }
                radiance /= static_cast<double>(sampler->samples_per_pixel);
                radiance = Clamp(radiance);
                film->write_color(radiance, i, j);
            }
        }

    }

    void SamplerIntegrator::RenderWithMultithreading(const Camera& camera, bool enable_gui) {
        std::cout << "[INFO] Rendering start." << std::endl;
        auto start_time = std::chrono::system_clock::now();

        BS::thread_pool pool;
        std::shared_ptr<Film> film = camera.film;

        if (enable_gui) {
            GLFWwindow* window = UI::create_gui_window();
            std::shared_ptr<UI::Image> image = std::make_shared<UI::Image>(film->image_width, film->image_height);

            std::vector<int> flags(film->tiles.size());

            int tile_index = 0;
            for (const auto& tile : film->tiles) {
                pool.push_task(
                    [&](int index) {
                        RenderOneTile(camera, tile);
                        flags[index] = 1;
                        printf("tile %d finish.\n", index);
                    },
                    tile_index++
                );
            }

            glViewport(0, 0, image->width, image->height);
            glfwSetWindowSize(window, image->width, image->height);

            std::shared_ptr<std::vector<unsigned char>> data = std::make_shared<std::vector<unsigned char>>(image->width * image->height * 3);


            while (!glfwWindowShouldClose(window)) {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);


                bool modified = false;
                for (int i = 0;i < flags.size();++i) {
                    if (flags[i] == 1) {
                        modified = true;
                        auto u_min = static_cast<int>(film->tiles[i].u_min);
                        auto v_min = static_cast<int>(film->tiles[i].v_min);
                        auto u_max = static_cast<int>(film->tiles[i].u_max);
                        auto v_max = static_cast<int>(film->tiles[i].v_max);
                        int num = (u_max - u_min + 1) * (v_max - v_min + 1);

                        // assert(u_max - u_min + 1 == film->tile_size);
                        // assert(v_max - v_min + 1 == film->tile_size);

                        int tile_width = u_max - u_min + 1;
                        int tile_height = v_max - v_min + 1;

                        // std::vector<unsigned char> image_data(num * 3);
                        std::vector<unsigned char> image_data;
                        for (int v = v_min, j = 0;v <= v_max;++v, ++j) {
                            for (int u = u_min, i = 0;u <= u_max;++u, ++i) {
                                image_data.push_back(film->data[(v * film->image_width + u) * 3 + 0]);
                                image_data.push_back(film->data[(v * film->image_width + u) * 3 + 1]);
                                image_data.push_back(film->data[(v * film->image_width + u) * 3 + 2]);

                                (*data)[(v * film->image_width + u) * 3 + 0] = film->data[(v * film->image_width + u) * 3 + 0];
                                (*data)[(v * film->image_width + u) * 3 + 1] = film->data[(v * film->image_width + u) * 3 + 1];
                                (*data)[(v * film->image_width + u) * 3 + 2] = film->data[(v * film->image_width + u) * 3 + 2];
                            }
                        }
                        // std::cout << u_min << " : " << v_min << " : " << u_max << " : " << v_max << std::endl;
                        // image->modify_subimage(
                        //     u_min,
                        //     v_min,
                        //     u_max,
                        //     v_max,
                        //     image_data);
                        // flags[i] = 2;
                        // break;
                    }
                }

                if (modified) {
                    image = std::make_shared<UI::Image>(film->image_width, film->image_height, data->data());
                }

                image->draw();
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            glfwTerminate();
            pool.wait_for_tasks();

            // UI::draw_gui(window, image);

        }
        else {
            int tile_index = 0;
            for (const auto& tile : film->tiles) {
                pool.push_task(
                    [&](int index) {
                        RenderOneTile(camera, tile);
                        printf("tile %d finish.\n", index);
                    },
                    tile_index++
                );
                // pool.push_task(&SamplerIntegrator::RenderOneTile, this, camera, tile);
            }
            pool.wait_for_tasks();

        }


        auto end_time = std::chrono::system_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        // std::cout << "[INFO] Render finished. Rendering take " << duration.count() << " seconds." << std::endl;

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "[INFO] Render finished. Rendering took " << duration.count() << " ms." << std::endl;

    }

}