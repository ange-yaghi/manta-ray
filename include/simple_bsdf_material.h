#ifndef MANTARAY_SIMPLE_BSDF_MATERIAL_H
#define MANTARAY_SIMPLE_BSDF_MATERIAL_H

#include "material.h"

#include "vector_node_output.h"

namespace manta {

	// Forward declarations
	class BSDF;

	class SimpleBSDFMaterial : public Material {
	public:
		SimpleBSDFMaterial();
		virtual ~SimpleBSDFMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		void generateRays(RayContainer *rays, const LightRay &incidentRay, 
			const IntersectionPoint &intersectionPoint, int degree, 
			StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, 
			const IntersectionPoint &intersectionPoint) const;

		void setReflectanceNode(VectorNodeOutput *node) { m_reflectanceNode = node; }
		const VectorNodeOutput *getReflectanceNode() const;

		void setEmissionNode(VectorNodeOutput *node) { m_emissionNode = node; }
		const VectorNodeOutput *getEmissionNode() const;

		void setEmission(const math::Vector &emission) { m_emission = emission; }
		math::Vector getEmission() { return m_emission; }

		void setReflectance(const math::Vector &reflectance) { m_reflectance = reflectance; }
		math::Vector getReflectance() { return m_reflectance; }

		void setBSDF(const BSDF *bsdf) { m_defaultBsdf = bsdf; }
		const BSDF *getDefaultBSDF() const { return m_defaultBsdf; }

	protected:
		virtual void _initialize();
		virtual void registerInputs();

		int m_maxDegree;

		piranha::pNodeInput m_reflectanceNode;
		piranha::pNodeInput m_emissionNode;

		// Single BSDF
		piranha::pNodeInput m_bsdf;
		const BSDF *m_defaultBsdf;

		// Fixed parameters
		math::Vector m_emission;
		math::Vector m_reflectance;
	};

} /* namespace manta */

#endif /* MANTARAY_SIMPLE_BSDF_MATERIAL_H */
