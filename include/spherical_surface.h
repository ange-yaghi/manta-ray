#ifndef SPHERICAL_SURFACE_H
#define SPHERICAL_SURFACE_H

#include <manta_math.h>

#include <sphere_primitive.h>

namespace manta {

	// Forward declarations
	class LightRay;

	class SphericalSurface {
	public:
		SphericalSurface();
		~SphericalSurface();

		virtual bool transformLightRay(const LightRay *ray, bool convex, 
			bool incoming, LightRay *transformed) const;

		SpherePrimitive *getSphere() { return &m_sphere; };

		math::real getRadius() const { return m_sphere.getRadius(); }

		void setIor(math::real ior) { m_ior = ior; }
		math::real getIor() const { return m_ior; }

	protected:
		SpherePrimitive m_sphere;
		math::real m_ior;
	};

} /* namespace manta */

#endif /* SPHERICAL_SURFACE_H */
