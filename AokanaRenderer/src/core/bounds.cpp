#include "bounds.h"
#include <memory>


namespace Aokana {

	const Point3& Bounds3::operator[](int index) const {
		assert(0 <= index && index <= 1);
		if (index == 0) return p_min;
		return p_max;
	}

	Point3& Bounds3::operator[](int index) {
		assert(0 <= index && index <= 1);
		if (index == 0) return p_min;
		return p_max;
	}

	Point3 Bounds3::Corner(int index) const {
		return Point3(
			(*this)[(index & 1)].x,
			(*this)[(index & 2) ? 1 : 0].y,
			(*this)[(index & 4) ? 1 : 0].z
		);
	}

	double Bounds3::SurfaceArea() const {
		auto d = Diagonal();
		return 2.0 * (d.x * d.y + d.x * d.z + d.y * d.z);
	}

	double Bounds3::Volume() const {
		auto d = Diagonal();
		return d.x * d.y * d.z;
	}

	int Bounds3::MaximumExtent() const { // 返回最长轴的索引
		auto d = Diagonal();
		if (d.x > d.y && d.x > d.z) return 0;
		if (d.y > d.z) return 1;
		return 2;
	}

	Point3 Bounds3::lerp(const Point3& t) const {
		return Point3(
			Lerp(t.x, p_min.x, p_max.x),
			Lerp(t.y, p_min.y, p_max.y),
			Lerp(t.z, p_min.z, p_max.z)
		);
	}

	Vector3 Bounds3::Offset(const Point3& p) const {
		Vector3 o = p - p_min;
		if (p_max.x > p_min.x) o.x /= p_max.x - p_min.x;
		if (p_max.y > p_min.y) o.y /= p_max.y - p_min.y;
		if (p_max.z > p_min.z) o.z /= p_max.z - p_min.z;
		return o;
	}

	void Bounds3::BoundingSphere(Point3* center, double* radius) const {
		*center = (p_min + p_max) / 2;
		*radius = Inside(*center, *this) ? Distance(*center, p_max) : 0;
	}

	bool Bounds3::Hit(const Ray& r, double t_min, double t_max) const {
		for (int a = 0; a < 3; a++) {
			auto invD = 1.0f / r.direction[a];
			auto t0 = (p_min[a] - r.origin[a]) * invD;
			auto t1 = (p_max[a] - r.origin[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)
				return false;
		}
		return true;
	}

	Bounds3 Bounds3::Merge(const Bounds3& b1, const Bounds3& b2) {
		Point3 p_min(
			std::min(b1.p_min.x, b2.p_min.x),
			std::min(b1.p_min.y, b2.p_min.y),
			std::min(b1.p_min.z, b2.p_min.z)
		);

		Point3 p_max(
			std::max(b1.p_max.x, b2.p_max.x),
			std::max(b1.p_max.y, b2.p_max.y),
			std::max(b1.p_max.z, b2.p_max.z)
		);

		return Bounds3(p_min, p_max);
	}

	Bounds3 Bounds3::Merge(const Bounds3& b, const Point3& p) {
		Point3 p_min(
			std::min(b.p_min.x, p.x),
			std::min(b.p_min.y, p.y),
			std::min(b.p_min.z, p.z)
		);

		Point3 p_max(
			std::max(b.p_max.x, p.x),
			std::max(b.p_max.y, p.y),
			std::max(b.p_max.z, p.z)
		);

		return Bounds3(p_min, p_max);
	}

	Bounds3 Bounds3::Intersect(const Bounds3& b1, const Bounds3& b2) {
		return Bounds3(
			Point3(
				std::max(b1.p_min.x, b2.p_min.x),
				std::max(b1.p_min.y, b2.p_min.y),
				std::max(b1.p_min.z, b2.p_min.z)
			),
			Point3(
				std::min(b1.p_min.x, b2.p_min.x),
				std::min(b1.p_min.y, b2.p_min.y),
				std::min(b1.p_min.z, b2.p_min.z)
			)
		);
	}

	bool Bounds3::Inside(const Point3& p, const Bounds3& b) {
		return (
			p.x >= b.p_min.x && p.x <= b.p_max.x &&
			p.y >= b.p_min.y && p.y <= b.p_max.y &&
			p.z >= b.p_min.z && p.z <= b.p_max.z
			);
	}

	bool Bounds3::InsideExclusive(const Point3& p, const Bounds3& b) {
		return (
			p.x >= b.p_min.x && p.x < b.p_max.x &&
			p.y >= b.p_min.y && p.y < b.p_max.y &&
			p.z >= b.p_min.z && p.z < b.p_max.z
			);
	}

	Bounds3 Bounds3::Expand(const Bounds3& b, double delta) {
		return Bounds3(
			b.p_min - Vector3(delta, delta, delta),
			b.p_max + Vector3(delta, delta, delta));
	}
}