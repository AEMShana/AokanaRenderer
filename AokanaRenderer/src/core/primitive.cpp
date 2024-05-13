#include "primitive.h"
#include "../external/obj_loader/OBJ_Loader.h"


namespace Aokana {

    bool GeometricPrimitive::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        bool result = shape->IntersectP(ray, isect, t_min, t_max);
        if (!result) return false;
        isect.primitive = this;
        isect.material = material.get();
        return true;
    }

    bool GeometricPrimitive::Intersect(const Ray& ray, double t_min, double t_max) const {
        return shape->Intersect(ray, t_min, t_max);
    }

    Bounds3 GeometricPrimitive::WorldBound(double time0, double time1) const {
        return shape->WorldBound(time0, time1);
    }

    const Material* GeometricPrimitive::GetMaterial() const {
        return material.get();
    }

    Aggregate::Aggregate(const std::vector<std::shared_ptr<Primitive>>& primitives) : primitives(primitives) {
        world_bound = primitives[0]->WorldBound();
        for (const auto& p : primitives) {
            world_bound = Bounds3::merge(world_bound, p->WorldBound());
        }
    }

    bool Aggregate::Intersect(const Ray& ray, double t_min, double t_max) const {
        for (const auto& primitive : primitives) {
            if (primitive->Intersect(ray, t_min, t_max)) {
                return true;
            }
        }
        return false;
    }

    bool Aggregate::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        bool result = false;

        for (const auto& primitive : primitives) {
            SurfaceInteraction cur_isect;
            if (primitive->IntersectP(ray, cur_isect, t_min, t_max)) {
                result = true;
                t_max = cur_isect.time;
                isect = cur_isect;
            }
        }
        return result;
    }

    Bounds3 Aggregate::WorldBound(double time0, double time1) const {
        if (primitives.empty()) {
            throw std::runtime_error("Primitives is empty!");
        }
        return world_bound;
    }

    const Material* Aggregate::GetMaterial() const {
        throw std::runtime_error("Can not invoke \"GetMaterial()\" from \"Aggregate\" object.");
    }

    void Aggregate::AddPrimitive(const std::shared_ptr<Primitive>& primitive) {
        primitives.push_back(primitive);
        world_bound = Bounds3::merge(world_bound, primitive->WorldBound());
    }

    void Aggregate::AddPrimitives(const std::vector<std::shared_ptr<Primitive>>& primitive_list) {
        primitives.insert(primitives.end(), primitive_list.begin(), primitive_list.end());
        for (const auto& p : primitive_list) {
            world_bound = Bounds3::merge(world_bound, p->WorldBound());
        }
    }

    void Aggregate::AddObj(const std::string& filepath, const std::shared_ptr<Material>& material, bool filp_x_axis) {
        objl::Loader objLoader;
        objLoader.LoadFile(filepath);

        double flip = 1.0;
        if (filp_x_axis) flip = -1.0;

        for (auto& mesh : objLoader.LoadedMeshes) {
            for (int i = 2;i < mesh.Indices.size();i += 3) {
                Point3 a(
                    flip * mesh.Vertices[mesh.Indices[i - 2]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Z);

                Point3 b(
                    flip * mesh.Vertices[mesh.Indices[i - 1]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Z);

                Point3 c(
                    flip * mesh.Vertices[mesh.Indices[i]].Position.X,
                    mesh.Vertices[mesh.Indices[i]].Position.Y,
                    mesh.Vertices[mesh.Indices[i]].Position.Z);

                primitives.push_back(
                    std::make_shared<GeometricPrimitive>(
                        std::make_shared<Triangle>(a, b, c),
                        material
                    )
                );
            }
        }
    }

}