#include "camera.h"


namespace Aokana {
    Camera CreateDefaultCamera() {
        double aspect_ratio = 16.0 / 9.0;
        Point3 lookfrom(13, 2, -3);
        Point3 lookat(0, 0, 0);
        Vector3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }

    Camera CreateCoffeeMakerSceneCamera() {
        double aspect_ratio = 16.0 / 9.0;
        Point3 lookfrom(1.5, 0.4, -0.5);
        Point3 lookat(0, 0.2, 0);
        Vector3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.005;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    }
}