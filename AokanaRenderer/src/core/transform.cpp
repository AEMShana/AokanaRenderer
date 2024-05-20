#include "transform.h"

namespace Aokana {

	Transform Aokana::Transform::Inverse(const Transform& transform) {
		return Transform(transform.GetInverseMatrix(), transform.GetMatrix());
	}

	Transform Transform::Transpose(const Transform& transform) {
		return Transform(
			transform.GetMatrix().Transpose(),
			transform.GetInverseMatrix().Transpose());
	}

	Transform Transform::Translate(Vector3 delta) {
		Matrix4x4 matrix(
			1, 0, 0, delta.x,
			0, 1, 0, delta.y,
			0, 0, 1, delta.z,
			0, 0, 0, 1
		);

		Matrix4x4 inv_matrix(
			1, 0, 0, -delta.x,
			0, 1, 0, -delta.y,
			0, 0, 1, -delta.z,
			0, 0, 0, 1
		);

		return Transform(matrix, inv_matrix);
	}

	Transform Transform::Scale(Vector3 scale) {
		Matrix4x4 matrix(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1
		);

		Matrix4x4 inv_matrix(
			1.0 / scale.x, 0, 0, 0,
			0, 1.0 / scale.y, 0, 0,
			0, 0, 1.0 / scale.z, 0,
			0, 0, 0, 1
		);

		return Transform(matrix, inv_matrix);
	}

	Transform Transform::RotateX(double degree) {
		double sin_theta = std::sin(Radians(degree));
		double cos_theta = std::cos(Radians(degree));
		Matrix4x4 mat(
			1, 0, 0, 0,
			0, cos_theta, -sin_theta, 0,
			0, sin_theta, cos_theta, 0,
			0, 0, 0, 1
		);
		return Transform(mat, mat.Transpose());
	}

	Transform Transform::RotateY(double degree) {
		double sin_theta = std::sin(Radians(degree));
		double cos_theta = std::cos(Radians(degree));
		Matrix4x4 mat(
			cos_theta, 0, sin_theta, 0,
			0, 1, 0, 0,
			-sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1
		);
		return Transform(mat, mat.Transpose());
	}

	Transform Transform::RotateZ(double degree) {
		double sin_theta = std::sin(Radians(degree));
		double cos_theta = std::cos(Radians(degree));
		Matrix4x4 mat(
			cos_theta, -sin_theta, 0, 0,
			sin_theta, cos_theta, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		return Transform(mat, mat.Transpose());
	}

	Transform Transform::Rotate(double sin_theta, double cos_theta, Vector3 axis) {
		Vector3 a = Normalize(axis);
		Matrix4x4 mat{};
		mat(0, 0) = a.x * a.x + (1 - a.x * a.x) * cos_theta;
		mat(0, 1) = a.x * a.y * (1 - cos_theta) - a.z * sin_theta;
		mat(0, 2) = a.x * a.z * (1 - cos_theta) + a.y * sin_theta;
		mat(0, 3) = 0;
	
		mat(1, 0) = a.x * a.y * (1 - cos_theta) + a.z * sin_theta;
		mat(1, 1) = a.y * a.y + (1 - a.y * a.y) * cos_theta;
		mat(1, 2) = a.y * a.z * (1 - cos_theta) - a.x * sin_theta;
		mat(1, 3) = 0;
	
		mat(2, 0) = a.x * a.z * (1 - cos_theta) - a.y * sin_theta;
		mat(2, 1) = a.y * a.z * (1 - cos_theta) + a.x * sin_theta;
		mat(2, 2) = a.z * a.z + (1 - a.z * a.z) * cos_theta;
		mat(2, 3) = 0;

		return Transform(mat, mat.Transpose());
	}

	Transform Transform::Rotate(double degree, Vector3 axis) {
		double sin_theta = std::sin(Radians(degree));
		double cos_theta = std::cos(Radians(degree));
		return Rotate(sin_theta, cos_theta, axis);
	}
}

