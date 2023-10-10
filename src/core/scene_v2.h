#pragma once

#include <memory>
#include "bounds.h"
#include "primitive.h"
#include "interaction.h"

namespace Asuka {

    class SceneV2 {
    public:
        SceneV2(std::shared_ptr<Primitive> aggregate) :
            aggregate(aggregate),
            world_bound(aggregate->WorldBound()) {}

        const Bounds3& WorldBound() const { return world_bound; }
        bool Intersect(const Ray& ray, SurfaceInteraction* isect) const;
        bool IntersectP(const Ray& ray) const;

    private:
        std::shared_ptr<Primitive> aggregate;
        Bounds3 world_bound;
    };

}