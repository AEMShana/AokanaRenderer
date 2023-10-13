#include "scene.h"
#include "moving_sphere.h"
#include "bvh.h"

namespace Asuka {

    bool Scene::Intersect(const Ray& ray, double t_min, double t_max) const {
        return aggregate->Intersect(ray, t_min, t_max);
    }

    bool Scene::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        return aggregate->IntersectP(ray, isect, t_min, t_max);
    }

    namespace SampleScene {

        Scene RandomBallScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, -1000, 0), 1000),
                    std::make_shared<Lambertian>(checker)
                )
            );

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
                            aggregate->AddPrimitive(
                                std::make_shared<GeometricPrimitive>(
                                    std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2),
                                    sphere_material
                                )
                            );
                        }
                        else if (choose_mat < 0.95) {
                            // metal
                            auto albedo = Color::Random(0.5, 1);
                            auto fuzz = random_double(0, 0.5);
                            sphere_material = std::make_shared<Metal>(albedo, fuzz);
                            aggregate->AddPrimitive(
                                std::make_shared<GeometricPrimitive>(
                                    std::make_shared<Sphere>(center, 0.2),
                                    sphere_material
                                )
                            );
                        }
                        else {
                            // glass
                            sphere_material = std::make_shared<Dielectric>(1.5);
                            aggregate->AddPrimitive(
                                std::make_shared<GeometricPrimitive>(
                                    std::make_shared<Sphere>(center, 0.2),
                                    sphere_material
                                )
                            );
                        }
                    }
                }
            }

            auto material1 = std::make_shared<Dielectric>(1.5);
            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, 1, 0), 1.0),
                    material1
                )
            );

            auto earth_texture = std::make_shared<ImageTexture>("./assets/textures/earthmap.jpg");
            auto material2 = std::make_shared<Lambertian>(earth_texture);
            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0),
                    material2
                )
            );

            auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(4, 1, 0), 1.0),
                    material3
                )
            );

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(13, 2, -3);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.1;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.70, 0.80, 1.00);
            auto bvh_root = std::make_shared<BVHNode>(aggregate, 0.0, 1.0);

            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(bvh_root, camera, background);
        }

        Scene TwoSpheresScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
            auto checker_material = std::make_shared<Lambertian>(checker);

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, -10, 0), 10),
                    checker_material
                )
            );

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, 10, 0), 10),
                    checker_material
                )
            );

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(13, 2, -3);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.1;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.70, 0.80, 1.00);

            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(aggregate, camera, background);
        }

        Scene TwoPerlinSpheresScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto perlin_texture = std::make_shared<NoiseTexture>(4);
            auto perlin_texture_material = std::make_shared<Lambertian>(perlin_texture);

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, -1000, 0), 1000),
                    perlin_texture_material
                )
            );
            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, 2, 0), 2),
                    perlin_texture_material
                )
            );

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(13, 2, -3);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.1;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.70, 0.80, 1.00);
            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(aggregate, camera, background);
        }

        Scene EarthScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto earth_texture = std::make_shared<ImageTexture>("./assets/textures/earthmap.jpg");
            auto earth_surface_material = std::make_shared<Lambertian>(earth_texture);
            auto earth = std::make_shared<Sphere>(Point3(0, 0, 0), 2);

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    earth,
                    earth_surface_material
                )
            );

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(13, 2, -3);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.1;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.70, 0.80, 1.00);
            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(aggregate, camera, background);
        }

        Scene SimpleLightScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto perlin_texture = std::make_shared<NoiseTexture>(4);
            auto perlin_texture_material = std::make_shared<Lambertian>(perlin_texture);

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, -1000, 0), 1000),
                    perlin_texture_material
                )
            );

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, 2, 0), 2),
                    perlin_texture_material
                )
            );

            auto diffuse_light_material = std::make_shared<DiffuseLight>(Color(4, 4, 4));

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Triangle>(Point3(3, 3, 2), Point3(5, 1, 2), Point3(3, 1, 2)),
                    diffuse_light_material
                )
            );

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Triangle>(Point3(3, 3, 2), Point3(5, 3, 2), Point3(5, 1, 2)),
                    diffuse_light_material
                )
            );

            aggregate->AddPrimitive(
                std::make_shared<GeometricPrimitive>(
                    std::make_shared<Sphere>(Point3(0, 7, 0), 2),
                    std::make_shared<DiffuseLight>(Color(1, 1, 1))
                )
            );

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(26, 3, -6);
            Point3 lookat(0, 2, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.02;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0, 0, 0);
            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(aggregate, camera, background);
        }


        Scene BunnyScene() {
            auto aggregate = std::make_shared<Aggregate>();

            auto material = std::make_shared<Lambertian>(Color(1.0, 0.1, 0.1));
            aggregate->AddObj("./assets/models/bunny/bunny2.obj", material);

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(1, 1.2239, 3.3532);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.02;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.70, 0.80, 1.00);

            // scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);

            return Scene(aggregate, camera, background);
        }

        Scene CornellBox() {
            auto aggregate = std::make_shared<Aggregate>();

            auto red_material = std::make_shared<Lambertian>(Color(1.0, 0.0, 0.0));
            auto green_material = std::make_shared<Lambertian>(Color(0.0, 1.0, 0.0));
            auto white_material = std::make_shared<Lambertian>(Color(1.0, 1.0, 1.0));
            auto light_material = std::make_shared<DiffuseLight>(Color(15.0, 15.0, 15.0));

            aggregate->AddObj("./assets/models/cornell_box/RedWall.obj", red_material);
            aggregate->AddObj("./assets/models/cornell_box/GreenWall.obj", green_material);
            aggregate->AddObj("./assets/models/cornell_box/WhiteWall.obj", white_material);
            aggregate->AddObj("./assets/models/cornell_box/Floor.obj", white_material);
            aggregate->AddObj("./assets/models/cornell_box/Ceiling.obj", white_material);
            aggregate->AddObj("./assets/models/cornell_box/LargeBox.obj", white_material);
            aggregate->AddObj("./assets/models/cornell_box/SmallBox.obj", white_material);
            aggregate->AddObj("./assets/models/cornell_box/Light.obj", light_material);

            double aspect_ratio = 16.0 / 9.0;
            Point3 lookfrom(0, 0, 3.5);
            Point3 lookat(0, 0, 0);
            Vector3 vup(0, 1, 0);
            double dist_to_focus = 10.0;
            double aperture = 0.02;
            Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

            Color background(0.0, 0.0, 0.0);

            auto bvh_root = std::make_shared<BVHNode>(aggregate, 0, 1.0);

            return Scene(bvh_root, camera, background);
        }
    }

}


