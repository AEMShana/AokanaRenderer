#pragma once

#include "vec.h"

namespace Aokana::SphericalGeometry {

	double SphericalTriangleArea(const Vector3& a, const Vector3& b, const Vector3& c) {
		return std::abs(2 * std::atan2(Dot(a, Cross(b, c)), 
			1 + Dot(a, b) + Dot(a, c) + Dot(b, c)));
	}

	//double SphericalTriangleArea(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d);

	
	Vector3 SphericalDirection(double sin_theta, double cos_theta, double phi) {
		// x = r * sin(theta) * cos(phi)
		// y = r * sin(theta) * sin(phi)
		// z = r * cos(theta)

		return Vector3(
			Clamp(sin_theta, -1, 1) * std::cos(phi),
			Clamp(sin_theta, -1, 1) * std::sin(phi),
			Clamp(cos_theta, -1, 1)
		);
	}



}