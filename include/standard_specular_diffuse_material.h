#ifndef STANDARD_SPECULAR_DIFFUSE_MATERIAL_H
#define STANDARD_SPECULAR_DIFFUSE_MATERIAL_H

#include <material.h>

namespace manta {

	class TextureMap;

	class StandardSpecularDiffuseMaterial : public Material {
	public:
		StandardSpecularDiffuseMaterial();
		virtual ~StandardSpecularDiffuseMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const;

		void setSpecularColor(const math::Vector &specular) { m_specularColor = specular; }
		math::Vector getSpecularColor() const { return m_specularColor; }

		void setDiffuseColor(const math::Vector &diffuse) { m_diffuseColor = diffuse; }
		math::Vector getDiffuseColor() const { return m_diffuseColor; }

		void setDiffuseMap(TextureMap *map) { m_diffuseMap = map; }
		TextureMap *getDiffuseMap() { return m_diffuseMap; }

		void setGloss(math::real gloss) { m_gloss = gloss; }
		math::real getGloss() const { return m_gloss; }

	protected:
		int m_maxDegree;
		math::real m_gloss;

		math::Vector m_specularColor;
		math::Vector m_diffuseColor;

		TextureMap *m_diffuseMap;

		virtual void preconfigureEmitterGroup(RayEmitterGroup *group, int degree) const;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;
	};

}

#endif /* SIMPLE_DIFFUSE_MATERIAL_H */
