#ifndef MANTARAY_JOB_QUEUE_H
#define MANTARAY_JOB_QUEUE_H

#include <mutex>
#include <queue>

namespace manta {

    class CameraRayEmitterGroup;
    class Scene;
    class ImagePlane;

    struct Job {
        CameraRayEmitterGroup *group;
        ImagePlane *target;
        const Scene *scene;
        int startX;
        int startY;
        int endX;
        int endY;
        int samples;
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

#endif /* MANTARAY_JOB_QUEUE_H */
