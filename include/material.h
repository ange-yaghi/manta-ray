#ifndef MATERIAL_H
#define MATERIAL_H

#include <manta_math.h>

#include <stack_allocator.h>

#include <new>
#include <string>

namespace manta {

	struct IntersectionPoint;
	class RayContainer;
	class LightRay;

	class Material {
	public:
		Material();
		~Material();

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		virtual void generateRays(RayContainer *rays, const LightRay &ray, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const = 0;
		virtual void integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const = 0;

		void setIndex(int index) { m_index = index; }
		int getIndex() const { return m_index; }

	protected:
		std::string m_name;

	protected:
		int m_index;
	};

} /* namespace manta */

#endif /* MATERIAL_H */
