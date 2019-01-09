#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include <manta_math.h>

namespace manta {

	class LightRay;
	class IntersectionList;
	struct IntersectionPoint;
	class SceneObject;
	struct CoarseIntersection;
	class StackAllocator;

	class SceneGeometry {
	public:
		SceneGeometry();
		~SceneGeometry();

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const = 0;
		virtual math::Vector getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const = 0;
		virtual void getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const = 0;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const = 0;
		virtual bool fastIntersection(const LightRay *ray) const = 0;

		void setId(int id);
		int getId();

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

	protected:
		math::Vector m_position;

		int m_id;
	};

} /* namespace manta */

#endif /* SCENE_GEOMETRY_H */
