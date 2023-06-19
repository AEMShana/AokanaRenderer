#include "camera.h"


namespace Asuka {
    Camera default_camera() {
        double aspect_ratio = 16.0 / 9.0;
        point3 lookfrom(13, 2, -3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        return Camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    }

}