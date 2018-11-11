#ifndef SPHERE_PRIMITIVE_H
#define SPHERE_PRIMITIVE_H

#include <scene_geometry.h>

namespace manta {

	class SpherePrimitive : public SceneGeometry {
	public:
		SpherePrimitive();
		~SpherePrimitive();

		virtual void detectIntersection(const LightRay *ray, IntersectionPoint *p);

		float getRadius() const { return m_radius; }
		void setRadius(float radius) { m_radius = radius; }

	protected:
		float m_radius;
	};

} /* namespace manta */

#endif /* SPHERE_PRIMITIVE_H */
