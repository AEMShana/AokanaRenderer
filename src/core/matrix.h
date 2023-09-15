#pragma once

#include "../../external/glm/glm/glm.hpp"
#include <iostream>

namespace Asuka {
    class Matrix4x4 {
    private:
        explicit Matrix4x4(const glm::mat4x4& m) : mat(m) {}
    public:
        Matrix4x4() {
            for (int i = 0;i < 4;++i)
                for (int j = 0;j < 4;++j)
                    mat[i][j] = (i == j) ? 1.0 : 0.0f;
        }

        Matrix4x4(
            double m00, double m01, double m02, double m03,
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33
        ) : mat(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}

        Matrix4x4 operator*(const Matrix4x4& rhs) const {
            return Matrix4x4(this->mat * rhs.mat);
        }

        static Matrix4x4 Identity() {
            Matrix4x4 res;
            for (int i = 0;i < 4;++i)
                for (int j = 0;j < 4;++j)
                    res.mat[i][j] = (i == j) ? 1.0 : 0.0f;
            return res;
        }

        static Matrix4x4 Zeros() {
            Matrix4x4 res;
            for (int i = 0;i < 4;++i)
                for (int j = 0;j < 4;++j)
                    res.mat[i][j] = 0.0f;
            return res;
        }

        Matrix4x4 inv() const {
            return Matrix4x4(glm::inverse(mat));
        }

        static Matrix4x4 Inv(const Matrix4x4& m) {
            return m.inv();
        }

        Matrix4x4 transpose() const {
            return Matrix4x4(glm::transpose(mat));
        }

        static Matrix4x4 Transpose(const Matrix4x4& m) {
            return m.transpose();
        }

        float& at(int row, int col) { return mat[row][col]; }
        float at(int row, int col) const { return mat[row][col]; }

        friend std::ostream& operator<<(std::ostream& out, const Matrix4x4& v);
    private:
        glm::mat4x4 mat;
    };

    inline std::ostream& operator<<(std::ostream& out, const Matrix4x4& v) {
        return out << "[[" << v.mat[0][0] << ", " << v.mat[0][1] << ", " << v.mat[0][2] << ", " << v.mat[0][3] << "],\n"
            << " [" << v.mat[1][0] << ", " << v.mat[1][1] << ", " << v.mat[1][2] << ", " << v.mat[1][3] << "]\n"
            << " [" << v.mat[2][0] << ", " << v.mat[2][1] << ", " << v.mat[2][2] << ", " << v.mat[2][3] << "]\n"
            << " [" << v.mat[3][0] << ", " << v.mat[3][1] << ", " << v.mat[3][2] << ", " << v.mat[3][3] << "]]\n";
    }
}