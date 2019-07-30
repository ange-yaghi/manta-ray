#ifndef MANTARAY_MATERIAL_H
#define MANTARAY_MATERIAL_H

#include "object_reference_node.h"

#include "manta_math.h"
#include "stack_allocator.h"

#include <new>
#include <string>

namespace manta {

	// Forward declarations
	struct IntersectionPoint;
	class RayContainer;
	class LightRay;

	class Material : public ObjectReferenceNode<Material> {
	public:
		Material();
		virtual	~Material();

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		virtual void generateRays(RayContainer *rays, const LightRay &ray, 
			const IntersectionPoint &intersectionPoint, int degree,
			StackAllocator *stackAllocator = nullptr) const = 0;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, 
			const IntersectionPoint &intersectionPoint) const = 0;

		void setIndex(int index) { m_index = index; }
		int getIndex() const { return m_index; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();

		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_library;
		piranha::pNodeInput m_nameInput;

	protected:
		std::string m_name;

	protected:
		int m_index;
	};

} /* namespace manta */

#endif /* MANTARAY_MATERIAL_H */
