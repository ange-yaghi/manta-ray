#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include <manta_math.h>
#include <runtime_statistics.h>

namespace manta {

	// Forward declarations
	struct IntersectionPoint;
	struct CoarseIntersection;
	class LightRay;
	class IntersectionList;
	class SceneObject;
	class StackAllocator;

	class SceneGeometry {
	public:
		SceneGeometry();
		~SceneGeometry();

		virtual bool findClosestIntersection(const LightRay *ray, 
			CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, 
			StackAllocator *s /**/ STATISTICS_PROTOTYPE) const = 0;
		virtual void fineIntersection(const math::Vector &r, 
			IntersectionPoint *p, const CoarseIntersection *hint) const = 0;
		virtual bool fastIntersection(const LightRay *ray) const = 0;

		void setId(int id);
		int getId() const;

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

	protected:
		math::Vector m_position;

		int m_id;
	};

} /* namespace manta */

#endif /* SCENE_GEOMETRY_H */