// std::shared_ptr<Scene> coffee_maker_scene() {
//     std::shared_ptr<ShapeList> objects = std::make_shared<ShapeList>();

//     std::shared_ptr<ShapeList> yellow = std::make_shared<ShapeList>();
//     yellow->load_obj("./assets/models/coffee_maker/Yellow.obj", std::make_shared<Lambertian>(Color(0.96, 0.73, 0.11)));
//     std::shared_ptr<ShapeList> black = std::make_shared<ShapeList>();
//     black->load_obj("./assets/models/coffee_maker/Black.obj", std::make_shared<Metal>(Color(0.05, 0.05, 0.05), 0.6));
//     std::shared_ptr<ShapeList> glass = std::make_shared<ShapeList>();
//     glass->load_obj("./assets/models/coffee_maker/Glass.obj", std::make_shared<Dielectric>(1.5));
//     std::shared_ptr<ShapeList> metal = std::make_shared<ShapeList>();
//     metal->load_obj("./assets/models/coffee_maker/Metal.obj", std::make_shared<Metal>(Color(0.9, 0.9, 0.9), 0.6));
//     std::shared_ptr<ShapeList> ground_plane = std::make_shared<ShapeList>();
//     ground_plane->load_obj("./assets/models/coffee_maker/Ground.obj", std::make_shared<Lambertian>(Color(0.95, 0.95, 0.95)));

//     objects->add(yellow);
//     objects->add(black);
//     objects->add(glass);
//     objects->add(metal);
//     objects->add(ground_plane);

//     std::shared_ptr<Scene> scene = std::make_shared<Scene>();
//     scene->objects = objects;
//     std::cout << "[INFO] Building BVH start." << std::endl;
//     auto start_time = std::chrono::system_clock::now();
//     scene->bvh = std::make_shared<BVHNode>(objects, 0.0, 1.0);
//     auto end_time = std::chrono::system_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
//     std::cout << "[INFO] Building BVH finished, took " << duration.count() << " ms." << std::endl;

//     scene->background = Color(0.70, 0.80, 1.00);

//     return scene;
// }