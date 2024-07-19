#include "triangle.h"

namespace Aokana {

	TriangleMesh::TriangleMesh(const Transform& render_from_object, bool reverse_orientation, std::vector<int> indices, std::vector<Point3> p, std::vector<Vector3> s, std::vector<Normal3> n, std::vector<Point2> uv, std::vector<int> face_indices) :
		triangle_count(indices.size() / 3), vertex_count(p.size()) {
		this->indices = g_int_buffer_cache->LookupOrAdd(indices);
		for (Point3& point : p) {
			point = render_from_object.Apply(point);
		}
		this->p = g_point3_buffer_cache->LookupOrAdd(p);
		this->reverse_orientation = reverse_orientation;
		this->transform_swaps_handedness = render_from_object.SwapsHandedness();

		// TODO:
		// 
		//if (!uv.empty()) {
		//	CHECK_EQ(nVertices, uv.size());
		//	this->uv = point2BufferCache->LookupOrAdd(uv, alloc);
		//}
		//if (!n.empty()) {
		//	CHECK_EQ(nVertices, n.size());
		//	for (Normal3f& nn : n) {
		//		nn = renderFromObject(nn);
		//		if (reverseOrientation)
		//			nn = -nn;
		//	}
		//	this->n = normal3BufferCache->LookupOrAdd(n, alloc);
		//}
		//if (!s.empty()) {
		//	CHECK_EQ(nVertices, s.size());
		//	for (Vector3f& ss : s)
		//		ss = renderFromObject(ss);
		//	this->s = vector3BufferCache->LookupOrAdd(s, alloc);
		//}

		//if (!faceIndices.empty()) {
		//	CHECK_EQ(nTriangles, faceIndices.size());
		//	this->faceIndices = intBufferCache->LookupOrAdd(faceIndices, alloc);
		//}

		//// Make sure that we don't have too much stuff to be using integers to
		//// index into things.
		//CHECK_LE(p.size(), std::numeric_limits<int>::max());
		//// We could be clever and check indices.size() / 3 if we were careful
		//// to promote to a 64-bit int before multiplying by 3 when we look up
		//// in the indices array...
		//CHECK_LE(indices.size(), std::numeric_limits<int>::max());
	}
}