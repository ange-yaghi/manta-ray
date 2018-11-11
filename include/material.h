#ifndef MATERIAL_H
#define MATERIAL_H

#include <manta_math.h>

#include <new>

namespace manta {

	class RayEmitterGroup;
	class LightRay;
	struct IntersectionPoint;

	class Material {
	public:
		Material();
		~Material();

		void setEmission(const math::Vector &emission) { m_emission = emission; }
		math::Vector getEmission() { return m_emission; }

		void setDiffuseColor(const math::Vector &diffuse) { m_diffuseColor = diffuse; }
		math::Vector getDiffuseColor() { return m_diffuseColor; }

		void setDiffuseCoeff(float diffuseCoeff) { m_diffuseCoeff = diffuseCoeff; }
		float getDiffuseCoeff() const { return m_diffuseCoeff; }

		RayEmitterGroup *generateRayEmitters(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree) const;
		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter) const;
		void destroyEmitterGroup(RayEmitterGroup *group);

	protected:
		math::Vector m_diffuseColor;
		math::Vector m_emission;
		
		float m_diffuseCoeff;

	protected:
		virtual void preconfigureEmitterGroup(RayEmitterGroup *group, int degree) const {}

		template<typename t_RayEmitterGroupType>
		t_RayEmitterGroupType *createEmitterGroup(int degree) const {
			void *buffer = _aligned_malloc(sizeof(t_RayEmitterGroupType), 16);
			t_RayEmitterGroupType *newEmitter = new(buffer) t_RayEmitterGroupType;

			preconfigureEmitterGroup(newEmitter, degree);

			newEmitter->setDegree(degree);
			newEmitter->createAllEmitters();

			return newEmitter;
		}

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree) const;
	};

}

#endif /* MATERIAL_H */