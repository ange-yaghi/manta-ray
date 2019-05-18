#ifndef SPHERE_PRIMITIVE_H
#define SPHERE_PRIMITIVE_H

#include <scene_geometry.h>

#include <runtime_statistics.h>

namespace manta {

	class SpherePrimitive : public SceneGeometry {
	public:
		enum DetectionMode {
			CONVEX_ONLY,
			CONCAVE_ONLY,
			STANDARD
		};

	public:
		SpherePrimitive();
		virtual ~SpherePrimitive();

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, 
			math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;
		virtual math::Vector getClosestPoint(const CoarseIntersection *hint, 
			const math::Vector &p) const;
		virtual void getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, 
			SceneObject *object) const;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, 
			const CoarseIntersection *hint) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		void detectIntersection(const LightRay *ray, IntersectionPoint *convex, 
			IntersectionPoint *concave) const;

		math::real getRadius() const { return m_radius; }
		void setRadius(math::real radius) { m_radius = radius; }

	protected:
		math::real m_radius;
	};

} /* namespace manta */

#endif /* SPHERE_PRIMITIVE_H */
