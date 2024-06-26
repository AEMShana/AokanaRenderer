#include "bvh.h"
#include "iostream"

namespace Aokana {

    bool BoxCompare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b, int axis) {
        Bounds3 box_a = a->WorldBound();
        Bounds3 box_b = b->WorldBound();

        return box_a.Min()[axis] < box_b.Min()[axis];
    }

    bool BoxXCompare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return BoxCompare(a, b, 0);
    }

    bool BoxYCompare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return BoxCompare(a, b, 1);
    }

    bool BoxZCompare(const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {
        return BoxCompare(a, b, 2);
    }

    // bool BVHNode::bounding_box(double time0, double time1, Bounds3& output_box) const {
    //     output_box = box;
    //     return true;
    // }

    Bounds3 BVHNode::WorldBound(double , double time_1) const {
        return box;
    }

    bool BVHNode::Intersect(const Ray& ray, double t_min, double t_max) const {
        if (!box.Hit(ray, t_min, t_max)) return false;
        if (left->Intersect(ray, t_min, t_max)) return true;
        if (right->Intersect(ray, t_min, t_max)) return true;
        return false;
    }

    bool BVHNode::IntersectP(const Ray& ray, SurfaceInteraction& isect, double t_min, double t_max) const {
        if (!box.Hit(ray, t_min, t_max)) return false;
        bool hit_left = left->IntersectP(ray, isect, t_min, t_max);
        bool hit_right = right->IntersectP(ray, isect, t_min, hit_left ? isect.time : t_max);
        return hit_left || hit_right;
    }

    const Material* BVHNode::GetMaterial() const {
        throw std::runtime_error("Can not invoke \"GetMaterial()\" from \"BVHNode\" object.");
    }

    BVHNode::BVHNode(const std::vector<std::shared_ptr<Primitive>>& src_objects,
        size_t start, size_t end, double time0, double time1) {

        auto objects = src_objects;
        int axis = RandomIntInRange(0, 2);
        auto comparator =
            (axis == 0) ? BoxXCompare :
            (axis == 1) ? BoxYCompare :
            BoxZCompare;

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

        box = Bounds3::Merge(box_left, box_right);
    }
}