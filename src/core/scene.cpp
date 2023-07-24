#include "scene.h"
#include "moving_sphere.h"

namespace Asuka {
    std::shared_ptr<Scene> random_ball_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();
        auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
        objects->add(std::make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = random_double();
                point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

                if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                    std::shared_ptr<Material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = std::make_shared<Lambertian>(albedo);
                        auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                        objects->add(std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                    }
                    else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = color::random(0.5, 1);
                        auto fuzz = random_double(0, 0.5);
                        sphere_material = std::make_shared<Metal>(albedo, fuzz);
                        objects->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                    }
                    else {
                        // glass
                        sphere_material = std::make_shared<Dielectric>(1.5);
                        objects->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }

        auto material1 = std::make_shared<Dielectric>(1.5);
        objects->add(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
        objects->add(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
        objects->add(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;

        return scene;
    }


};