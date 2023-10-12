#pragma once

#include <memory>
#include <vector>

#include "ray.h"
#include "utils.h"
#include "film.h"
#include "sampler.h"

namespace Asuka {

    class Camera {
    public:
        std::shared_ptr<Film> film;

        Camera(
            Point3 cam_position,
            Point3 lookat,
            Vector3 vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
        ) {
            set_parameters(cam_position, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, _time0, _time1);
        }

        void set_parameters(
            Point3 cam_position,
            Point3 lookat,
            Vector3 vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0,
            double _time1
        ) {
            double theta = degree_to_radian(vfov);
            double h = tan(theta / 2.0);
            double viewport_height = 2.0 * h;
            double viewport_width = aspect_ratio * viewport_height;

            w = Normalize(lookat - cam_position);
            u = Normalize(Cross(vup, w));
            v = Cross(w, u);

            origin = cam_position;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = (Vector3(origin) - horizontal / 2.0 - vertical / 2.0 + focus_dist * w);
            lens_radius = aperture / 2.0;
            time0 = _time0;
            time1 = _time1;

            film = std::make_shared<Film>(800, 800.0 / aspect_ratio);
        }


        void save_image(const std::string& path) const {
            film->save_image(path);
        }

        Ray get_ray(const Sample& sample) const {
            Vector3 rd = lens_radius * Vector3::RandomInUnitDisk();
            Vector3 offset = u * rd.x + v * rd.y;
            return Ray(
                origin + offset,
                lower_left_corner + sample.u * horizontal + sample.v * vertical - Vector3(origin) - offset,
                random_double(time0, time1));
        }

        Point3 position() const { return origin; }
    private:
        Point3 origin;
        Vector3 horizontal;
        Vector3 vertical;
        Vector3 lower_left_corner;
        Vector3 u, v, w;
        double lens_radius;
        double time0, time1;
    };

    Camera default_camera();
    Camera coffee_maker_scene_camera();
}

/*
Asuka 渲染器使用左手坐标系，x轴向右，y轴向上，z轴向前
图像使用uv坐标，左下角为坐标原点，u轴向右，v轴向上
*/

// World Coord (x, y, z)    == Blender Coord (x, -z, y)
// Blender Coord (x, y, z) == World Coord (x, z, -y)