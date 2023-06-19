#pragma once

#include <vector>
#include <memory>

#include "shape.h"

namespace Asuka {
    class Scene {
    public:
        std::shared_ptr<Shape> objects;
    };

    std::shared_ptr<Scene> random_ball_scene();
}