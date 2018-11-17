#include <ray_tracer.h>

#include <material.h>
#include <light_ray.h>
#include <ray_emitter.h>
#include <ray_emitter_group.h>
#include <scene.h>
#include <scene_object.h>
#include <intersection_point.h>
#include <scene_geometry.h>
#include <stack_allocator.h>
#include <memory_management.h>
#include <worker.h>

#include <iostream>
#include <thread>
#include <chrono>

manta::RayTracer::RayTracer() {
}

manta::RayTracer::~RayTracer() {

}

void manta::RayTracer::traceAll(const Scene *scene, RayEmitterGroup *group) {
	// Simple performance metrics for now
	auto startTime = std::chrono::system_clock::now();

	// Set up the emitter group
	group->setDegree(0);
	group->setStackAllocator(&m_stack);
	group->createAllEmitters();

	// Create jobs
	int emitterCount = group->getEmitterCount();
	int start = 0;
	int end = 0;
	bool done = false;

	while (!done) {
		end = start + m_renderBlockSize - 1;

		if (end >= emitterCount - 1) {
			end = emitterCount - 1;
			done = true;
		}

		Job newJob;
		newJob.scene = scene;
		newJob.group = group;
		newJob.start = start;
		newJob.end = end;

		m_jobQueue.push(newJob);

		start += m_renderBlockSize;
	}

	// Create and start all threads
	createWorkers();
	startWorkers();
	waitForWorkers();

	auto endTime = std::chrono::system_clock::now();

	std::chrono::duration<double> diff = endTime - startTime;
	std::cout << "Total processing time: " << diff.count() << " s" << std::endl;
}

void manta::RayTracer::traceRayEmitter(const Scene *scene, const RayEmitter *emitter, StackAllocator *stackAllocator) const {
	LightRay *rays = emitter->getRays();
	int rayCount = emitter->getRayCount();

	for (int i = 0; i < rayCount; i++) {
		LightRay *ray = &rays[i];

		math::Vector average = math::constants::Zero;
		math::Vector samples = math::loadScalar((math::real)emitter->getSamplesPerRay());
		for (int s = 0; s < emitter->getSamplesPerRay(); s++) {
			traceRay(scene, ray, emitter->getDegree(), stackAllocator);
			average = math::add(average, math::div(ray->getIntensity(), samples));
		}
		ray->setIntensity(average);
	}
}

void manta::RayTracer::initialize(unsigned int stackSize, unsigned int workerStackSize, int threadCount, int renderBlockSize, bool multithreaded) {
	m_stack.initialize(stackSize);
	m_renderBlockSize = renderBlockSize;
	m_multithreaded = multithreaded;
	m_threadCount = threadCount;
	m_workerStackSize = workerStackSize;
}

void manta::RayTracer::destroy() {
	destroyWorkers();
}

void manta::RayTracer::incrementRayCompletion(const Job *job) {
	m_outputLock.lock();
	int emitterCount = job->group->getEmitterCount();
	m_currentRay += 1;
	if (m_currentRay % 1000 == 0) {
		std::cout << "Ray " << m_currentRay << "/" << emitterCount << std::endl;
	}
	m_outputLock.unlock();
}

void manta::RayTracer::createWorkers() {
	m_workers = new Worker[m_threadCount];

	for (int i = 0; i < m_threadCount; i++) {
		m_workers[i].initialize(m_workerStackSize, this);
	}
}

void manta::RayTracer::startWorkers() {
	int workerCount = m_threadCount;

	for (int i = 0; i < workerCount; i++) {
		m_workers[i].start(m_multithreaded);
	}
}

void manta::RayTracer::waitForWorkers() {
	int workerCount = m_threadCount;

	for (int i = 0; i < workerCount; i++) {
		m_workers[i].join();
	}
}

void manta::RayTracer::destroyWorkers() {
	int workerCount = m_threadCount;

	for (int i = 0; i < workerCount; i++) {
		m_workers[i].destroy();
	}

	delete[] m_workers;
	m_workers = nullptr;
}

void manta::RayTracer::depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point) const {
	int objectCount = scene->getSceneObjectCount();

	math::real minDepth = math::REAL_MAX;
	SceneObject *currentClosest = nullptr;
	IntersectionPoint closestIntersection;
	closestIntersection.m_intersection = false;

	IntersectionPoint intersection;
	for (int i = 0; i < objectCount; i++) {
		SceneObject *object = scene->getSceneObject(i);
		SceneGeometry *geometry = object->getGeometry();
		
		if (!geometry->fastIntersection(ray)) continue;

		geometry->detectIntersection(ray, &intersection);

		if (intersection.m_intersection) {
			if (intersection.m_depth < minDepth) {
				if (intersection.m_depth >= 1E-5) {
					minDepth = intersection.m_depth;
					currentClosest = object;
					closestIntersection = intersection;
				}
			}
		}
	}

	*point = closestIntersection;
	*closestObject = currentClosest;
}

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *stack) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point);

	if (sceneObject != nullptr) {
		Material *material = sceneObject->getMaterial();

		RayEmitterGroup *group = material->generateRayEmitterGroup(ray, &point, degree + 1, stack);

		if (group != nullptr) {
			traceRayEmitterGroup(scene, group, stack);
		}

		material->integrateRay(ray, group);

		if (group != nullptr) {
			int emitterCount = group->getEmitterCount();

			// Data must be freed in reverse order
			for (int i = emitterCount - 1; i >= 0; i--) {
				group->getEmitters()[i]->destroyRays();
			}

			material->destroyEmitterGroup(group, stack);
		}
	}
}

void manta::RayTracer::traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, StackAllocator *s) const {
	int emitterCount = rayEmitterGroup->getEmitterCount();
	RayEmitter **emitters = rayEmitterGroup->getEmitters();

	for (int i = 0; i < emitterCount; i++) {
		RayEmitter *emitter = emitters[i];
		emitter->setStackAllocator(s);
		emitter->generateRays();
		traceRayEmitter(scene, emitter, s);
	}
}
