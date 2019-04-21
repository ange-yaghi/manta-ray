#ifndef SIMPLE_BSDF_MATERIAL_H
#define SIMPLE_BSDF_MATERIAL_H

#include <material.h>

namespace manta {

	class VectorNode;
	class BSDF;

	class SimpleBSDFMaterial : public Material {
	public:
		SimpleBSDFMaterial();
		virtual ~SimpleBSDFMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		void generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const;

		void setReflectanceNode(VectorNode *node) { m_reflectanceNode = node; }
		VectorNode *getReflectanceNode() const { return m_reflectanceNode; }

		void setEmissionNode(VectorNode *node) { m_emissionNode = node; }
		VectorNode *getEmissionNode() const { return m_emissionNode; }

		void setEmission(const math::Vector &emission) { m_emission = emission; }
		math::Vector getEmission() { return m_emission; }

		void setReflectance(const math::Vector &reflectance) { m_reflectance = reflectance; }
		math::Vector getReflectance() { return m_reflectance; }

		void setBSDF(BSDF *bsdf) { m_bsdf = bsdf; }
		BSDF *getBSDF() const { return m_bsdf; }

	protected:
		int m_maxDegree;

		VectorNode *m_reflectanceNode;
		VectorNode *m_emissionNode;

		// Fixed parameters
		math::Vector m_emission;
		math::Vector m_reflectance;

		// Single BSDF
		BSDF *m_bsdf;
	};

} /* namespace manta */

#endif /* SIMPLE_BSDF_MATERIAL_H */
