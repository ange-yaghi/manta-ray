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

		virtual void preconfigureEmitterGroup(RayEmitterGroup *_group, int degree) const;

		void setSpecularPower(math::real specularPower) { m_specularPower = specularPower; }
		math::real getSpecularPower() const { return m_specularPower; }

		void setSpecularColor(const math::Vector &specularColor) { m_specularColor = specularColor; }
		math::Vector getSpecularColor() const { return m_specularColor; }

	protected:
		int m_maxDegree;

		math::real m_specularPower;
		math::Vector m_specularColor;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;

	};

} /* namespace manta */

#endif /* PHONG_MATERIAL_H */
