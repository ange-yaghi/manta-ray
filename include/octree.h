#ifndef OCTREE_H
#define OCTREE_H

#include <manta_math.h>
#include <geometry.h>
#include <scene_geometry.h>
#include <stack_list.h>

#include <vector>
#include <fstream>

namespace manta {

	class Mesh;
	class SceneObject;
	class StackAllocator;

	struct OctreeBV {
		math::Vector maxPoint;
		math::Vector minPoint;
		unsigned int childList;
		unsigned int faceList;
		unsigned short int faceCount;
		unsigned char childCount;

		char padding[5];
	};

	class Octree : public SceneGeometry {
	public:
		struct TempFace{
			int face;
			bool presentInChild;
		};

		struct OctreeLeafCollision {
			const Octree *leaf;
			math::real depth;
		};

		static const int MAX_DEPTH = 50;

	public:
		Octree();
		~Octree();

		void initialize(math::real width, const math::Vector &position);
		void destroy();

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const;
		virtual math::Vector getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const;
		virtual void getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		void analyze(Mesh *mesh, int maxSize);

		void writeToObjFile(const char *fname, const LightRay *ray) const;

		int countFaces() const;
		int countLeaves() const;

	protected:
	public:
		int countFaces(const OctreeBV *leaf) const;
		int countLeaves(const OctreeBV *leaf) const;

		bool findClosestIntersection(const OctreeBV *leaf, const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const;
		void getVicinity(const OctreeBV *leaf, const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const;

		bool analyze(Mesh *mesh, OctreeBV *leaf, int maxSize, std::vector<int> &facePool);
		void shrink(OctreeBV *leaf);
		void deleteChild(int childIndex);
		void clear();

		bool checkVertex(const OctreeBV *leaf, const math::Vector &v, math::real epsilon) const;
		bool checkPlane(const OctreeBV *leaf, const math::Vector &n, math::real d) const;
		bool checkTriangle(const OctreeBV *leaf, const math::Vector &v0, const math::Vector &v1, const math::Vector &v2) const;

		bool AABBIntersect(const OctreeBV *leaf, const LightRay *ray, math::real *depth) const;
		bool AABBIntersect(const OctreeBV *leaf, const math::Vector &p, math::real radius) const;

		void writeToObjFile(const OctreeBV *leaf, std::ofstream &f, int &currentLeaf, const LightRay *ray) const;

		Mesh *m_mesh;
		OctreeBV m_tree;

		std::vector<OctreeBV *>m_childLists;
		std::vector<int *>m_faceLists;
	};

} /* namespace manta */

#endif /* OCTREE_H */
