#ifndef PHONG_PHONG_BILAYER_MATERIAL_H
#define PHONG_PHONG_BILAYER_MATERIAL_H

#include <material.h>

#include <lambertian_bsdf.h>
#include <phong_bsdf.h>
#include <dielectric_media_interface.h>

namespace manta {

	class VectorMaterialNode;

	class PhongPhongBilayerMaterial : public Material {
	public:
		enum EMITTERS {
			SPECULAR_EMITTER,
			DIFFUSE_EMITTER
		};

	public:
		PhongPhongBilayerMaterial();
		virtual ~PhongPhongBilayerMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		void generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const;

		PhongBSDF *getDiffuseBSDF() { return &m_diffuseBSDF; }
		PhongBSDF *getSpecularBSDF() { return &m_specularBSDF; }
		DielectricMediaInterface *getCoatingFresnel() { return &m_coatingFresnel; }

		void setSpecularColor(const math::Vector &specularColor) { m_specularColor = specularColor; }
		math::Vector getSpecularColor() const { return m_specularColor; }

		void setSurfaceTransmission(math::real t) { m_surfaceTransmission = t; }
		math::real getSurfaceTransmission() const { return m_surfaceTransmission; }

		void setEmission(const math::Vector &emission) { m_emission = emission; }
		math::Vector getEmission() { return m_emission; }

		void setDiffuseColor(const math::Vector &diffuse) { m_diffuseColor = diffuse; }
		math::Vector getDiffuseColor() { return m_diffuseColor; }

		void setEmissionNode(VectorMaterialNode *node) { m_emissionNode = node; }
		VectorMaterialNode *getEmissionNode() { return m_emissionNode; }

		void setDiffuseNode(VectorMaterialNode *node) { m_diffuseNode = node; }
		VectorMaterialNode *getDiffuseNode() { return m_diffuseNode; }

		void setSpecularNode(VectorMaterialNode *node) { m_specularNode = node; }
		VectorMaterialNode *getSpecularNode() { return m_specularNode; }

	protected:
		int m_maxDegree;

		// BSDF
		PhongBSDF m_diffuseBSDF;
		PhongBSDF m_specularBSDF;

		DielectricMediaInterface m_coatingFresnel;
		
		// Fixed parameters
		math::Vector m_emission;
		math::Vector m_diffuseColor;
		math::Vector m_specularColor;
		math::real m_surfaceTransmission;

		// Node connections
		VectorMaterialNode *m_emissionNode;
		VectorMaterialNode *m_diffuseNode;
		VectorMaterialNode *m_specularNode;
	};

} /* namespace manta */

#endif /* PHONG_PHONG_BILAYER_MATERIAL_H */
