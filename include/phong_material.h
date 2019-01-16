#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include <material.h>

namespace manta {

	class PhongMaterial : public Material {
	public:
		enum EMITTERS {
			SPECULAR_EMITTER,
			DIFFUSE_EMITTER
		};
	public:
		PhongMaterial();
		virtual ~PhongMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const;

		virtual void configureEmitterGroup(RayEmitterGroup *group, int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint) const;

		void setSpecularPower(math::real specularPower) { m_specularPower = specularPower; }
		math::real getSpecularPower() const { return m_specularPower; }

		void setSpecularColor(const math::Vector &specularColor) { m_specularColor = specularColor; }
		math::Vector getSpecularColor() const { return m_specularColor; }

		void setSurfaceTransmission(math::real t) { m_surfaceTransmission = t; }
		math::real getSurfaceTransmission() const { return m_surfaceTransmission; }

	protected:
		int m_maxDegree;

		math::real m_specularPower;
		math::Vector m_specularColor;
		math::real m_surfaceTransmission;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;

	};

} /* namespace manta */

#endif /* PHONG_MATERIAL_H */
