#include <iostream>
#include <string>
#include "core/integrator.h"

using namespace std;


int main() {
    using namespace Asuka;

    Camera camera = default_camera();
    SamplerIntegrator integrator;

    integrator.max_depth = 20;
    integrator.sampler = std::make_shared<SimpleSampler>();
    integrator.sampler->samples_per_pixel = 50;

    int scene_id = 0;

    std::cout << "[INFO] please input scene id:\n" <<
        "0: random_ball_scene\n" <<
        "1: two_spheres_scene\n" <<
        "2: two_perlin_spheres_scene\n" <<
        "3: earth_scene\n";

    std::cin >> scene_id;

    switch (scene_id) {
    case 0: integrator.scene = random_ball_scene(); break;
    case 1: integrator.scene = two_spheres_scene(); break;
    case 2: integrator.scene = two_perlin_spheres_scene(); break;
    case 3: integrator.scene = earth_scene(); break;
    default: integrator.scene = random_ball_scene(); break;
    }



    // auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left = std::make_shared<Dielectric>(1.5);
    // auto material_right = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 1.0);

    // integrator.scene = std::make_shared<Scene>();
    // auto shapeList = std::make_shared<ShapeList>();
    // shapeList->add(std::make_shared<Sphere>(point3(0.0, -100.5, 1.0), 100.0, material_ground));
    // shapeList->add(std::make_shared<Sphere>(point3(0.0, 0.0, 1.0), 0.5, material_center));
    // shapeList->add(std::make_shared<Sphere>(point3(-1.0, 0.0, 1.0), 0.5, material_left));
    // shapeList->add(std::make_shared<Sphere>(point3(-1.0, 0.0, 1.0), -0.4, material_left));
    // shapeList->add(std::make_shared<Sphere>(point3(1.0, 0.0, 1.0), 0.5, material_right));
    // integrator.scene->objects = shapeList;

    // integrator.Render(camera);
    integrator.RenderWithMultithreading(camera);
    camera.save_image("../../../../outputs/result.png");


    return 0;
}
