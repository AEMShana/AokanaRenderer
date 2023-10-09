#pragma once

#include "matrix.h"
#include "vec.h"
#include "bounds.h"
#include "ray.h"

namespace Asuka {
    class Transform {
    public:
        Transform() = default;
        Transform(const Matrix4x4& _m) : m(_m), mInv(_m.Inv()) {}
        Transform(const Matrix4x4& _m, const Matrix4x4& _mInv) : m(_m), mInv(_mInv) {}

        Transform inv() const { return Transform(mInv, m); }

        static Transform Inv(const Transform& t) { return t.inv(); }

        Transform operator*(const Transform& rhs) const {
            return Transform(this->m * rhs.m, rhs.mInv * this->mInv);
        }

        bool SwapsHandedness() const {
            double det = 
                m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
                m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
            return det < 0;
        }

        Point3 Apply(const Point3& p) {
            return m * p;
        }

        Vector3 Apply(const Vector3& v) {
            return m * v;
        }

        Bounds3 Apply(const Bounds3& b) {
            return Bounds3(m * b.min(), m * b.max());
        }

        Ray Apply(const Ray& ray) {
            return Ray(Apply(ray.origin()), Apply(ray.direction()));
        }

        static Transform Translate(double x, double y, double z) {
            Transform trans;

            trans.m = Matrix4x4(
                1.0, 0.0, 0.0, x,
                0.0, 1.0, 0.0, y,
                0.0, 0.0, 1.0, z,
                0.0, 0.0, 0.0, 1.0
            );

            trans.mInv = Matrix4x4(
                1.0, 0.0, 0.0, -x,
                0.0, 1.0, 0.0, -y,
                0.0, 0.0, 1.0, -z,
                0.0, 0.0, 0.0, 1.0
            );

            return trans;
        }

        static Transform Translate(const Vector3& v) {
            return Translate(v.x, v.y, v.z);
        }

        static Transform Scale(double x, double y, double z) {
            Transform trans;

            trans.m = Matrix4x4(
                x, 0.0, 0.0, 0.0,
                0.0, y, 0.0, 0.0,
                0.0, 0.0, z, 0.0,
                0.0, 0.0, 0.0, 1.0
            );

            trans.mInv = Matrix4x4(
                1.0 / x, 0.0, 0.0, 0.0,
                0.0, 1.0 / y, 0.0, 0.0,
                0.0, 0.0, 1.0 / z, 0.0,
                0.0, 0.0, 0.0, 1.0
            );

            return trans;
        }

        static Transform Scale(const Vector3& v) {
            return Scale(v.x, v.y, v.z);
        }

        static Transform RotateX(double degree) {
            Transform trans;
            double theta = degree_to_radian(degree);
            double c = cos(theta);
            double s = sin(theta);
            trans.m = Matrix4x4(
                1.0, 0.0, 0.0, 0.0,
                0.0, c, -s, 0.0,
                0.0, s, c, 0.0,
                0.0, 0.0, 0.0, 1.0
            );
            trans.mInv = trans.m.Transpose();

            return trans;
        }

        static Transform RotateY(double degree) {
            Transform trans;
            double theta = degree_to_radian(degree);
            double c = cos(theta);
            double s = sin(theta);
            trans.m = Matrix4x4(
                c, 0.0, s, 0.0,
                0.0, 1.0, 0.0, 0.0,
                -s, 0.0, c, 0.0,
                0.0, 0.0, 0.0, 1.0
            );
            trans.mInv = trans.m.Transpose();

            return trans;
        }

        static Transform RotateZ(double degree) {
            Transform trans;
            double theta = degree_to_radian(degree);
            double c = cos(theta);
            double s = sin(theta);
            trans.m = Matrix4x4(
                c, -s, 0.0, 0.0,
                s, c, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0
            );
            trans.mInv = trans.m.Transpose();

            return trans;
        }

        static Transform Rotate(double x_degree, double y_degree, double z_degree) {
            return Transform::RotateX(x_degree) * Transform::RotateY(y_degree) * Transform::RotateZ(z_degree);
        }

        static Transform Rotate(const Vector3& v) {
            return Rotate(v.x, v.y, v.z);
        }

    private:
        Matrix4x4 m;
        Matrix4x4 mInv;

    };
}