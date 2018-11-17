#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <atomic>
#include <stack_allocator.h>

namespace manta {

	class RayEmitterGroup;
	class Scene;
	class RayTracer;
	struct Job;

	class Worker {
	public:
		Worker();
		~Worker();

		void initialize(unsigned int stackSize, RayTracer *rayTracer);
		void start(bool multithreaded = true);
		void join();
		void destroy();

	protected:
		void work();
		void doJob(const Job *job);

		StackAllocator *m_stack;
		RayTracer *m_rayTracer;

		std::thread *m_thread;
		
		int m_start;
		int m_end;
	};

}

#endif /* WORKER_H */
