#ifndef MATERIAL_H
#define MATERIAL_H

#include <manta_math.h>

#include <stack_allocator.h>

#include <new>
#include <string>

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

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

		RayEmitterGroup *generateRayEmitterGroup(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator = nullptr) const;
		virtual void integrateRay(LightRay *ray, const RayEmitterGroup *_rayEmitter, const IntersectionPoint *intersectionPoint) const;
		void destroyEmitterGroup(RayEmitterGroup *group, StackAllocator *stackAllocator = nullptr);

		void setIndex(int index) { m_index = index; }
		int getIndex() const { return m_index; }

	protected:
		math::Vector m_diffuseColor;
		math::Vector m_emission;
		std::string m_name;

	protected:
		virtual void configureEmitterGroup(RayEmitterGroup *group, int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint) const {};

		template<typename t_RayEmitterGroupType>
		t_RayEmitterGroupType *createEmitterGroup(int degree, const LightRay *ray, const IntersectionPoint *intersectionPoint, StackAllocator *stackAllocator = nullptr) const {
			void *buffer = nullptr;
			if (stackAllocator == nullptr) {
				buffer = _aligned_malloc(sizeof(t_RayEmitterGroupType), 16);
			}
			else {
				buffer = stackAllocator->allocate(sizeof(t_RayEmitterGroupType), 16);
			}

			t_RayEmitterGroupType *newEmitter = new(buffer) t_RayEmitterGroupType;

			newEmitter->setDegree(degree);
			newEmitter->setStackAllocator(stackAllocator);
			configureEmitterGroup(newEmitter, degree, ray, intersectionPoint);

			newEmitter->createAllEmitters();

			return newEmitter;
		}

		virtual RayEmitterGroup *generateRayEmittersInternal(const LightRay *ray, const IntersectionPoint *intersectionPoint, int degree, StackAllocator *stackAllocator) const;

		int m_index;
	};

}

#endif /* MATERIAL_H */
