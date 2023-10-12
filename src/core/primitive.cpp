#include "primitive.h"

namespace Asuka {

    bool GeometricPrimitive::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        bool result = shape->IntersectP(ray, isect, t_min, t_max);
        if(!result) return false;
        isect.primitive = this;
        isect.material = material.get();
        return true;
    }

    bool GeometricPrimitive::Intersect(const Ray& ray, double t_min = 0.0001, double t_max = inf) const {
        return shape->Intersect(ray, t_min, t_max);
    }

    Bounds3 GeometricPrimitive::WorldBound() const {
        return shape->WorldBound();
    }

    const Material* GeometricPrimitive::GetMaterial() const {
        return material.get();
    }

    Aggregate::Aggregate(const std::vector<std::shared_ptr<Primitive>>&primitives) : primitives(primitives) {
        world_bound = primitives[0]->WorldBound();
        for(const auto& p : primitives) {
            world_bound = Bounds3::merge(world_bound, p->WorldBound());
        }
    }

    bool Aggregate::Intersect(const Ray& ray, double t_min = 0.0001, double t_max = inf) const {
        for(const auto& primitive : primitives) {
            if(primitive->Intersect(ray, t_min, t_max)) {
                return true;
            }
        }
        return false;s
    }

    bool Aggregate::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        bool result = false;

        for(const auto& primitive : primitives) {
            SurfaceInteraction cur_isect;
            if(primitive->IntersectP(ray, cur_isect, t_min, t_max)){
                result = true;
                t_max = isect.time;                
                isect = cur_isect;
            }
        }
        return result;
    }

    Bounds3 Aggregate::WorldBound() const {
        return world_bound;
    }

    const Material* Aggregate::GetMaterial() const {
        throw std::runtime_error("Can not invoke \"GetMaterial()\" from \"Aggregate\" object.");
    }

    void Aggregate::AddPrimitive(const std::shared_ptr<Primitive>& primitive) {
        primitives.push_back(primitive);
        world_bound = Bounds3::merge(world_bound, primitive->WorldBound());
    }

    void Aggregate::AddPrimitives(const std::vector<std::shared_ptr<Primitive>> &primitive_list) {
        primitives.insert(primitives.end(), primitive_list.begin(), primitive_list.end());
        for(const auto& p : primitive_list) {
            world_bound = Bounds3::merge(world_bound, p->WorldBound());
        }
    }
}