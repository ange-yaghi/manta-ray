#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include <manta_math.h>

namespace manta {

	class LightRay;
	struct IntersectionPoint;

	class SceneGeometry {
	public:
		SceneGeometry();
		~SceneGeometry();

		virtual void detectIntersection(const LightRay *ray, IntersectionPoint *p) const = 0;

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
