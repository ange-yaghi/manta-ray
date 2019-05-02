#ifndef MATERIAL_POINTER_H
#define MATERIAL_POINTER_H

#include <material.h>

namespace manta {

	class MaterialPointer : public Material {
	public:
		MaterialPointer();
		virtual ~MaterialPointer();

		virtual void generateRays(RayContainer *rays, const LightRay &incidentRay,
			const IntersectionPoint &intersectionPoint, int degree, 
			StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, 
			const IntersectionPoint &intersectionPoint) const;

		void setMaterial(const Material *material) { m_material = material; }
		const Material *getMaterial() const { return m_material; }

	protected:
		const Material *m_material;
	};

} /* namespace manta */

#endif /* MATERIAL_POINTER_H */
