#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <mutex>
#include <queue>

namespace manta {

	class CameraRayEmitterGroup;
	class Scene;
	class SceneBuffer;

	struct Job {
		CameraRayEmitterGroup *group;
		SceneBuffer *target;
		const Scene *scene;
		int startX;
		int startY;
		int endX;
		int endY;
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
