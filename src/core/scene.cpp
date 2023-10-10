#include "scene.h"
#include "moving_sphere.h"

#include <chrono>

namespace Asuka {
    std::shared_ptr<Scene> random_ball_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();
        auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
        objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = random_double();
                Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

                if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                    std::shared_ptr<Material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = PairwiseMul(Color::Random(), Color::Random());
                        sphere_material = std::make_shared<Lambertian>(albedo);
                        auto center2 = center + Vector3(0, random_double(0, 0.5), 0);
                        objects->add(std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                    }
                    else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = Color::Random(0.5, 1);
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
        objects->add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

        auto earth_texture = std::make_shared<ImageTexture>("./assets/textures/earthmap.jpg");
        auto material2 = std::make_shared<Lambertian>(earth_texture);
        objects->add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

        auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
        objects->add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }

    std::shared_ptr<Scene> two_spheres_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();
        auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
        objects->add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
        objects->add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }

    std::shared_ptr<Scene> two_perlin_spheres_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

        auto perlin_texture = std::make_shared<NoiseTexture>(4);
        objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin_texture)));
        objects->add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlin_texture)));

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }

    std::shared_ptr<Scene> earth_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

        auto earth_texture = std::make_shared<ImageTexture>("./assets/textures/earthmap.jpg");
        auto earth_surface = std::make_shared<Lambertian>(earth_texture);
        auto earth = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);
        objects->add(earth);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }

    std::shared_ptr<Scene> simple_light_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

        auto perlin_texture = std::make_shared<NoiseTexture>(4);
        objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin_texture)));
        objects->add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlin_texture)));

        auto diffuse_light = std::make_shared<DiffuseLight>(Color(4, 4, 4));
        objects->add(std::make_shared<Triangle>(Point3(3, 3, 2), Point3(5, 1, 2), Point3(3, 1, 2), diffuse_light));
        objects->add(std::make_shared<Triangle>(Point3(3, 3, 2), Point3(5, 3, 2), Point3(5, 1, 2), diffuse_light));

        objects->add(std::make_shared<Sphere>(Point3(0, 7, 0), 2, std::make_shared<DiffuseLight>(Color(1, 1, 1))));


        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0, 0, 0);

        return scene;
    }

    std::shared_ptr<Scene> bunny_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();
        objects->load_obj("./assets/models/bunny/bunny.obj", std::make_shared<Lambertian>(Color(1.0, 0.1, 0.1)));

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }
    std::shared_ptr<Scene> coffee_maker_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

        std::shared_ptr<ShapeList> yellow = std::make_shared<ShapeList>();
        yellow->load_obj("./assets/models/coffee_maker/Yellow.obj", std::make_shared<Lambertian>(Color(0.96, 0.73, 0.11)));
        std::shared_ptr<ShapeList> black = std::make_shared<ShapeList>();
        black->load_obj("./assets/models/coffee_maker/Black.obj", std::make_shared<Metal>(Color(0.05, 0.05, 0.05), 0.6));
        std::shared_ptr<ShapeList> glass = std::make_shared<ShapeList>();
        glass->load_obj("./assets/models/coffee_maker/Glass.obj", std::make_shared<Dielectric>(1.5));
        std::shared_ptr<ShapeList> metal = std::make_shared<ShapeList>();
        metal->load_obj("./assets/models/coffee_maker/Metal.obj", std::make_shared<Metal>(Color(0.9, 0.9, 0.9), 0.6));
        std::shared_ptr<ShapeList> ground_plane = std::make_shared<ShapeList>();
        ground_plane->load_obj("./assets/models/coffee_maker/Ground.obj", std::make_shared<Lambertian>(Color(0.95, 0.95, 0.95)));

        objects->add(yellow);
        objects->add(black);
        objects->add(glass);
        objects->add(metal);
        objects->add(ground_plane);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        std::cout << "[INFO] Building BVH start." << std::endl;
        auto start_time = std::chrono::system_clock::now();
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        auto end_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "[INFO] Building BVH finished, took " << duration.count() << " ms." << std::endl;

        scene->background = Color(0.70, 0.80, 1.00);

        return scene;
    }

    std::shared_ptr<Scene> cornell_box_scene() {
        std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

        auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
        auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
        auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
        auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

        objects->add(std::make_shared<Triangle>(Point3(555, 0, 0), Point3(555, 0, -555), Point3(555, 555, 0), green));
        objects->add(std::make_shared<Triangle>(Point3(555, 0, -555), Point3(555, 555, -555), Point3(555, 555, 0), green));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(0, 0, -555), Point3(0, 555, 0), red));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, -555), Point3(0, 555, -555), Point3(0, 555, 0), red));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(0, 0, -555), Point3(555, 0, -555), white));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, 0), Point3(555, 0, -555), Point3(555, 0, 0), white));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, -555), Point3(0, 555, -555), Point3(555, 555, -555), white));
        objects->add(std::make_shared<Triangle>(Point3(0, 0, -555), Point3(555, 555, -555), Point3(555, 0, -555), white));
        objects->add(std::make_shared<Triangle>(Point3(0, 555, 0), Point3(0, 555, -555), Point3(555, 555, -555), white));
        objects->add(std::make_shared<Triangle>(Point3(0, 555, 0), Point3(555, 555, -555), Point3(555, 555, 0), white));
        objects->add(std::make_shared<Triangle>(Point3(213, 555, -227), Point3(213, 555, -332), Point3(343, 555, -332), light));
        objects->add(std::make_shared<Triangle>(Point3(213, 555, -227), Point3(343, 555, -332), Point3(343, 555, -227), light));

        auto small_box = std::make_shared<ShapeList>();
        auto trans1 = Transform::Translate(50, 0, -200);

        small_box->add(std::make_shared<Triangle>(
            trans1.Apply(Point3(0, 0, 0)),
            trans1.Apply(Point3(165, 0, 0)),
            trans1.Apply(Point3(165, 165, 0)),
            white));
        small_box->add(std::make_shared<Triangle>(
            trans1.Apply(Point3(0, 0, 0)),
            trans1.Apply(Point3(165, 165, 0)),
            trans1.Apply(Point3(0, 165, 0)),
            white));
        objects->add(small_box);

        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        scene->objects = objects;
        scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
        scene->background = Color(0, 0, 0);

        return scene;
    }
};