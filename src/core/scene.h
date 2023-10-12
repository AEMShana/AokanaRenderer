#pragma once

#include <memory>
#include "bounds.h"
#include "primitive.h"
#include "interaction.h"
#include "camera.h"

namespace Asuka {

    class Scene {
    public:
        Scene(const std::shared_ptr<Primitive>& aggregate, const Camera& camera, const Color& background) :
            aggregate(aggregate),
            world_bound(aggregate->WorldBound()),
            camera(camera),
            background(background) {}

        const Bounds3& WorldBound() const { return world_bound; }
        bool Intersect(const Ray& ray, double t_min = 0.0001, double t_max = inf) const;
        bool IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min = 0.0001, double t_max = inf) const;

    public:
        Camera camera;
        Color background;
    private:
        std::shared_ptr<Primitive> aggregate;
        Bounds3 world_bound;
    };

    namespace SampleScene {
        Scene RandomBallScene();
        Scene TwoSpheresScene();
        Scene TwoPerlinSpheresScene();
        Scene EarthScene();
        Scene SimpleLightScene();
        Scene BunnyScene();
    }
}