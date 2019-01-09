#include <worker.h>

#include <job_queue.h>
#include <ray_tracer.h>
#include <ray_emitter.h>
#include <ray_emitter_group.h>
#include <path_recorder.h>
#include <light_ray.h>

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

void manta::Worker::initialize(unsigned int stackSize, RayTracer *rayTracer, int workerId, bool deterministicSeed, const std::string &pathRecorderOutputDirectory) {
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
	RayEmitter **emitters = job->group->getEmitters();

	for (int i = job->start; i <= job->end; i++) {
		RayEmitter *emitter = emitters[i];

		if (m_deterministicSeed) {
			// Seed the random number generator with the emitter index
			// This is useful for exactly replicating a run with a different number of pixels

			// Compute pseudorandom LCG
			constexpr __int64 a = 1664525;
			constexpr __int64 c = 1013904223;
			unsigned __int64 xn = (a * i + c) % 0xFFFFFFFF;
			srand((unsigned int)xn);
		}

		if (emitter != nullptr) {
			emitter->setStackAllocator(m_stack);
			m_rayTracer->traceRayEmitter(job->scene, emitter, m_stack /**/ PATH_RECORDER_ARG);
		}
		m_rayTracer->incrementRayCompletion(job);
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
