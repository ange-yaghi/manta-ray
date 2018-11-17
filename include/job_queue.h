#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <mutex>
#include <queue>

namespace manta {

	class RayEmitterGroup;
	class Scene;

	struct Job {
		RayEmitterGroup *group;
		const Scene *scene;
		int start;
		int end;
	};

	class JobQueue {
	public:
		JobQueue();
		~JobQueue();

		void push(const Job &job);
		bool pop(Job *job);

	protected:
		std::queue<Job> m_queue;
		std::mutex m_queueLock;
	};

} /* namespace manta */

#endif /* JOB_QUEUE_H */
