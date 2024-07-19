#pragma once

#include <iostream>
#include "vec.h"

namespace Aokana {

    class Matrix4x4 {
    public:
        Matrix4x4(); // 初始化为单位矩阵
        Matrix4x4(const double mat[4][4]);
        Matrix4x4(
            double m00, double m01, double m02, double m03,
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33
        ) : mat{ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 } {}

        double operator()(int x, int y) const { return mat[x][y]; }
        double& operator()(int x, int y) { return mat[x][y]; }

        Matrix4x4 operator+(const Matrix4x4& rhs) const;
        Matrix4x4& operator+=(const Matrix4x4& rhs);
        Matrix4x4 operator*(const Matrix4x4& rhs) const;
        Matrix4x4 operator/(double rhs) const;
        Matrix4x4& operator/=(double rhs);
        Point3 operator*(const Point3& rhs) const;
        Vector3 operator*(const Vector3& rhs) const;
        double& At(int row, int col) { return mat[row][col]; }
        double At(int row, int col) const { return mat[row][col]; }
        bool IsIdentity() const;

        Matrix4x4 Inv() const { return Inverse(*this); }
        Matrix4x4 Inverse() const { return Inverse(*this); }
        Matrix4x4 Transpose() const;

        static Matrix4x4 Identity();
        static Matrix4x4 Zeros();
        static Matrix4x4 Inverse(const Matrix4x4& m);
        static Matrix4x4 Transpose(const Matrix4x4& m) { return m.Transpose(); }

        friend std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);
    private:
        double mat[4][4];
    };

    inline std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);

    class Matrix3x3 {
    public:
        Matrix3x3(); // 初始化为单位矩阵
        Matrix3x3(const double mat[3][3]);
        Matrix3x3(
            double m00, double m01, double m02,
            double m10, double m11, double m12,
            double m20, double m21, double m22
        ) : mat{ m00, m01, m02, m10, m11, m12, m20, m21, m22 } {}

        double operator()(int x, int y) const { return mat[x][y]; }
        double& operator()(int x, int y) { return mat[x][y]; }

        Matrix3x3 operator+(const Matrix3x3& rhs) const;
        Matrix3x3& operator+=(const Matrix3x3& rhs);
        Matrix3x3 operator*(const Matrix3x3& rhs) const;
        Matrix3x3 operator/(double rhs) const;
        Matrix3x3& operator/=(double rhs);
        //Point3 operator*(const Point3& rhs) const;
        //Vector3 operator*(const Vector3& rhs) const;
        double& At(int row, int col) { return mat[row][col]; }
        double At(int row, int col) const { return mat[row][col]; }
        bool IsIdentity() const;

        //Matrix3x3 Inv() const { return Inverse(*this); }
        //Matrix3x3 Inverse() const { return Inverse(*this); }
        Matrix3x3 Transpose() const;

        static Matrix3x3 Identity();
        static Matrix3x3 Zeros();
        //static Matrix3x3 Inverse(const Matrix3x3& m);
        static Matrix3x3 Transpose(const Matrix3x3& m) { return m.Transpose(); }

        friend std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);
    private:
        double mat[3][3];
    };
}