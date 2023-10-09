#pragma once

#include <iostream>
#include "vec.h"

namespace Asuka {
    class Matrix4x4 {
    public:
        Matrix4x4();
        Matrix4x4(double mat[4][4]);
        Matrix4x4(
            double m00, double m01, double m02, double m03,
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33
        ) : mat{ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 } {}

        double operator()(int x, int y) const { return mat[x][y]; }
        double& operator()(int x, int y) { return mat[x][y]; }

        Matrix4x4 operator*(const Matrix4x4& rhs) const;
        Point3 operator*(const Point3& rhs) const;
        Vector3 operator*(const Vector3& rhs) const;

        static Matrix4x4 Identity();
        static Matrix4x4 Zeros();

        Matrix4x4 Inv() const { return Inverse(*this); }
        Matrix4x4 Inverse() const { return Inverse(*this); }
        static Matrix4x4 Inverse(const Matrix4x4& m);

        Matrix4x4 Transpose() const;
        static Matrix4x4 Transpose(const Matrix4x4& m) { return m.Transpose(); }

        double& at(int row, int col) { return mat[row][col]; }
        double at(int row, int col) const { return mat[row][col]; }

        friend std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);
    private:
        double mat[4][4];
    };

    inline std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);
}