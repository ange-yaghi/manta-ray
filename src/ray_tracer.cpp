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
#include <intersection_list.h>

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
		std::cout << "starting worker" << i << std::endl;
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

void manta::RayTracer::depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point, StackAllocator *s) const {
	int objectCount = scene->getSceneObjectCount();

	constexpr math::real epsilon = 1E-4;
	math::real minDepth = math::REAL_MAX - epsilon;
	SceneObject *currentClosest = nullptr;
	//IntersectionPoint closestIntersection;
	//closestIntersection.m_intersection = false;

	IntersectionList list;
	list.setStack(s);

	// First generate a list of coarse intersections
	for (int i = 0; i < objectCount; i++) {
		SceneObject *object = scene->getSceneObject(i);
		SceneGeometry *geometry = object->getGeometry();

		if (!geometry->fastIntersection(ray)) continue;

		minDepth = geometry->coarseIntersection(ray, &list, object, minDepth, epsilon);
	}

	// Refine and resolve the coarse intersections
	/*
	int intersectionCount = list.getIntersectionCount();
	IntersectionPoint intersection;

	for (int i = 0; i < intersectionCount; i++) {
		CoarseIntersection *c = list.getIntersection(i);

		// Only process coarse collisions that are within epsilon
		// of the minimum depth
		if ((c->depth - minDepth) > epsilon) {
			SceneObject *object = c->sceneObject;
			object->getGeometry()->fineIntersection(ray, &intersection, c);

			if (intersection.m_intersection) {
				if (intersection.m_depth < minDepth) {
					minDepth = intersection.m_depth;
					currentClosest = object;
					closestIntersection = intersection;
				}
			}
		}
	}*/

	fluxMultisample(ray, &list, point, closestObject, minDepth, epsilon, s);

	list.destroy();

	//*point = closestIntersection;
	//*closestObject = currentClosest;
}

void manta::RayTracer::fluxMultisample(const LightRay *ray, IntersectionList *list, IntersectionPoint *point, SceneObject **closestObject, math::real minDepth, math::real epsilon, StackAllocator *s) const {
	constexpr math::real SURFACE_BIAS = 1E-4;
	
	int conflicts = 0;
	int intersectionCount = list->getIntersectionCount();
	CoarseIntersection *closest = nullptr;

	// Count the number of real conflicts
	for (int i = 0; i < intersectionCount; i++) {
		CoarseIntersection *c = list->getIntersection(i);
		if ((c->depth - minDepth) < epsilon) {
			conflicts++;

			if (closest == nullptr || c->depth < closest->depth) {
				closest = c;
			}
		}
	}

	if (conflicts == 0) {
		point->m_intersection = false;
		*closestObject = nullptr;
		return;
	}
	else if (conflicts == 1) {
		// There can only be one answer
		closest->sceneObject->getGeometry()->fineIntersection(ray, point, closest);
		*closestObject = closest->sceneObject;

		// Bias the intersection position
		point->m_position = math::add(point->m_position, math::mul(math::loadScalar(SURFACE_BIAS), point->m_faceNormal));
	}
	else {
		// There are multiple conflicts that need to be resolved
		constexpr int SAMPLE_RADIUS = 1;
		constexpr int SAMPLE_WIDTH = SAMPLE_RADIUS * 2 + 1;
		constexpr int SAMPLE_HEIGHT = SAMPLE_RADIUS * 2 + 1;
		constexpr int SAMPLE_COUNT = SAMPLE_WIDTH * SAMPLE_HEIGHT;
		
		IntersectionPoint *samples = (IntersectionPoint *)s->allocate(sizeof(IntersectionPoint) * SAMPLE_COUNT);
		int *intersectionPointer = (int *)s->allocate(sizeof(int) * SAMPLE_COUNT);
		int *sampleTally = (int *)s->allocate(sizeof(int) * intersectionCount);

		// Initialize all intersection points
		for (int i = 0; i < SAMPLE_WIDTH * SAMPLE_HEIGHT; i++) {
			samples[i].m_intersection = false;
			intersectionPointer[i] = 0;
		}

		for (int i = 0; i < intersectionCount; i++) {
			sampleTally[i] = 0;
		}

		// Calculate basis vectors
		math::Vector u = math::constants::YAxis;
		math::Vector v;
		if (abs(math::getX(ray->getDirection())) < (math::real)0.1) {
			u = math::constants::XAxis;
		}
		u = math::normalize(math::cross(u, ray->getDirection()));
		v = math::cross(ray->getDirection(), u);

		u = math::mul(u, math::loadScalar(epsilon));
		v = math::mul(v, math::loadScalar(epsilon));

		math::Vector mainPoint = math::add(math::mul(ray->getDirection(), math::loadScalar(closest->depth)), ray->getSource());

		for (int i = -SAMPLE_RADIUS; i <= SAMPLE_RADIUS; i++) {
			for (int j = -SAMPLE_RADIUS; j <= SAMPLE_RADIUS; j++) {
				int si = (i + SAMPLE_RADIUS) * SAMPLE_WIDTH + j + SAMPLE_RADIUS;
				IntersectionPoint *sample = &samples[si];
				for (int k = 0; k < intersectionCount; k++) {
					CoarseIntersection *c = list->getIntersection(k);
					if ((c->depth - minDepth) < epsilon) {
						IntersectionPoint p;

						// Create a new ray
						math::Vector newTarget = math::add(mainPoint, math::mul(u, math::loadScalar(i)));
						newTarget = math::add(newTarget, math::mul(v, math::loadScalar(j)));

						LightRay newRay;
						newRay.setSource(ray->getSource());
						newRay.setDirection(math::normalize(math::sub(newTarget, ray->getSource())));

						c->sceneObject->getGeometry()->fineIntersection(&newRay, &p, c);

						if (sample->m_intersection == false || p.m_depth < sample->m_depth) {
							*sample = p;
							intersectionPointer[si] = k;
						}
					}
				}
			}
		}

		// Count the intersections
		for (int i = 0; i < SAMPLE_COUNT; i++) {
			sampleTally[intersectionPointer[i]]++;
		}

		int highestSampleCount = 0;
		int preferredIntersection = 0;
		for (int i = 0; i < intersectionCount; i++) {
			if (sampleTally[i] > highestSampleCount) {
				highestSampleCount = sampleTally[i];
				preferredIntersection = i;
			}
		}

		// Calculate the bias normal
		math::Vector biasNormal = math::constants::Zero;
		for (int i = 0; i < intersectionCount; i++) {
			biasNormal = math::add(biasNormal, math::mul(math::loadScalar((math::real)sampleTally[i]), samples[i].m_faceNormal));
		}
		biasNormal = math::normalize(biasNormal);

		list->getIntersection(preferredIntersection)->sceneObject->getGeometry()->fineIntersection(ray, point, list->getIntersection(preferredIntersection));
		*closestObject = list->getIntersection(preferredIntersection)->sceneObject;

		// Bias the position of the intersection point
		point->m_position = math::add(point->m_position, math::mul(math::loadScalar(SURFACE_BIAS), biasNormal));

		// Cleanup the allocated memory
		s->free((void *)sampleTally);
		s->free((void *)intersectionPointer);
		s->free((void *)samples);
	}
}

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *stack) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point, stack);

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
	else {
		// The ray hit nothing so it receives the background color
		ray->setIntensity(m_backgroundColor);
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
