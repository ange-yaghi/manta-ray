#include "../include/worker.h"

#include "../include/job_queue.h"
#include "../include/ray_tracer.h"
#include "../include/path_recorder.h"
#include "../include/light_ray.h"
#include "../include/camera_ray_emitter_group.h"
#include "../include/camera_ray_emitter.h"
#include "../include/ray_container.h"
#include "../include/image_plane.h"
#include "../include/stratified_sampler.h"

#include <sstream>
#include <time.h>

manta::Worker::Worker() {
    m_stack = nullptr;
    m_thread = nullptr;

    m_deterministicSeed = false;

    m_sampler = nullptr;
}

manta::Worker::~Worker() {
    assert(m_stack == nullptr);
    assert(m_thread == nullptr);
}

void manta::Worker::initialize(mem_size stackSize, RayTracer *rayTracer, int workerId, 
    bool deterministicSeed, const std::string &pathRecorderOutputDirectory, unsigned int seed) 
{
    if (stackSize > 0) {
        m_stack = new StackAllocator;
        m_stack->initialize(stackSize);
    }
    else {
        m_stack = nullptr;
    }

    m_rayTracer = rayTracer;
    m_deterministicSeed = deterministicSeed;
    m_workerId = workerId;
    m_pathRecorderOutputDirectory = pathRecorderOutputDirectory;

    m_sampler = m_rayTracer->getSampler()->clone();
    m_sampler->seed(seed);

    // Initialize all statistics
    m_statistics.reset();

    // Attach the worker id to the intersection point manager
    m_ipManager.setThreadId(workerId);
}

void manta::Worker::start(bool multithreaded) {
    if (multithreaded) {
        m_thread = new std::thread(&Worker::work, this);
    }
    else {
        m_thread = nullptr;
        work();
    }
}

void manta::Worker::join() {
    if (m_thread != nullptr) {
        m_thread->join();
    }
    else {
        // This is part of the main thread so there is no reason to join
    }
}

void manta::Worker::destroy() {
    if (m_stack != nullptr) {
        delete m_stack;
        m_stack = nullptr;
    }
    delete m_thread;
    m_thread = nullptr;
}

std::string manta::Worker::getTreeName(int pixelIndex, int sample) const {
    std::stringstream ss;
    ss << "PATH_" << pixelIndex << "_S" << sample;

    return ss.str();
}

void manta::Worker::work() {
    Job currentJob;
    while (m_rayTracer->getJobQueue()->pop(&currentJob)) {
        doJob(&currentJob);
    }

    // Output the path recordings to a 3D object file
    PATH_RECORDER_OUTPUT(getObjFname());

    // Record statistics
    if (m_stack != nullptr) m_maxMemoryUsage = m_stack->getMaxUsage();
    else m_maxMemoryUsage = 0;
}

void manta::Worker::doJob(const Job *job) {
    constexpr int SAMPLE_BUFFER_CAPACITY = 0x1 << 7;

    int sampleCount = 0;
    ImageSample *samples = (ImageSample *)m_stack->allocate(sizeof(ImageSample) * SAMPLE_BUFFER_CAPACITY, 16);

    for (int x = job->startX; x <= job->endX; x++) {
        for (int y = job->startY; y <= job->endY; y++) {
            m_sampler->startPixelSession();

            CameraRayEmitter *emitter = job->group->createEmitter(x, y, m_stack);
            emitter->setSampler(m_sampler);
            int pixelIndex = job->group->getResolutionX() * y + x;

            if (m_deterministicSeed) {
                // Seed the random number generator with the emitter index
                // This is useful for exactly replicating a run with a different number of pixels

                // Compute pseudorandom LCG
                constexpr __int64 a = 1664525;
                constexpr __int64 c = 1013904223;
                unsigned __int64 xn = (a * pixelIndex + c) % 0xFFFFFFFF;
                srand((unsigned int)xn);
            }

            math::Vector result = math::constants::Zero;

            if (emitter != nullptr) {
                emitter->setStackAllocator(m_stack);

                do {
                    NEW_TREE(getTreeName(pixelIndex, samp), emitter->getPosition());
                    LightRay ray; emitter->generateRay(&ray);
                    ray.calculateTransformations();

                    math::Vector L = m_rayTracer->traceRay(job->scene, &ray, 0, &m_ipManager, m_sampler,
                        m_stack /**/ PATH_RECORDER_ARG /**/ STATISTICS_ROOT(&m_statistics));

                    ImageSample &sample = samples[sampleCount++];
                    sample.imagePlaneLocation = ray.getImagePlaneLocation();
                    sample.intensity = L;

                    if (sampleCount >= SAMPLE_BUFFER_CAPACITY) {
                        job->target->processSamples(samples, sampleCount, m_stack);
                        sampleCount = 0;
                    }

                    END_TREE();

                } while (emitter->getSampler()->startNextSample());
            }
            m_rayTracer->incrementRayCompletion(job);

            job->group->freeEmitter(emitter, m_stack);
        }
    }

    if (sampleCount > 0) {
        job->target->processSamples(samples, sampleCount, m_stack);
        sampleCount = 0;
    }

    m_stack->free((void *)samples);
}

std::string manta::Worker::getObjFname() {
    time_t rawTime;
    struct tm timeInfo;
    char buffer[256];

    time(&rawTime);
    localtime_s(&timeInfo, &rawTime);

    strftime(buffer, 256, "diagnostics_%F_%H%M%S", &timeInfo);

    std::stringstream ss;
    ss << m_workerId << ".obj";

    return m_pathRecorderOutputDirectory + std::string(buffer) + "worker_" + ss.str();
}
