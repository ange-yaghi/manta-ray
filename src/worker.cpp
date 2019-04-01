#include <worker.h>

#include <job_queue.h>
#include <ray_tracer.h>
#include <path_recorder.h>
#include <light_ray.h>
#include <camera_ray_emitter_group.h>
#include <camera_ray_emitter.h>
#include <ray_container.h>
#include <image_plane.h>

#include <sstream>
#include <time.h>

manta::Worker::Worker() {
	m_stack = nullptr;
	m_thread = nullptr;

	m_deterministicSeed = false;
}

manta::Worker::~Worker() {
	assert(m_stack == nullptr);
	assert(m_thread == nullptr);
}

void manta::Worker::initialize(mem_size stackSize, RayTracer *rayTracer, int workerId, bool deterministicSeed, const std::string &pathRecorderOutputDirectory) {
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

	// Initialize all statistics
	m_statistics.reset();
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
	for (int x = job->startX; x <= job->endX; x++) {
		for (int y = job->startY; y <= job->endY; y++) {
			CameraRayEmitter *emitter = job->group->createEmitter(x, y, m_stack);
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
				RayContainer container;
				container.setStackAllocator(m_stack);

				emitter->setStackAllocator(m_stack);
				emitter->generateRays(&container);

				LightRay *rays = container.getRays();
				int rayCount = container.getRayCount();

				for (int samp = 0; samp < rayCount; samp++) {
					NEW_TREE(getTreeName(i, samp), emitter->getPosition());
					LightRay *ray = &rays[samp];
					ray->calculateTransformations();

					m_rayTracer->traceRay(job->scene, ray, 0, m_stack /**/ PATH_RECORDER_ARG /**/ STATISTICS_ROOT(&m_statistics));
					END_TREE();
				}

				container.calculateIntensity();
				container.destroyRays();

				result = container.getIntensity();
			}
			m_rayTracer->incrementRayCompletion(job);

			job->group->freeEmitter(emitter, m_stack);

			// Add the results to the scene buffer target
			constexpr math::Vector DEBUG_RED = { (math::real)1.0, (math::real)0.0, (math::real)0.0 };
			constexpr math::Vector DEBUG_BLUE = { (math::real)0.0, (math::real)0.0, (math::real)1.0 };
			constexpr math::Vector DEBUG_GREEN = { (math::real)0.0, (math::real)1.0, (math::real)0.0 };

			if (std::isnan(math::getX(result)) || std::isnan(math::getY(result)) || std::isnan(math::getZ(result))) {
				result = DEBUG_RED;
			}
			else if (std::isinf(math::getX(result)) || std::isinf(math::getY(result)) || std::isinf(math::getZ(result))) {
				result = DEBUG_GREEN;
			}
			else if (math::getX(result) < 0 || math::getY(result) < 0 || math::getZ(result) < 0) {
				result = DEBUG_BLUE;
			}

			job->target->set(result, x, y);
		}
	}
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
