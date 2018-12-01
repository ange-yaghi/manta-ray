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
	m_start = 0;
	m_end = 0;

	m_stack = nullptr;
	m_thread = nullptr;

	m_deterministicSeed = false;
}

manta::Worker::~Worker() {
	assert(m_stack == nullptr);
	assert(m_thread == nullptr);
}

void manta::Worker::initialize(unsigned int stackSize, RayTracer *rayTracer, int workerId, bool deterministicSeed) {
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
}

void manta::Worker::doJob(const Job *job) {
	RayEmitter **emitters = job->group->getEmitters();

	for (int i = job->start; i <= job->end; i++) {
		RayEmitter *emitter = emitters[i];

		if (m_deterministicSeed) {
			// Seed the random number generator with the emitter index
			// This is useful for exactly replicating a run with a different number of pixels
			srand(i);
		}

		if (emitter != nullptr) {
			emitter->setStackAllocator(m_stack);
			emitter->generateRays();

			LightRay *rays = emitter->getRays();
			int rayCount = emitter->getRayCount();

			for (int j = 0; j < rayCount; j++) {
				LightRay *ray = &rays[j];

				math::Vector average = math::constants::Zero;
				math::Vector samples = math::loadScalar((math::real)emitter->getSamplesPerRay());
				for (int samp = 0; samp < emitter->getSamplesPerRay(); samp++) {
					NEW_TREE(getTreeName(i, samp), emitter->getPosition());

					m_rayTracer->traceRay(job->scene, ray, emitter->getDegree(), m_stack /**/ PATH_RECORDER_ARG);
					average = math::add(average, math::div(ray->getIntensity(), samples));

					END_TREE();
				}
				ray->setIntensity(average);
			}
		}
		m_rayTracer->incrementRayCompletion(job);
	}
}

std::string manta::Worker::getTreeName(int pixelIndex, int sample) {
	std::stringstream ss;
	ss << "PATH_" << pixelIndex << "_S" << sample;

	return ss.str();
}

std::string manta::Worker::getObjFname() {
	time_t rawTime;
	struct tm timeInfo;
	char buffer[256];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);

	strftime(buffer, 256, "diagnostics_%F_%H%M.obj", &timeInfo);

	std::stringstream ss;
	ss << m_workerId;

	return "diagnostics/wId_" + ss.str() + std::string(buffer);
}
