#ifndef RAY_EMITTER_GROUP_H
#define RAY_EMITTER_GROUP_H

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
			void *buffer = _aligned_malloc(sizeof(t_RayEmitterType), 16);
			t_RayEmitterType *newEmitter = new(buffer) t_RayEmitterType;
			newEmitter->setDegree(m_degree);

			return newEmitter;
		}

		void initializeEmitters(int count);
		void destroyEmitters();

		int getEmitterCount() const { return m_rayEmitterCount; }
		RayEmitter **getEmitters() const { return m_rayEmitters; }

		void setDegree(int degree) { m_degree = degree; }
		int getDegree() const { return m_degree; }

	protected:
		RayEmitter **m_rayEmitters;
		int m_rayEmitterCount;

		int m_degree;
	};

}

#endif /* RAY_EMITTER_GROUP_H */

