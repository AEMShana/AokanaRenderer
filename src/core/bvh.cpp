#include "bvh.h"
#include "iostream"

namespace Asuka {

    bool BVHNode::bounding_box(double time0, double time1, Bounds3& output_box) const {
        output_box = box;
        return true;
    }

    Bounds3 BVHNode::WorldBound() const {
        return box;
    }

    bool BVHNode::Intersect(const Ray& ray, double t_min, double t_max) const {
        if (!box.hit(ray, t_min, t_max)) return false;
        if (left->Intersect(ray, t_min, t_max)) return true;
        if (right->Intersect(ray, t_min, t_max)) return true;
        return false;
    }

    bool BVHNode::IntersectP(const Ray& ray, SurfaceInteraction& hit_point, double t_min, double t_max) const {
        if (!box.hit(ray, t_min, t_max)) return false;
        bool hit_left = left->IntersectP(ray, hit_point, t_min, t_max);
        bool hit_right = right->IntersectP(ray, hit_point, t_min, hit_left ? hit_point.time : t_max);
        return hit_left || hit_right;
    }

    BVHNode::BVHNode(const std::vector<std::shared_ptr<Shape>>& src_objects,
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

        Bounds3 box_left, box_right;

        if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
            std::cerr << "[ERROR] No bounding box in bvh_node constructor.\n";

        box = Bounds3::merge(box_left, box_right);
    }
}