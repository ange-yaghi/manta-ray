#ifndef SIMPLE_SPECULAR_DIFFUSE_MATERIAL_H
#define SIMPLE_SPECULAR_DIFFUSE_MATERIAL_H

#include <material.h>

namespace manta {

	class SimpleSpecularDiffuseMaterial : public Material {
	public:
		SimpleSpecularDiffuseMaterial();
		virtual ~SimpleSpecularDiffuseMaterial();

		void setMaxDiffuseDegree(int degree) { m_maxDiffuseDegree = degree; }
		int getMaxDiffuseDegree() const { return m_maxDiffuseDegree; }

		void setMaxSpecularDegree(int degree) { m_maxSpecularDegree = degree; }
		int getMaxSpecularDegree() const { return m_maxSpecularDegree; }

		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const;

		void setSpecularColor(const math::Vector &specular);
		math::Vector getSpecularColor() const { return m_specularColor; }

		void setDiffuseColor(const math::Vector &diffuse);
		math::Vector getDiffuseColor() const { return m_diffuseColor; }

		void setAutoDisableEmitters(bool autoDisableEmitters);

		int getDiffuseSampleCount(int degree) const;

	protected:
		int m_maxDiffuseDegree;
		int m_maxSpecularDegree;

		math::Vector m_specularColor;
		math::Vector m_diffuseColor;

		bool m_enableDiffuse;
		bool m_enableSpecular;
		bool m_autoDisableEmitters;

		virtual void preconfigureEmitterGroup(RayEmitterGroup *group, int degree) const;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;
	};

}

#endif /* SIMPLE_DIFFUSE_MATERIAL_H */
