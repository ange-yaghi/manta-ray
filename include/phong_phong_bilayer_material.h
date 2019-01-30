#ifndef PHONG_PHONG_BILAYER_MATERIAL_H
#define PHONG_PHONG_BILAYER_MATERIAL_H

#include <material.h>

#include <lambertian_bsdf.h>
#include <phong_bsdf.h>
#include <dielectric_media_interface.h>

namespace manta {

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

	protected:
		int m_maxDegree;

		PhongBSDF m_diffuseBSDF;
		PhongBSDF m_specularBSDF;
		DielectricMediaInterface m_coatingFresnel;
		bool m_phongOnly;
		
		math::Vector m_specularColor;
		math::real m_surfaceTransmission;
	};

} /* namespace manta */

#endif /* PHONG_PHONG_BILAYER_MATERIAL_H */
