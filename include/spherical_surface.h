#ifndef SPHERICAL_SURFACE_H
#define SPHERICAL_SURFACE_H

#include <manta_math.h>

#include <sphere_primitive.h>

namespace manta {

	class LightRay;

	class SphericalSurface {
	public:
		SphericalSurface();
		~SphericalSurface();

		virtual bool transformLightRay(const LightRay *ray, LightRay *transformed) const;

		void setConvex(bool convex) { m_convex = convex; }
		bool isConvex() const { return m_convex; }

		void setInput(bool input) { m_input = input; }
		bool isInput() const { return m_input; }

		SpherePrimitive *getSphere() { return &m_sphere; };

		math::real getRadius() const { return m_sphere.getRadius(); }

		void setIOR(math::real ior) { m_ior = ior; }
		math::real getIOR() const { return m_ior; }

	protected:
		SpherePrimitive m_sphere;
		math::real m_ior;

		bool m_convex;
		bool m_input;
	};

} /* namespace manta */

#endif /* SPHERICAL_SURFACE_H */