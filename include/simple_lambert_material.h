#ifndef SIMPLE_LAMBERT_MATERIAL_H
#define SIMPLE_LAMBERT_MATERIAL_H

#include <material.h>

#include <lambertian_bsdf.h>

namespace manta {

	class SimpleLambertMaterial : public Material {
	public:
		SimpleLambertMaterial();
		virtual ~SimpleLambertMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		void generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const;

	protected:
		int m_maxDegree;
		LambertianBSDF m_diffuseBSDF;
	};

} /* namespace manta */

#endif /* SIMPLE_LAMBERT_MATERIAL_H */

