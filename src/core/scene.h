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
    };

    std::shared_ptr<Scene> random_ball_scene();
    std::shared_ptr<Scene> two_sphere_scene();
}