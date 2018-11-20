#include <worker.h>

#include <job_queue.h>
#include <ray_tracer.h>

#include <ray_emitter.h>
#include <ray_emitter_group.h>

manta::Worker::Worker() {
	m_start = 0;
	m_end = 0;

	m_stack = nullptr;
	m_thread = nullptr;
}

manta::Worker::~Worker() {
	assert(m_stack == nullptr);
	assert(m_thread == nullptr);
}

void manta::Worker::initialize(unsigned int stackSize, RayTracer *rayTracer) {
	if (stackSize > 0) {
		m_stack = new StackAllocator;
		m_stack->initialize(stackSize);
	}
	else {
		m_stack = nullptr;
	}

	m_rayTracer = rayTracer;
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
}

void manta::Worker::doJob(const Job *job) {
	RayEmitter **emitters = job->group->getEmitters();

	for (int i = job->start; i <= job->end; i++) {
		RayEmitter *emitter = emitters[i];

		if (emitter != nullptr) {
			emitter->setStackAllocator(m_stack);
			emitter->generateRays();
			m_rayTracer->traceRayEmitter(job->scene, emitter, m_stack);
		}
		m_rayTracer->incrementRayCompletion(job);
	}
}
