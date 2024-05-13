#include "matrix.h"

namespace Aokana {

    Matrix4x4::Matrix4x4() {
        for (int i = 0;i < 4;++i)
            for (int j = 0;j < 4;++j)
                mat[i][j] = (i == j) ? 1.0 : 0.0f;
    }

    Matrix4x4::Matrix4x4(double mat[4][4]) {
        memcpy(this->mat, mat, 16 * sizeof(double));
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
        auto res = Matrix4x4::Zeros();
        for (int i = 0;i < 4;++i) {
            for (int j = 0;j < 4;++j) {
                for (int k = 0;k < 4;++k) {
                    res(i, j) += mat[i][k] * rhs(k, j);
                }
            }
        }
        return res;
    }

    Point3 Matrix4x4::operator*(const Point3& rhs) const {
        double v[4] = {rhs.x, rhs.y, rhs.z, 1.0};
        double temp[4]{};

        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                temp[i] += mat[i][j] * v[j];
            }
        }

        double inv = 1.0 / temp[3];
        return Point3(temp[0] * inv, temp[1] * inv, temp[2] * inv);
    }

    Vector3 Matrix4x4::operator*(const Vector3& rhs) const {
        double v[3] = {rhs.x, rhs.y, rhs.z};
        double temp[3]{};

        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                temp[i] += mat[i][j] * v[j];
            }
        }

        return Vector3(temp[0], temp[1], temp[2]);
    }

    Matrix4x4 Matrix4x4::Identity() {
        Matrix4x4 res;
        for (int i = 0;i < 4;++i)
            for (int j = 0;j < 4;++j)
                res.mat[i][j] = (i == j) ? 1.0 : 0.0f;
        return res;
    }

    Matrix4x4 Matrix4x4::Zeros() {
        Matrix4x4 res;
        for (int i = 0;i < 4;++i)
            for (int j = 0;j < 4;++j)
                res.mat[i][j] = 0.0f;
        return res;
    }

    Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m) {
        int indxc[4], indxr[4];
        int iPIv[4] = { 0, 0, 0, 0 };
        double minv[4][4];
        memcpy(minv, m.mat, 4 * 4 * sizeof(double));
        for (int i = 0; i < 4; i++) {
            int irow = 0, icol = 0;
            double big = 0.f;
            // Choose PIvot
            for (int j = 0; j < 4; j++) {
                if (iPIv[j] != 1) {
                    for (int k = 0; k < 4; k++) {
                        if (iPIv[k] == 0) {
                            if (std::abs(minv[j][k]) >= big) {
                                big = static_cast<double>(std::abs(minv[j][k]));
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (iPIv[k] > 1)
                            std::cerr << "Singular matrix in MatrixInvert\n";
                    }
                }
            }
            ++iPIv[icol];
            // Swap rows _irow_ and _icol_ for PIvot
            if (irow != icol) {
                for (int k = 0; k < 4; ++k)
                    std::swap(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.f)
                std::cerr << "Singular matrix in MatrixInvert\n";

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            double PIvinv = 1.0 / minv[icol][icol];
            minv[icol][icol] = 1.0;
            for (int j = 0; j < 4; j++) minv[icol][j] *= PIvinv;

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++) {
                if (j != icol) {
                    double save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++)
                        minv[j][k] -= minv[icol][k] * save;
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = 3; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < 4; k++)
                    std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
            }
        }
        return Matrix4x4(minv);
    }

    Matrix4x4 Matrix4x4::Transpose() const {
        return Matrix4x4(
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
    }

    inline std::ostream& operator<<(std::ostream& out, const Matrix4x4& v) {
        return out << "[[" << v.mat[0][0] << ", " << v.mat[0][1] << ", " << v.mat[0][2] << ", " << v.mat[0][3] << "],\n"
            << " [" << v.mat[1][0] << ", " << v.mat[1][1] << ", " << v.mat[1][2] << ", " << v.mat[1][3] << "]\n"
            << " [" << v.mat[2][0] << ", " << v.mat[2][1] << ", " << v.mat[2][2] << ", " << v.mat[2][3] << "]\n"
            << " [" << v.mat[3][0] << ", " << v.mat[3][1] << ", " << v.mat[3][2] << ", " << v.mat[3][3] << "]]\n";
    }
}