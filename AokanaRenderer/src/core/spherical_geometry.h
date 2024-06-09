#pragma once

#include "vec.h"
#include "utils.h"

namespace Aokana::SphericalGeometry {

	// https://pbr-book.org/4ed/Geometry_and_Transformations/Spherical_Geometry#DirectionCone

	// Girard's theorem: 
	// 单位球面上的三角形面积 A = \alpha + \beta + \gamma - \pi，其中 \alpha, \beta, \gamma 是球面三角形的内角
	// 单位球面上的四边形面积 A = \alpha + \beta + \gamma + \delta - 2 * \pi，其中 \alpha, \beta, \gamma, \delta 是球面四边形的内角

	// 计算单位球面上的三角形面积，其中 a, b, c 是从球心到球面的单位向量
	double SphericalTriangleArea(const Vector3& a, const Vector3& b, const Vector3& c) {
		return std::abs(2 * std::atan2(Dot(a, Cross(b, c)), 
			1 + Dot(a, b) + Dot(a, c) + Dot(b, c)));
	}

	// 计算单位球面上的四边形面积，其中 a, b, c, d 是从球心到球面的单位向量
	double SphericalQuadArea(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d) {


	}
	
	Vector3 SphericalDirection(double sin_theta, double cos_theta, double phi) {
		// x = r * sin(theta) * cos(phi)
		// y = r * sin(theta) * sin(phi)
		// z = r * cos(theta)

		return Vector3(
			Clamp(sin_theta, -1.0, 1.0) * std::cos(phi),
			Clamp(sin_theta, -1.0, 1.0) * std::sin(phi),
			Clamp(cos_theta, -1.0, 1.0)
		);
	}



}