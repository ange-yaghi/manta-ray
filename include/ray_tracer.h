#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <atomic>
#include <mutex>

#include <stack_allocator.h>
#include <job_queue.h>

namespace manta {

	class LightRay;
	class Scene;
	class RayEmitter;
	class RayEmitterGroup;
	class SceneObject;
	struct IntersectionPoint;
	class Worker;

	class RayTracer {
	public:
		RayTracer();
		~RayTracer();

		void traceAll(const Scene *scene, RayEmitterGroup *rayEmitterGroup);

		int getThreadCount() const { return m_threadCount; }

		void initialize(unsigned int stackSize, unsigned int workerStackSize, int threadCount, int renderBlockSize, bool multithreaded);
		void destroy();

		void incrementRayCompletion(const Job *job);

		// Interface to workers
		JobQueue *getJobQueue() { return &m_jobQueue; }
		void traceRayEmitter(const Scene *scene, const RayEmitter *emitter, StackAllocator *stack) const;

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
		void depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point) const;

		void traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s) const;
		void traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, StackAllocator *s) const;

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