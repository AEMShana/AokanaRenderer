#pragma once
#include "vec.h"
#include "transform.h"

namespace Aokana {

	class TriangleMesh {
	public:
		TriangleMesh(
			const Transform& render_from_object, bool reverse_orientation,
			std::vector<int> indices, std::vector<Point3> p,
			std::vector<Vector3> s, std::vector<Normal3> n,
			std::vector<Point2> uv, std::vector<int> face_indices) :
			triangle_count(indices.size() / 3), vertex_count(p.size()) {
			// TODO
		}


	public:
		int triangle_count;
		int vertex_count;
		const int* indices{ nullptr };
		const Point3* p{ nullptr };		// Position
		const Normal3* n{ nullptr };	// Normal
		const Vector3* s{ nullptr };	// Tangent
		const Point2* uv{ nullptr };	// Tex Coord
		bool reverse_orientation;
		bool transform_swaps_handedness;
	};
}