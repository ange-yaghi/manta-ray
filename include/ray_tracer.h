#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <stack_allocator.h>
#include <job_queue.h>
#include <manta_math.h>

#include <atomic>
#include <mutex>

namespace manta {

	class LightRay;
	class Scene;
	class RayEmitter;
	class RayEmitterGroup;
	class SceneObject;
	struct IntersectionPoint;
	class IntersectionList;
	class Worker;

	class RayTracer {
	public:
		RayTracer();
		~RayTracer();

		void traceAll(const Scene *scene, RayEmitterGroup *rayEmitterGroup);

		int getThreadCount() const { return m_threadCount; }

		void initialize(unsigned int stackSize, unsigned int workerStackSize, int threadCount, int renderBlockSize, bool multithreaded);
		void destroy();

		void setBackgroundColor(const math::Vector &color) { m_backgroundColor = color; }

		// Interface to workers
		JobQueue *getJobQueue() { return &m_jobQueue; }
		void traceRayEmitter(const Scene *scene, const RayEmitter *emitter, StackAllocator *stack) const;
		void incrementRayCompletion(const Job *job);

	protected:
		// Multithreading features
		JobQueue m_jobQueue;
		Worker *m_workers;
		Worker *m_singleThreadedWorker;

		unsigned m_workerStackSize;

		void createWorkers();
		void startWorkers();
		void waitForWorkers();
		void destroyWorkers();

	protected:
		void depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point, StackAllocator *s) const;
		void fluxMultisample(const LightRay *ray, IntersectionList *list, IntersectionPoint *point, SceneObject **closestObject, math::real minDepth, math::real epsilon, StackAllocator *s) const;

		void traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s) const;
		void traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, StackAllocator *s) const;

		math::Vector m_backgroundColor;

		std::atomic<int> m_currentRay;
		std::mutex m_outputLock;
		int m_rayCount;
		int m_threadCount;
		int m_renderBlockSize;

		StackAllocator m_stack;

		bool m_multithreaded;
	};

} /* namespace manta */

#endif /* RAY_TRACER_H */