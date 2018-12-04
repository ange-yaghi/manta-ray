#ifndef OCTREE_H
#define OCTREE_H

#include <manta_math.h>
#include <geometry.h>
#include <scene_geometry.h>

#include <vector>

namespace manta {

	class Mesh;
	class SceneObject;

	class Octree : public SceneGeometry {
	public:
		struct TempFace{
			int face;
			bool presentInChild;
		};
	public:
		Octree();
		~Octree();

		void initialize(math::real width, const math::Vector &position);
		void destroy();

		virtual const CoarseIntersection *coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, const CoarseIntersection *reference, math::real epsilon) const;
		virtual void fineIntersection(const LightRay *ray, IntersectionPoint *p, const CoarseIntersection *hint, math::real bleed) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		void analyze(SceneObject *object, int maxSize);

	protected:
		void analyze(SceneObject *object, Octree *parent, int maxSize);

		bool checkVertex(const math::Vector &v, math::real epsilon) const;

		int getUsageInternal() const { return (int)m_tempFaces.size(); }

		bool AABBIntersect(const LightRay *ray, math::real *depth) const;

		math::real m_width;
		math::Vector m_maxPoint;
		math::Vector m_minPoint;

		std::vector<TempFace> m_tempFaces;
		Mesh *m_mesh;

		Octree *m_children;
	};

} /* namespace manta */

#endif /* OCTREE_H */
