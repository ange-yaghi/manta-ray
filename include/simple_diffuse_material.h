#ifndef SIMPLE_DIFFUSE_MATERIAL_H
#define SIMPLE_DIFFUSE_MATERIAL_H

#include <material.h>

namespace manta {

	class SimpleDiffuseMaterial : public Material {
	public:
		SimpleDiffuseMaterial();
		virtual ~SimpleDiffuseMaterial();

		void setMaxDegree(int degree) { m_maxDegree = degree; }
		int getMaxDegree() const { return m_maxDegree; }

		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const;

	protected:
		int m_maxDegree;

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;

	};

} /* namespace manta */

#endif /* SIMPLE_DIFFUSE_MATERIAL_H */
