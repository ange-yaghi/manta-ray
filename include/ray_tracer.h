#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <atomic>
#include <mutex>

#include <stack_allocator.h>

namespace manta {

	class LightRay;
	class Scene;
	class RayEmitter;
	class RayEmitterGroup;
	class SceneObject;
	struct IntersectionPoint;

	class RayTracer {
	public:
		RayTracer();
		~RayTracer();

		void traceAll(const Scene *scene, RayEmitterGroup *rayEmitterGroup);

		void setThreadCount(int threadCount) { m_threadCount = threadCount; }
		int getThreadCount() const { return m_threadCount; }

		void initializeAllocators(unsigned int mainAllocatorSize, unsigned int secondaryAllocatorSize);
		void destroyAllocators();

	protected:
		void depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point) const;

		void traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s) const;
		void traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, StackAllocator *s) const;
		void traceRayEmitter(const Scene *scene, const RayEmitter *emitter, StackAllocator *s) const;
		//void traceRayEmitterThread(const Scene *scene, const RayEmitter *emitter, int start, int end);
		void traceRayEmitterGroupThread(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, int start, int end, StackAllocator *s);

		std::atomic<int> m_currentRay;
		std::mutex m_outputLock;
		int m_rayCount;
		int m_threadCount;

		StackAllocator m_mainAllocator;
		StackAllocator **m_secondaryAllocators;
	};

} /* namespace manta */

#endif /* RAY_TRACER_H */