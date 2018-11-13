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

#include <iostream>
#include <thread>

manta::RayTracer::RayTracer() {
	m_secondaryAllocators = nullptr;
}

manta::RayTracer::~RayTracer() {

}

void manta::RayTracer::traceAll(const Scene *scene, RayEmitterGroup *group) {
	// Set up the emitter group
	group->setDegree(0);
	group->setStackAllocator(&m_mainAllocator);
	group->createAllEmitters();

	if (m_threadCount > 0) {
		int raysPerThread = group->getEmitterCount() / m_threadCount;
		m_rayCount = group->getEmitterCount();
		m_currentRay = 0;

		std::thread **threads = new std::thread *[m_threadCount];

		for (int i = 0; i < m_threadCount; i++) {
			int start = i * raysPerThread;
			int end = (i + 1) * raysPerThread - 1;

			if (i == m_threadCount - 1) {
				end = group->getEmitterCount() - 1;
			}

			threads[i] = new std::thread(&RayTracer::traceRayEmitterGroupThread, this, scene, group, start, end, m_secondaryAllocators[i]);
		}

		for (int i = 0; i < m_threadCount; i++) {
			threads[i]->join();
			delete threads[i];
		}
		delete[] threads;
	}
	else {
		traceRayEmitterGroupThread(scene, group, 0, group->getEmitterCount() - 1, &m_mainAllocator);
	}
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

/*
void manta::RayTracer::traceRayGroupThread(const Scene * scene, const RayEmitter * emitter, int start, int end) {
	LightRay *rays = emitter->getRays();
	int rayCount = emitter->getRayCount();

	for (int i = start; i <= end; i++) {
		LightRay *ray = &rays[i];

		math::Vector average = math::constants::Zero;
		math::Vector samples = math::loadScalar(emitter->getSamplesPerRay());
		for (int s = 0; s < emitter->getSamplesPerRay(); s++) {
			traceRay(scene, ray, emitter->getDegree());
			average = math::add(average, math::div(ray->getIntensity(), samples));
		}
		ray->setIntensity(average);

		
		m_outputLock.lock();
		m_currentRay += 1;
		if (m_currentRay % 1000 == 0) {
			std::cout << "Ray " << m_currentRay << "/" << rayCount << std::endl;
		}
		m_outputLock.unlock();
	}
}*/

void manta::RayTracer::initializeAllocators(unsigned int mainAllocatorSize, unsigned int secondaryAllocatorSize) {
	m_mainAllocator.initialize(mainAllocatorSize);

	if (m_threadCount > 0) {
		m_secondaryAllocators = new StackAllocator *[m_threadCount];
		if (secondaryAllocatorSize > 0) {
			for (int i = 0; i < m_threadCount; i++) {
				m_secondaryAllocators[i] = new StackAllocator;
				m_secondaryAllocators[i]->initialize(secondaryAllocatorSize);
			}
		}
		else {
			for (int i = 0; i < m_threadCount; i++) {
				m_secondaryAllocators[i] = nullptr;
			}
		}
	}
	else {
		m_secondaryAllocators = nullptr;
	}
}

void manta::RayTracer::destroyAllocators() {
	for (int i = 0; i < m_threadCount; i++) {
		delete m_secondaryAllocators[i];
	}

	delete[] m_secondaryAllocators;
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

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point);

	if (sceneObject != nullptr) {
		Material *material = sceneObject->getMaterial();

		RayEmitterGroup *group = material->generateRayEmitters(ray, &point, degree + 1, s);

		if (group != nullptr) {
			traceRayEmitterGroup(scene, group, s);
			material->integrateRay(ray, group);

			int emitterCount = group->getEmitterCount();
			for (int i = emitterCount - 1; i >= 0; i--) {
				group->getEmitters()[i]->destroyRays();
			}

			material->destroyEmitterGroup(group, s);
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

void manta::RayTracer::traceRayEmitterGroupThread(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, int start, int end, StackAllocator *s) {
	int emitterCount = rayEmitterGroup->getEmitterCount();
	RayEmitter **emitters = rayEmitterGroup->getEmitters();

	for (int i = start; i <= end; i++) {
		RayEmitter *emitter = emitters[i];
		emitter->setStackAllocator(s);
		emitter->generateRays();
		traceRayEmitter(scene, emitter, s);

		m_outputLock.lock();
		m_currentRay += 1;
		if (m_currentRay % 1000 == 0) {
			std::cout << "Ray " << m_currentRay << "/" << emitterCount << std::endl;
		}
		m_outputLock.unlock();
	}
}
