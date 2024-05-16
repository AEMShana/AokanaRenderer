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

}

