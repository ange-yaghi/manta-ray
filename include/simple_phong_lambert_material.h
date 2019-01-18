#ifndef SIMPLE_PHONG_MATERIAL_H
#define SIMPLE_PHONG_MATERIAL_H

#include <material.h>

#include <lambertian_bsdf.h>
#include <phong_bsdf.h>

namespace manta {

	class SimplePhongLambertMaterial : public Material {
	public:
		enum EMITTERS {
			SPECULAR_EMITTER,
			DIFFUSE_EMITTER
		};
	public:
		SimplePhongLambertMaterial();
		virtual ~SimplePhongLambertMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const;

		virtual void configureEmitterGroup(RayEmitterGroup *group, int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint) const;

		LambertianBSDF *getDiffuseBSDF() { return &m_diffuseBSDF; }
		PhongBSDF *getSpecularBSDF() { return &m_specularBSDF; }

		void setSpecularColor(const math::Vector &specularColor) { m_specularColor = specularColor; }
		math::Vector getSpecularColor() const { return m_specularColor; }

		void setSurfaceTransmission(math::real t) { m_surfaceTransmission = t; }
		math::real getSurfaceTransmission() const { return m_surfaceTransmission; }

	protected:
		int m_maxDegree;

		LambertianBSDF m_diffuseBSDF;
		PhongBSDF m_specularBSDF;
		
		math::Vector m_specularColor;
		math::real m_surfaceTransmission;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;

	};

} /* namespace manta */

#endif /* SIMPLE_PHONG_MATERIAL_H */
