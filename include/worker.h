#ifndef WORKER_H
#define WORKER_H

#include <stack_allocator.h>
#include <path_recorder.h>
#include <manta_conf.h>

#include <thread>
#include <atomic>

#if ENABLE_PATH_RECORDING
#define PATH_RECORDER_ARG ,&m_pathRecorder
#define NEW_TREE(name, origin) m_pathRecorder.startNewTree(name, origin)
#define END_TREE()
#define PATH_RECORDER_OUTPUT(fname) m_pathRecorder.writeObjFile(fname)
#else
#define PATH_RECORDER_ARG
#define NEW_TREE(name, origin)
#define END_TREE()
#define PATH_RECORDER_OUTPUT(fname)
#endif

namespace manta {

	class CameraRayEmitterGroup;
	class Scene;
	class RayTracer;
	struct Job;

	class Worker {
	public:
		Worker();
		~Worker();

		void initialize(unsigned int stackSize, RayTracer *rayTracer, int workerId, bool deterministicSeed, const std::string &pathRecorderOutputDirectory);
		void start(bool multithreaded = true);
		void join();
		void destroy();

		unsigned int getMaxMemoryUsage() const { return m_maxMemoryUsage; }
		std::string getTreeName(int pixelIndex, int sample) const;

	protected:
		void work();
		void doJob(const Job *job);

		StackAllocator *m_stack;
		RayTracer *m_rayTracer;

		std::thread *m_thread;	

	protected:
		// Debugging/Path recording
		std::string getObjFname();

		PathRecorder m_pathRecorder;
		std::string m_pathRecorderOutputDirectory;
		bool m_deterministicSeed;

	protected:
		// Identification
		int m_workerId;

	protected:
		// Statistics
		unsigned int m_maxMemoryUsage;
	};

}

#endif /* WORKER_H */
