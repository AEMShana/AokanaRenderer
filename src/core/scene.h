#pragma once

#include <vector>
#include <memory>

#include "shape.h"
#include "bvh.h"

namespace Asuka {
    class Scene {
    public:
        std::shared_ptr<Shape> objects;
        std::shared_ptr<BVHNode> bvh;
        color background;
    };

    std::shared_ptr<Scene> random_ball_scene();
    std::shared_ptr<Scene> two_spheres_scene();
    std::shared_ptr<Scene> two_perlin_spheres_scene();
    std::shared_ptr<Scene> earth_scene();
    std::shared_ptr<Scene> simple_light_scene();
    std::shared_ptr<Scene> bunny_scene();
    std::shared_ptr<Scene> coffee_maker_scene();
    std::shared_ptr<Scene> cornell_box_scene();
}