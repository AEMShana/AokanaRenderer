#include "bvh.h"
#include "iostream"

namespace Asuka {

    bool box_compare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b, int axis) {
        Bounds3 box_a = a->WorldBound();
        Bounds3 box_b = b->WorldBound();

        return box_a.min()[axis] < box_b.min()[axis];
    }

    bool box_x_compare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return box_compare(a, b, 0);
    }

    bool box_y_compare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return box_compare(a, b, 1);
    }

    bool box_z_compare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return box_compare(a, b, 2);
    }

    // bool BVHNode::bounding_box(double time0, double time1, Bounds3& output_box) const {
    //     output_box = box;
    //     return true;
    // }

    Bounds3 BVHNode::WorldBound(double time_0, double time_1) const {
        return box;
    }

    bool BVHNode::Intersect(const Ray& ray, Interval ray_t) const {
        if (!box.Intersect(ray, ray_t)) return false;
        if (left->Intersect(ray, ray_t)) return true;
        if (right->Intersect(ray, ray_t)) return true;
        return false;
    }

    bool BVHNode::IntersectP(const Ray& ray, SurfaceInteraction& isect, Interval ray_t) const {
        if (!box.Intersect(ray, ray_t)) return false;
        bool hit_left = left->IntersectP(ray, isect, ray_t);
        bool hit_right = right->IntersectP(ray, isect, Interval(t_min, hit_left ? isect.time : t_max));
        return hit_left || hit_right;
    }

    const Material* BVHNode::GetMaterial() const {
        throw std::runtime_error("Can not invoke \"GetMaterial()\" from \"BVHNode\" object.");
    }

    BVHNode::BVHNode(const std::vector<std::shared_ptr<Primitive>>& src_objects,
        size_t start, size_t end, double time0, double time1) {

        auto objects = src_objects;
        int axis = random_int(0, 2);
        auto comparator =
            (axis == 0) ? box_x_compare :
            (axis == 1) ? box_y_compare :
            box_z_compare;

        size_t object_num = end - start;

        if (object_num == 1)
            left = right = objects[start];
        else if (object_num == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            auto mid = start + object_num / 2;
            left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
            right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);
        }

        Bounds3 box_left = left->WorldBound(time0, time1);
        Bounds3 box_right = right->WorldBound(time0, time1);

        // if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        //     std::cerr << "[ERROR] No bounding box in bvh_node constructor.\n";

        box = Bounds3::merge(box_left, box_right);
    }
}