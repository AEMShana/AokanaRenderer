#include "camera.h"


namespace Asuka {
    Camera default_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(13, 2, -3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera random_ball_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(13, 2, -3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera two_sphere_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(13, 2, -3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera simple_light_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(26, 3, -6);
        point3 lookat(0, 2, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.02;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera bunny_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(13, 3, 2);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.02;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera coffee_maker_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(1.5, 0.4, -0.5);
        point3 lookat(0, 0.2, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.005;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera cornell_box_scene_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(278, 278, 800);
        point3 lookat(278, 278, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.05;

        return Camera(lookfrom, lookat, vup, 40, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }
}