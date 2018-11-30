#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include <manta_math.h>

namespace manta {

	class LightRay;
	class IntersectionList;
	struct IntersectionPoint;
	struct CoarseIntersection;
	class SceneObject;

	class SceneGeometry {
	public:
		SceneGeometry();
		~SceneGeometry();

		virtual math::real coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, math::real depthHint, math::real epsilon) const = 0;
		virtual void fineIntersection(const LightRay *ray, IntersectionPoint *p, CoarseIntersection *hint, math::real bleed) const = 0;
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
