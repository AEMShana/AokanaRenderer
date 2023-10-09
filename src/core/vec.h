#pragma once

#include <cmath>
#include <cassert>
#include <iostream>
#include "utils.h"

namespace Asuka {
    // Asuka Renderer 使用左手坐标系

    class Vector2;
    class Point2;
    class Vector3;
    class Point3;
    class Normal3;

    using Color = Vector3;


    inline double Square(double value) {
        return value * value;
    }

    class Vector2 {
    public:
        Vector2() { x = y = 0; }

        Vector2(double x, double y) : x(x), y(y) {
            assert(!HasNaNs());
        }

        explicit Vector2(const Point2& p);

        bool HasNaNs() const {
            return std::isnan(x) || std::isnan(y);
        }

        double operator[](int index) const {
            assert(0 <= index && index <= 1);
            if (index == 0) return x;
            return y;
        }

        double& operator[](int index) {
            assert(0 <= index && index <= 1);
            if (index == 0) return x;
            return y;
        }

        Vector2 operator-() const {
            return Vector2(-x, -y);
        }

        Vector2 operator+(const Vector2& rhs) const {
            return Vector2(x + rhs.x, y + rhs.y);
        }

        Vector2& operator+=(const Vector2& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vector2 operator-(const Vector2& rhs) const {
            return Vector2(x - rhs.x, y - rhs.y);
        }

        Vector2& operator-=(const Vector2& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vector2 operator*(double rhs) const {
            return Vector2(x * rhs, y * rhs);
        }

        Vector2& operator*=(double rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        Vector2 operator/(double rhs) const {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            return Vector2(x * inv, y * inv);
        }

        Vector2& operator/=(double rhs) {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            x *= inv;
            y *= inv;
            return *this;
        }

        double LengthSquare() const {
            return Square(x) + Square(y);
        }

        double Length() const {
            return sqrt(LengthSquare());
        }

        double x;
        double y;
    };

    class Point2 {
    public:
        Point2() { x = y = 0; }

        Point2(double x, double y) : x(x), y(y) {
            assert(!HasNaNs());
        }

        explicit Point2(const Point3& p);

        explicit operator Vector2() const {
            return Vector2(x, y);
        }

        bool HasNaNs() const {
            return std::isnan(x) || std::isnan(y);
        }

        double operator[](int index) const {
            assert(0 <= index && index <= 1);
            if (index == 0) return x;
            return y;
        }

        double& operator[](int index) {
            assert(0 <= index && index <= 1);
            if (index == 0) return x;
            return y;
        }

        Point2 operator+(const Point2& p) const {
            return Point2(x + p.x, y + p.y);
        }

        Point2 operator+(const Vector2& v) const {
            return Point2(x + v.x, y + v.y);
        }

        Point2& operator+=(const Vector2& v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Point2 operator-(const Vector2& v) const {
            return Point2(x - v.x, y - v.y);
        }

        Point2& operator-=(const Vector2& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vector2 operator-(const Point2& p) const {
            return Vector2(x - p.x, y - p.y);
        }

        Point2 operator*(double rhs) const {
            return Point2(rhs * x, rhs * y);
        }

        double x;
        double y;
    };

    class Vector3 {
    public:
        Vector3() :x(0), y(0), z(0) {}

        Vector3(double x, double y, double z) : x(x), y(y), z(z) {
            assert(!HasNaNs());
        }

        explicit Vector3(const Point3& p);

        explicit Vector3(const Normal3& n);

        bool HasNaNs() const {
            return std::isnan(x) || std::isnan(y) || std::isnan(z);
        }

        double operator[](int index) const {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        double& operator[](int index) {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }

        Vector3 operator+(const Vector3& rhs) const {
            return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vector3& operator+=(const Vector3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3 operator-(const Vector3& rhs) const {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Vector3& operator-=(const Vector3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vector3 operator*(double rhs) const {
            return Vector3(x * rhs, y * rhs, z * rhs);
        }

        Vector3& operator*=(double rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Vector3 operator/(double rhs) const {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            return Vector3(x * inv, y * inv, z * inv);
        }

        Vector3& operator/=(double rhs) {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }

        double LengthSquare() const {
            return Square(x) + Square(y) + Square(z);
        }

        double Length() const {
            return std::sqrt(LengthSquare());
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(x) < s) && (fabs(x) < s) && (fabs(x) < s);
        }

        static Vector3 Random() {
            return Vector3(random_double(), random_double(), random_double());
        }

        static Vector3 Random(double min, double max) {
            return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        static Vector3 RandomInUnitSphere() {
            while (true) {
                auto p = Vector3::Random(-1, 1);
                if (p.LengthSquare() < 1) return p;
            }
        }

        static Vector3 RandomUnitVector();

        static Vector3 RandomInHemisphere(const Normal3& normal);

        static Vector3 RandomInUnitDisk() {
            while (true) {
                auto p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
                if (p.LengthSquare() >= 1) continue;
                return p;
            }
        }

        double x;
        double y;
        double z;
    };

    class Normal3 {
    public:
        Normal3() :x(0), y(0), z(0) {}

        Normal3(double x, double y, double z) : x(x), y(y), z(z) {
            assert(!HasNaNs());
        }

        explicit Normal3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {
            assert(!HasNaNs());
        }

        bool HasNaNs() const {
            return std::isnan(x) || std::isnan(y) || std::isnan(z);
        }

        double operator[](int index) const {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        double& operator[](int index) {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        Normal3 operator-() const { return Normal3(-x, -y, -z); }

        Normal3 operator+(const Normal3& rhs) const {
            return Normal3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Normal3& operator+=(const Normal3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Normal3 operator-(const Normal3& rhs) const {
            return Normal3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Normal3& operator-=(const Normal3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Normal3 operator*(double rhs) const {
            return Normal3(x * rhs, y * rhs, z * rhs);
        }

        Normal3& operator*=(double rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Normal3 operator/(double rhs) const {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            return Normal3(x * inv, y * inv, z * inv);
        }

        Normal3& operator/=(double rhs) {
            assert(rhs != 0);
            double inv = static_cast<double>(1) / rhs;
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }

        double LengthSquare() const {
            return Square(x) + Square(y) + Square(z);
        }

        double Length() const {
            return sqrt(LengthSquare());
        }

        double x;
        double y;
        double z;
    };

    class Point3 {
    public:
        Point3() { x = y = z = 0; }

        Point3(double x, double y, double z) : x(x), y(y), z(z) {
            assert(!HasNaNs());
        }

        explicit operator Vector3() const {
            return Vector3(x, y, z);
        }

        bool HasNaNs() const {
            return std::isnan(x) || std::isnan(y) || std::isnan(z);
        }

        double operator[](int index) const {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        double& operator[](int index) {
            assert(0 <= index && index <= 2);
            if (index == 0) return x;
            if (index == 1) return y;
            return z;
        }

        Point3 operator+(const Point3& p) const {
            return Point3(x + p.x, y + p.y, z + p.z);
        }

        Point3 operator+(const Vector3& v) const {
            return Point3(x + v.x, y + v.y, z + v.z);
        }

        Point3& operator+=(const Vector3& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Point3 operator-(const Vector3& v) const {
            return Point3(x - v.x, y - v.y, z - v.z);
        }

        Point3& operator-=(const Vector3& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3 operator-(const Point3& p) const {
            return Vector3(x - p.x, y - p.y, z - p.z);
        }

        Point3 operator*(double rhs) const {
            return Point3(rhs * x, rhs * y, rhs * z);
        }

        Point3 operator/(double rhs) const {
            double inv = 1.0 / rhs;
            return Point3(rhs * inv, rhs * inv, rhs * inv);
        }

        Point3& operator/=(double rhs) {
            double inv = 1.0 / rhs;
            this->x *= inv;
            this->y *= inv;
            this->z *= inv;
            return *this;
        }

        Point3& operator*=(double rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        double x;
        double y;
        double z;
    };

    inline Point2::Point2(const Point3& p) : x(p.x), y(p.y) {
        assert(!HasNaNs());
    }

    inline Vector2::Vector2(const Point2& p) : x(p.x), y(p.y) {
        assert(!HasNaNs());
    }

    inline Vector3::Vector3(const Normal3& n) : x(n.x), y(n.y), z(n.z) {
        assert(!HasNaNs());
    }

    inline Vector3::Vector3(const Point3& p) : x(p.x), y(p.y), z(p.z) {
        assert(!HasNaNs());
    }

    inline Vector2 operator*(double x, const Vector2& v) {
        return v * x;
    }

    inline Point2 operator*(double x, const Point2& p) {
        return p * x;
    }

    inline Vector3 operator*(double x, const Vector3& v) {
        return v * x;
    }

    inline Point3 operator*(double x, const Point3& p) {
        return p * x;
    }

    inline Normal3 operator*(double x, const Normal3& n) {
        return n * x;
    }

    inline double Distance(const Point2& p1, const Point2& p2) {
        return (p1 - p2).Length();
    }

    inline double DistanceSquare(const Point2& p1, const Point2& p2) {
        return (p1 - p2).LengthSquare();
    }

    inline double Distance(const Point3& p1, const Point3& p2) {
        return (p1 - p2).Length();
    }

    inline double DistanceSquare(const Point3& p1, const Point3& p2) {
        return (p1 - p2).LengthSquare();
    }

    inline double Lerp(double t, double p0, double p1) {
        return (1.0 - t) * p0 + t * p1;
    }

    inline Point2 Lerp(double t, const Point2& p0, const Point2& p1) {
        return (1 - t) * p0 + t * p1;
    }

    inline Point3 Lerp(double t, const Point3& p0, const Point3& p1) {
        return (1 - t) * p0 + t * p1;
    }

    inline Point2 Max(const Point2& p1, const Point2& p2) {
        return Point2(
            std::max(p1.x, p2.x),
            std::max(p1.y, p2.y)
        );
    }

    inline Point3 Max(const Point3& p1, const Point3& p2) {
        return Point3(
            std::max(p1.x, p2.x),
            std::max(p1.y, p2.y),
            std::max(p1.z, p2.z)
        );
    }

    inline Point2 Min(const Point2& p1, const Point2& p2) {
        return Point2(
            std::min(p1.x, p2.x),
            std::min(p1.y, p2.y)
        );
    }

    inline Point3 Min(const Point3& p1, const Point3& p2) {
        return Point3(
            std::min(p1.x, p2.x),
            std::min(p1.y, p2.y),
            std::min(p1.z, p2.z)
        );
    }

    inline Point2 Abs(const Point2& p) {
        return Point2(std::abs(p.x), std::abs(p.y));
    }

    inline Point3 Abs(const Point3& p) {
        return Point3(std::abs(p.x), std::abs(p.y), std::abs(p.z));
    }

    inline Vector3 Abs(const Vector3& v) {
        return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
    }

    inline Point2 Floor(const Point2& p) {
        return Point2(std::floor(p.x), std::floor(p.y));
    }

    inline Point3 Floor(const Point3& p) {
        return Point3(std::floor(p.x), std::floor(p.y), std::floor(p.z));
    }

    inline Vector3 Floor(const Vector3& v) {
        return Vector3(std::floor(v.x), std::floor(v.y), std::floor(v.z));
    }

    inline Point2 Ceil(const Point2& p) {
        return Point2(std::ceil(p.x), std::ceil(p.y));
    }

    inline Point3 Ceil(const Point3& p) {
        return Point3(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
    }

    inline Vector3 Ceil(const Vector3& v) {
        return Vector3(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z));
    }

    inline Point2 Permute(const Point2& p, int x, int y) {
        return Point2(p[x], p[y]);
    }

    inline Point3 Permute(const Point3& p, int x, int y, int z) {
        return Point3(p[x], p[y], p[z]);
    }

    inline Vector2 PairwiseMul(const Vector2& vec1, const Vector2& vec2) {
        return Vector2(vec1.x * vec2.x, vec1.y * vec2.y);
    }

    inline Vector3 PairwiseMul(const Vector3& vec1, const Vector3& vec2) {
        return Vector3(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
    }

    inline Vector2 Normalize(const Vector2& v) {
        return v / v.Length();
    }

    inline Vector3 Normalize(const Vector3& v) {
        return v / v.Length();
    }

    inline Normal3 Normalize(const Normal3& n) {
        return n / n.Length();
    }

    inline double Dot(const Vector2& v1, const Vector2& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline double Dot(const Vector3& v1, const Vector3& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline double Dot(const Vector3& v, const Normal3& n) {
        return v.x * n.x + v.y * n.y + v.z * n.z;
    }

    inline double Dot(const Normal3& n, const Vector3& v) {
        return v.x * n.x + v.y * n.y + v.z * n.z;
    }

    inline double Dot(const Normal3& n1, const Normal3& n2) {
        return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
    }

    inline double AbsDot(const Vector2& v1, const Vector2& v2) {
        return std::abs(Dot(v1, v2));
    }

    inline double AbsDot(const Vector3& v1, const Vector3& v2) {
        return std::abs(Dot(v1, v2));
    }

    inline double AbsDot(const Vector3& v, const Normal3& n) {
        return std::abs(Dot(v, n));
    }

    inline double AbsDot(const Normal3& n, const Vector3& v) {
        return std::abs(Dot(n, v));
    }

    inline double AbsDot(const Normal3& n1, const Normal3& n2) {
        return std::abs(Dot(n1, n2));
    }

    inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
        double v1x = v1.x, v1y = v1.y, v1z = v1.z;
        double v2x = v2.x, v2y = v2.y, v2z = v2.z;
        return Vector3(
            v1y * v2z - v1z * v2y,
            v1z * v2x - v1x * v2z,
            v1x * v2y - v1y * v2x);
    }

    inline Normal3 FaceForward(const Normal3& n, const Vector3& v) {
        // 翻转法线n使其与给定向量v位于同一半球
        return (Dot(n, v) < 0) ? -n : n;
    }


    inline double Clamp(double x, double min = 0, double max = 1) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    inline Vector3 Clamp(Vector3 v, double min = 0, double max = 1) {
        return Vector3(Clamp(v.x, min, max), Clamp(v.y, min, max), Clamp(v.z, min, max));
    }

    inline Vector3 LimitVector(const Vector3& v) {
        return (v + Vector3(1, 1, 1)) * 0.5;
    }

    inline Vector3 Reflect(const Vector3& v, const Normal3& n) {
        // n is a unit vector, v has no limit
        // v is point to hit point
        // return v' is point out hit point
        return v - 2 * Dot(v, n) * Vector3(n);
    }

    inline Vector3 Refract(const Vector3& uv, const Normal3& n, double etai_over_etat) {
        double cos_theta = std::min(Dot(-uv, n), 1.0);
        Vector3 r_out_perp = etai_over_etat * (uv + Vector3(cos_theta * n));
        Vector3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.LengthSquare())) * Vector3(n);
        return r_out_perp + r_out_parallel;
    }

    inline Vector3 Vector3::RandomUnitVector() {
        return Normalize(RandomInUnitSphere());
    }

    inline Vector3 Vector3::RandomInHemisphere(const Normal3& normal) {
        Vector3 in_unit_sphere = RandomInUnitSphere();
        if (Dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return in_unit_sphere;
        else
            return -in_unit_sphere;
    }

    inline std::ostream& operator<<(std::ostream& out, const Vector2& v) {
        out << "Vector2[" << v.x << ", " << v.y << "]";
        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, const Point2& v) {
        out << "Point2[" << v.x << ", " << v.y << "]";
        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
        out << "Vector3[" << v.x << ", " << v.y << ", " << v.z << "]";
        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, const Point3& v) {
        out << "Point3[" << v.x << ", " << v.y << ", " << v.z << "]";
        return out;
    }

    inline std::ostream& operator<<(std::ostream& out, const Normal3& v) {
        out << "Normal3[" << v.x << ", " << v.y << ", " << v.z << "]";
        return out;
    }
}