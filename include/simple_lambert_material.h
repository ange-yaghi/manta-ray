#ifndef SIMPLE_LAMBERT_MATERIAL_H
#define SIMPLE_LAMBERT_MATERIAL_H

#include <material.h>

#include <lambertian_bsdf.h>
#include <microfacet_reflection_bsdf.h>
#include <phong_distribution.h>

namespace manta {

	class VectorMaterialNode;

	class SimpleLambertMaterial : public Material {
	public:
		SimpleLambertMaterial();
		virtual ~SimpleLambertMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		void generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const;

		void setDiffuseNode(VectorMaterialNode *node) { m_diffuseNode = node; }
		VectorMaterialNode *getDiffuseNode() const { return m_diffuseNode; }

		void setEmission(const math::Vector &emission) { m_emission = emission; }
		math::Vector getEmission() { return m_emission; }

		void setDiffuseColor(const math::Vector &diffuse) { m_diffuseColor = diffuse; }
		math::Vector getDiffuseColor() { return m_diffuseColor; }

	protected:
	public:
		int m_maxDegree;

		VectorMaterialNode *m_diffuseNode;

		// Fixed parameters
		math::Vector m_emission;
		math::Vector m_diffuseColor;

		// BSDF
		//LambertianBSDF m_diffuseBSDF;
		MicrofacetReflectionBSDF m_diffuseBSDF;
		PhongDistribution m_distribution;
	};

} /* namespace manta */

#endif /* SIMPLE_LAMBERT_MATERIAL_H */
