#include <ray_tracer.h>

#include <material.h>
#include <light_ray.h>
#include <ray_emitter.h>
#include <ray_emitter_group.h>
#include <scene.h>
#include <scene_object.h>
#include <intersection_point.h>
#include <scene_geometry.h>

#include <iostream>
#include <thread>

manta::RayTracer::RayTracer() {

}

manta::RayTracer::~RayTracer() {

}

void manta::RayTracer::traceAll(const Scene *scene, RayEmitterGroup *group) {
	int threadCount = 12;

	// Set up the emitter group
	group->setDegree(0);
	group->createAllEmitters();

	int raysPerThread = group->getEmitterCount() / threadCount;
	m_rayCount = group->getEmitterCount();
	m_currentRay = 0;

	std::thread **threads = new std::thread *[threadCount];

	for (int i = 0; i < threadCount; i++) {
		int start = i * raysPerThread;
		int end = (i + 1) * raysPerThread - 1;

		if (i == threadCount - 1) {
			end = group->getEmitterCount() - 1;
		}

		threads[i] = new std::thread(&RayTracer::traceRayEmitterGroupThread, this, scene, group, start, end);
	}

	for (int i = 0; i < threadCount; i++) {
		threads[i]->join();
		delete threads[i];
	}
	delete[] threads;
}

void manta::RayTracer::traceRayEmitter(const Scene *scene, const RayEmitter *emitter) const {
	LightRay *rays = emitter->getRays();
	int rayCount = emitter->getRayCount();

	for (int i = 0; i < rayCount; i++) {
		LightRay *ray = &rays[i];

		math::Vector average = math::constants::Zero;
		math::Vector samples = math::loadScalar((math::real)emitter->getSamplesPerRay());
		for (int s = 0; s < emitter->getSamplesPerRay(); s++) {
			traceRay(scene, ray, emitter->getDegree());
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

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point);

	if (sceneObject != nullptr) {
		Material *material = sceneObject->getMaterial();

		RayEmitterGroup *group = material->generateRayEmitters(ray, &point, degree + 1);

		if (group != nullptr) {
			traceRayEmitterGroup(scene, group);
			material->integrateRay(ray, group);
			material->destroyEmitterGroup(group);
		}
	}
}

void manta::RayTracer::traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup) const {
	int emitterCount = rayEmitterGroup->getEmitterCount();
	RayEmitter **emitters = rayEmitterGroup->getEmitters();

	for (int i = 0; i < emitterCount; i++) {
		RayEmitter *emitter = emitters[i];
		emitter->generateRays();
		traceRayEmitter(scene, emitter);
	}
}

void manta::RayTracer::traceRayEmitterGroupThread(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, int start, int end) {
	int emitterCount = rayEmitterGroup->getEmitterCount();
	RayEmitter **emitters = rayEmitterGroup->getEmitters();

	for (int i = start; i <= end; i++) {
		RayEmitter *emitter = emitters[i];
		emitter->generateRays();
		traceRayEmitter(scene, emitter);

		m_outputLock.lock();
		m_currentRay += 1;
		if (m_currentRay % 1000 == 0) {
			std::cout << "Ray " << m_currentRay << "/" << emitterCount << std::endl;
		}
		m_outputLock.unlock();
	}
}
