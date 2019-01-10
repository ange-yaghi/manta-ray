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

		int countVertices() const;
		int countFaces() const;
		int countLeaves() const;

	protected:
	public:
		// Internal version
		void octreeTest(const LightRay *ray, StackList<OctreeLeafCollision> *list) const;

		void analyze(Mesh *mesh, Octree *parent, int maxSize);
		void shrink();
		void deleteChild(int childIndex);
		void clear();

		bool checkVertex(const math::Vector &v, math::real epsilon) const;
		bool checkPlane(const math::Vector &n, math::real d) const;
		bool checkTriangle(const math::Vector &v0, const math::Vector &v1, const math::Vector &v2) const;

		int getUsageInternal() const { return (int)m_tempFaces.size(); }

		bool AABBIntersect(const LightRay *ray, math::real *depth) const;
		bool AABBIntersect(const math::Vector &p, math::real radius) const;

		void writeToObjFile(std::ofstream &f, int &currentLeaf, const LightRay *ray) const;

		math::real m_width;
		math::Vector m_maxPoint;
		math::Vector m_minPoint;

		std::vector<TempFace> m_tempFaces;
		Mesh *m_mesh;

		Octree *m_children;
		int m_childCount;
	};

} /* namespace manta */

#endif /* OCTREE_H */
