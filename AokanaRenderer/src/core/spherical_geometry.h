#pragma once

#include "vec.h"
#include "utils.h"

namespace Aokana::SphericalGeometry {

    // 球面几何
    // https://pbr-book.org/4ed/Geometry_and_Transformations/Spherical_Geometry#DirectionCone
    // 对于球面坐标系，我们规定 theta 角是单位球面上的向量和 z 轴的夹角, phi 角是和 x 轴的夹角
    // 球面坐标系在靠近极点的区域 (theta 接近 0 或 pi) 精度较高，在靠近赤道的区域精度较低

    // Girard's theorem: 
    // 单位球面上的三角形面积 A = alpha + beta + gamma - pi，其中 alpha, beta, gamma 是球面三角形的内角
    // 单位球面上的四边形面积 A = alpha + beta + gamma + delta - 2 * pi，其中 alpha, beta, gamma, delta 是球面四边形的内角

    // 计算单位球面上的三角形面积，其中 a, b, c 是从球心到球面的单位向量
    inline double SphericalTriangleArea(const Vector3& a, const Vector3& b, const Vector3& c) {
        return std::abs(2 * std::atan2(Dot(a, Cross(b, c)),
            1 + Dot(a, b) + Dot(a, c) + Dot(b, c)));
    }

    // 计算单位球面上的四边形面积，其中 a, b, c, d 是从球心到球面的单位向量
    inline double SphericalQuadArea(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d) {
        Vector3 axb = Cross(a, b);
        Vector3 bxc = Cross(b, c);
        Vector3 cxd = Cross(c, d);
        Vector3 dxa = Cross(d, a);

        if (axb.LengthSquare() == 0 || bxc.LengthSquare() == 0 || cxd.LengthSquare() == 0 || dxa.LengthSquare() == 0) {
            return 0;
        }

        axb = Normalize(axb);
        bxc = Normalize(bxc);
        cxd = Normalize(cxd);
        dxa = Normalize(dxa);

        double alpha = AngleBetween(dxa, -axb);
        double beta = AngleBetween(axb, -bxc);
        double gamma = AngleBetween(bxc, -cxd);
        double delta = AngleBetween(cxd, -dxa);

        return std::abs(alpha + beta + gamma + delta - 2 * PI);
    }

    // 给定球面坐标下的 theta 角的 sin 值和 cos 值，以及 phi 角，返回单位球面上的向量
    inline Vector3 SphericalDirection(double sin_theta, double cos_theta, double phi) {
        // x = r * sin(theta) * cos(phi)
        // y = r * sin(theta) * sin(phi)
        // z = r * cos(theta)

        return Vector3(
            Clamp(sin_theta, -1.0, 1.0) * std::cos(phi),
            Clamp(sin_theta, -1.0, 1.0) * std::sin(phi),
            Clamp(cos_theta, -1.0, 1.0)
        );
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角
    inline double SphericalTheta(const Vector3& v) {
        // theta = arccos(z)
        return SafeACos(v.z);
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 cos 值
    inline double SphericalCosTheta(const Vector3& v) {
        // cos(theta) = (0, 0, 1) dot v
        return v.z;
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 cos 值的绝对值
    inline double SphericalAbsCosTheta(const Vector3& v) {
        // cos(theta) = (0, 0, 1) dot v
        return std::abs(v.z);
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 cos 值的平方
    inline double SphericalCos2Theta(const Vector3& v) {
        // cos(theta) = (0, 0, 1) dot v
        return Square(v.z);
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 sin 值的平方
    inline double SphericalSin2Theta(const Vector3& v) {
        // sin^2(theta) = 1 - cos^2(theta) 
        return std::max(0.0, 1 - SphericalCos2Theta(v));
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 sin 值
    inline double SphericalSinTheta(const Vector3& v) {
        return std::sqrt(SphericalSin2Theta(v));
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 tan 值
    inline double SphericalTanTheta(const Vector3& v) {
        // tan(theta) = sin(theta) / cos(theta)
        return SphericalSinTheta(v) / SphericalCosTheta(v);
    }

    // 给定单位球面上的向量，返回球面坐标下的 theta 角的 tan 值的平方
    inline double SphericalTan2Theta(const Vector3& v) {
        // tan^2(theta) = sin^2(theta) / cos^2(theta)
        return SphericalSin2Theta(v) / SphericalCos2Theta(v);
    }

    // 给定单位球面上的向量，返回球面坐标下的 phi 角，限制在 0 ~ 2PI 范围内
    inline double SphericalPhi(const Vector3& v) {
        // phi = arctan(y / x)
        double p = std::atan2(v.y, v.x);
        return (p < 0) ? (p + 2 * PI) : p;
    }

    // 给定单位球面上的向量，返回球面坐标下的 phi 角的 cos 值
    inline double SphericalCosPhi(const Vector3& v) {
        // cos(phi) = x / r = x / sin(theta)
        double sin_theta = SphericalSinTheta(v);
        return (sin_theta == 0) ? 1 : Clamp(v.x / sin_theta, -1.0, 1.0);
    }

    // 给定单位球面上的向量，返回球面坐标下的 phi 角的 sin 值
    inline double SphericalSinPhi(const Vector3& v) {
        // sin(phi) = y / r = y / sin(theta)
        double sin_theta = SphericalSinTheta(v);
        return (sin_theta == 0) ? 0 : Clamp(v.y / sin_theta, -1.0, 1.0);
    }

    // 给定单位球面上两个向量 v1 和 v2，返回它们 phi 角的 delta 的 cos 值
    inline double SphericalCosDeltaPhi(const Vector3& v1, const Vector3& v2) {
        double v1xy = Square(v1.x) + Square(v1.y);
        double v2xy = Square(v2.x) + Square(v2.y);
        if (v1xy == 0 || v2xy == 0) return 1;
        return Clamp((v1.x * v2.x + v1.y * v2.y) / std::sqrt(v1xy * v2xy), -1.0, 1.0);
    }


    // 球面八面体映射, 存储单位球面上的方向向量仅需 4 个 byte，而球面坐标系需要 16 个 byte (theta, phi)
    class OctahedralVector {
    public:
        // 要求传入的 v 是单位球面上的单位向量
        OctahedralVector(Vector3 v) {
            v /= std::abs(v.x) + std::abs(v.y) + std::abs(v.z); // v 除以自身的 L1 范数，将 v 变换到内切于单位球面的八面体内
            if (v.z >= 0) { // 上半球面
                x = Encode(v.x);
                y = Encode(v.y);
            }
            else { // 下半球面
                x = Encode((1 - std::abs(v.y)) * Sign(v.x));
                y = Encode((1 - std::abs(v.x)) * Sign(v.y));
            }
        }

        // 将八面体映射转换回单位向量
        explicit operator Vector3() const {
            Vector3 v{};
            v.x = -1 + 2 * (x / 65535.0);
            v.y = -1 + 2 * (y / 65535.0);
            v.z = 1 - (std::abs(v.x) + std::abs(v.y));
            if (v.z < 0) { // 下半球面
                double xo = v.x;
                v.x = (1 - std::abs(v.y)) * Sign(xo);
                v.y = (1 - std::abs(xo)) * Sign(v.y);
            }
            return Normalize(v);
        }

    private:
        static double Sign(double v) { return std::copysign(1.0, v); }

        static uint16_t Encode(double f) {
            // 传入的 f in [-1, 1]
            // Encode 将 f 映射到 [0, 1] 上，然后使用 16bit 量化成 [0, 65535] 的整数
            return std::round(Clamp((f + 1) / 2, 0.0, 1.0) * 65535);
        }

        uint16_t x, y;
    };
}