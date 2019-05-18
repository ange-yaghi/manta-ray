#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <stack_allocator.h>
#include <job_queue.h>
#include <manta_math.h>
#include <manta_build_conf.h>
#include <material_manager.h>
#include <runtime_statistics.h>

#include <atomic>
#include <mutex>

// Utilities for path recording
#if ENABLE_PATH_RECORDING
#define PATH_RECORDER_DECL , PathRecorder *pathRecorder
#define PATH_RECORDER_VAR , pathRecorder
#define START_BRANCH(location) pathRecorder->startBranch(location)
#define END_BRANCH() pathRecorder->endBranch()
#else
#define PATH_RECORDER_DECL
#define PATH_RECORDER_VAR
#define START_BRANCH(location)
#define END_BRANCH()
#endif

namespace manta {

	struct IntersectionPoint;
	struct CoarseIntersection;
	class LightRay;
	class Scene;
	class CameraRayEmitter;
	class CameraRayEmitterGroup;
	class SceneObject;
	class Worker;
	class PathRecorder;
	class RayContainer;
	class ImagePlane;

	class RayTracer {
	public:
		RayTracer();
		~RayTracer();

		void traceAll(const Scene *scene, CameraRayEmitterGroup *rayEmitterGroup, 
			ImagePlane *target);
		void tracePixel(int px, int py, const Scene *scene, 
			CameraRayEmitterGroup *rayEmitterGroup, ImagePlane *target);
		void traceRayEmitter(const CameraRayEmitter *emitter, RayContainer *container, 
			const Scene *scene, StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const;

		int getThreadCount() const { return m_threadCount; }

		void initialize(mem_size stackSize, mem_size workerStackSize, int threadCount, 
			int renderBlockSize, bool multithreaded);
		void destroy();

		void setBackgroundColor(const math::Vector &color) { m_backgroundColor = color; }

		// Interface to workers
		JobQueue *getJobQueue() { return &m_jobQueue; }
		
		void traceRay(const Scene *scene, LightRay *ray, int degree, 
			StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const;
		void incrementRayCompletion(const Job *job, int increment = 1);

		void setDeterministicSeedMode(bool enable) { m_deterministicSeed = enable; }
		bool isDeterministicSeedMode() const { return m_deterministicSeed; }

		void setPathRecordingOutputDirectory(const std::string &s) { m_pathRecordingOutputDirectory = s; }
		std::string getPathRecordingOutputDirector() const { return m_pathRecordingOutputDirectory; }

		MaterialManager *getMaterialManager() { return &m_materialManager; }

	protected:
		// Multithreading features
		JobQueue m_jobQueue;
		Worker *m_workers;
		Worker *m_singleThreadedWorker;

		mem_size m_workerStackSize;
		int m_rayCount;
		int m_threadCount;
		int m_renderBlockSize;
		bool m_multithreaded;

		void createWorkers();
		void startWorkers();
		void waitForWorkers();
		void destroyWorkers();

	protected:
		// Ray tracing features
		void depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, 
			IntersectionPoint *point, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;
		void refineContact(const LightRay *ray, math::real depth, IntersectionPoint *point, 
			SceneObject **closestObject, StackAllocator *s) const;

		void traceRays(const Scene *scene, const RayContainer &rayContainer, 
			StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const;

		math::Vector m_backgroundColor;

	protected:
		// Material library
		MaterialManager m_materialManager;

	protected:
		// Statistics
		std::atomic<int> m_currentRay;
		std::mutex m_outputLock;

		StackAllocator m_stack;

	protected:
		// Debugging and path recording
		std::string m_pathRecordingOutputDirectory;
		bool m_deterministicSeed;
	};

} /* namespace manta */

#endif /* RAY_TRACER_H */
