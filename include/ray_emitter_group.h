#ifndef RAY_EMITTER_GROUP_H
#define RAY_EMITTER_GROUP_H

#include <stack_allocator.h>

#include <malloc.h>
#include <new>

namespace manta {

	class RayEmitter;

	class RayEmitterGroup {
	public:
		RayEmitterGroup();
		virtual ~RayEmitterGroup();

		virtual void createAllEmitters() = 0;

		template<typename t_RayEmitterType>
		t_RayEmitterType *createEmitter() {
			t_RayEmitterType *newEmitter = nullptr;
			if (m_stackAllocator == nullptr) {
				void *buffer = _aligned_malloc(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
				newEmitter->setDegree(m_degree);
			}
			else {
				void *buffer = m_stackAllocator->allocate(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
				newEmitter->setDegree(m_degree);
			}

			return newEmitter;
		}

		void initializeEmitters(int count);
		void destroyEmitters();

		int getEmitterCount() const { return m_rayEmitterCount; }
		RayEmitter **getEmitters() const { return m_rayEmitters; }
		RayEmitter *getEmitter(int index) const { return m_rayEmitters[index]; }

		void setDegree(int degree) { m_degree = degree; }
		int getDegree() const { return m_degree; }

		void setStackAllocator(StackAllocator *allocator) { m_stackAllocator = allocator; }
		StackAllocator *getStackAllocator() const { return m_stackAllocator; }

	protected:
		RayEmitter **m_rayEmitters;
		int m_rayEmitterCount;

		int m_degree;

	private:
		StackAllocator *m_stackAllocator;
	};

}

#endif /* RAY_EMITTER_GROUP_H */

