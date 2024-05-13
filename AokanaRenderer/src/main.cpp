#include <iostream>
#include <string>
#include "core/integrator.h"
#include "core/matrix.h"

using namespace std;

void Render() {
    using namespace Aokana;

    Camera camera = default_camera();
    SamplerIntegrator integrator;

    integrator.max_depth = 5;
    integrator.sampler = std::make_shared<SimpleSampler>();
    integrator.sampler->samples_per_pixel = 10;

    int scene_id = 0;

    std::cout << "[INFO] please input scene id:\n" <<
        "0: random_ball_scene\n" <<
        "1: two_spheres_scene\n" <<
        "2: two_perlin_spheres_scene\n" <<
        "3: earth_scene\n" <<
        "4: simple_light_scene\n" <<
        "5: bunny_scene\n" <<
        // "6: coffee_maker_scene\n" <<
        "6: cornell_box_scene\n";


    std::cin >> scene_id;

    std::cout << "[INFO] please input max depth (default = 5):" << std::endl;
    std::cin >> integrator.max_depth;

    std::cout << "[INFO] please input samples per pixel (default = 10):" << std::endl;
    std::cin >> integrator.sampler->samples_per_pixel;

    std::cout << "[INFO] Display GUI? (0/1)" << std::endl;
    int use_gui;
    std::cin >> use_gui;

    switch (scene_id) {
    case 0: integrator.scene = std::make_shared<Scene>(SampleScene::RandomBallScene()); break;
    case 1: integrator.scene = std::make_shared<Scene>(SampleScene::TwoSpheresScene()); break;
    case 2: integrator.scene = std::make_shared<Scene>(SampleScene::TwoPerlinSpheresScene()); break;
    case 3: integrator.scene = std::make_shared<Scene>(SampleScene::EarthScene()); break;
    case 4: integrator.scene = std::make_shared<Scene>(SampleScene::SimpleLightScene()); break;
    case 5: integrator.scene = std::make_shared<Scene>(SampleScene::BunnyScene()); break;
    case 6: integrator.scene = std::make_shared<Scene>(SampleScene::CornellBox()); break;
    }


    // switch (scene_id) {
    // case 4: integrator.scene = simple_light_scene(); camera = simple_light_scene_camera(); break;
    // case 5: integrator.scene = bunny_scene(); camera = bunny_scene_camera(); break;
    // case 6: integrator.scene = coffee_maker_scene(); camera = coffee_maker_scene_camera(); break;
    // case 7: integrator.scene = cornell_box_scene(); camera = cornell_box_scene_camera(); break;
    // default: integrator.scene = random_ball_scene(); break;
    // }



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
    integrator.RenderWithMultithreading(use_gui);
    integrator.scene->camera.save_image("./output/result.png");
}


int main() {
    Render();

    return 0;
}
