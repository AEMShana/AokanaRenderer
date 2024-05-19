#pragma once

#include "matrix.h"
#include "vec.h"
#include "bounds.h"
#include "ray.h"

namespace Aokana {

	class Transform {
	public:
		Transform() = default;
		Transform(const Matrix4x4& matrix) : matrix(matrix), inv_matrix(matrix.Inverse()) {}
		Transform(const Matrix4x4& matrix, const Matrix4x4& inv_matrix) : matrix(matrix), inv_matrix(inv_matrix) {}
		Transform(const double mat[4][4]) : Transform(Matrix4x4(mat)) {}

		const Matrix4x4& GetMatrix() const { return matrix; }
		const Matrix4x4& GetInverseMatrix() const { return inv_matrix; }
		Transform Inverse() const { return Inverse(*this); }
		Transform Transpose() const { return Transpose(*this); }
		bool IsIdentity() const { return matrix.IsIdentity(); }


		static Transform Inverse(const Transform& transform);
		static Transform Transpose(const Transform& transform);
		static Transform Translate(Vector3 delta);
		static Transform Translate(double x, double y, double z) { return Translate(Vector3(x, y, z)); }
		static Transform Scale(Vector3 scale);
		static Transform Scale(double x, double y, double z) { return Scale(x, y, z); }
		static Transform RotateX(double degree);
		static Transform RotateY(double degree);
		static Transform RotateZ(double degree);


	private:
		Matrix4x4 matrix;
		Matrix4x4 inv_matrix;
	};



}