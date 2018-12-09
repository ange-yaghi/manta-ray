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
#include <camera_ray_emitter_group.h>
#include <path_recorder.h>
#include <simple_scatter_emitter_group.h>
#include <simple_scatter_emitter.h>

#include <iostream>
#include <thread>
#include <chrono>

manta::RayTracer::RayTracer() {
	m_deterministicSeed = false;
	m_pathRecordingOutputDirectory = "";
	m_backgroundColor = math::constants::Zero;
	m_currentRay = 0;
}

manta::RayTracer::~RayTracer() {

}

void manta::RayTracer::traceAll(const Scene *scene, CameraRayEmitterGroup *group) {
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

void manta::RayTracer::tracePixel(int px, int py, const Scene *scene, CameraRayEmitterGroup *group) {
	int pixelIndex = py * group->getResolutionX() + px;

	// Set up the emitter group
	group->setDegree(0);
	group->setStackAllocator(&m_stack);
	group->createAllEmitters();

	Job job;
	job.scene = scene;
	job.group = group;
	job.start = pixelIndex;
	job.end = pixelIndex;

	m_jobQueue.push(job);

	// Create and start all threads
	createWorkers();
	startWorkers();
	waitForWorkers();
}

void manta::RayTracer::traceRayEmitter(const Scene *scene, RayEmitter *emitter, StackAllocator *s /**/ PATH_RECORDER_DECL) const {
	emitter->generateRays();

	LightRay *rays = emitter->getRays();
	int rayCount = emitter->getRayCount();

	for (int i = 0; i < rayCount; i++) {
		LightRay *ray = &rays[i];

		math::Vector average = math::constants::Zero;
		math::Vector samples = math::loadScalar((math::real)emitter->getSamplesPerRay());
		for (int samp = 0; samp < emitter->getSamplesPerRay(); samp++) {
			traceRay(scene, ray, emitter->getDegree(), s /**/ PATH_RECORDER_VAR);
			average = math::add(average, math::div(ray->getIntensity(), samples));
		}
		ray->setIntensity(average);
	}

	emitter->calculateIntensity();
	emitter->destroyRays();
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

void manta::RayTracer::incrementRayCompletion(const Job *job, int increment) {
	m_outputLock.lock();
	int emitterCount = job->group->getEmitterCount();
	m_currentRay += increment;
	if (m_currentRay % 1000 == 0) {
		std::cout << "Ray " << m_currentRay << "/" << emitterCount << std::endl;
	}
	m_outputLock.unlock();
}

void manta::RayTracer::createWorkers() {
	m_workers = new Worker[m_threadCount];

	for (int i = 0; i < m_threadCount; i++) {
		m_workers[i].initialize(m_workerStackSize, this, i, m_deterministicSeed, m_pathRecordingOutputDirectory);
	}
}

void manta::RayTracer::startWorkers() {
	int workerCount = m_threadCount;

	for (int i = 0; i < workerCount; i++) {
		std::cout << "Starting worker " << i << std::endl;
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

	constexpr math::real epsilon = 1E-2;

	IntersectionList list;
	list.setStack(s);

	const CoarseIntersection *referenceIntersection = nullptr;

	// First generate a list of coarse intersections
	for (int i = 0; i < objectCount; i++) {
		SceneObject *object = scene->getSceneObject(i);
		SceneGeometry *geometry = object->getGeometry();

		if (!geometry->fastIntersection(ray)) continue;

		referenceIntersection = geometry->coarseIntersection(ray, &list, object, referenceIntersection, epsilon, s);
	}

	fluxMultisample(ray, &list, point, closestObject, referenceIntersection, epsilon, s);

	list.destroy();
}

void manta::RayTracer::fluxMultisample(const LightRay *ray, IntersectionList *list, IntersectionPoint *point, 
	SceneObject **closestObject, const CoarseIntersection *referenceIntersection, math::real epsilon, StackAllocator *s) const {

	constexpr math::real SURFACE_BIAS = 5E-3;
	
	int conflicts = 0;
	int intersectionCount = list->getIntersectionCount();

	IntersectionPoint *fineIntersections = nullptr;
	
	if (intersectionCount > 0) {
		fineIntersections = (IntersectionPoint *)s->allocate(sizeof(IntersectionPoint) * intersectionCount);
	}

	if (referenceIntersection != nullptr) {
		for (int i = 0; i < intersectionCount; i++) {
			CoarseIntersection *c = list->getIntersection(i);
			if (abs(c->depth - referenceIntersection->depth) < epsilon) {
				conflicts++;
			}
		}
	}
	else {
		conflicts = 0;
	}

	if (conflicts == 0) {
		point->m_intersection = false;
		*closestObject = nullptr;
	}
	else if (conflicts == 1) {
		// There can only be one answer
		referenceIntersection->sceneObject->getGeometry()->fineIntersection(ray, point, referenceIntersection, 1E-6);

		if (point->m_intersection) {
			*closestObject = referenceIntersection->sceneObject;

			// Bias the intersection position
			point->m_position = math::add(point->m_position, math::mul(math::loadScalar(SURFACE_BIAS), point->m_faceNormal));
			//point->m_position = math::add(point->m_position, math::mul(math::loadScalar(-SURFACE_BIAS), ray->getDirection()));
		}
		else {
			*closestObject = nullptr;
		}
	}
	else {
		// There are multiple conflicts that need to be resolved
		constexpr int SAMPLE_RADIUS = 5;
		constexpr int SAMPLE_WIDTH = SAMPLE_RADIUS * 2 + 1;
		constexpr int SAMPLE_HEIGHT = SAMPLE_RADIUS * 2 + 1;
		constexpr int SAMPLE_COUNT = SAMPLE_WIDTH * SAMPLE_HEIGHT;
		
		IntersectionPoint *samples = (IntersectionPoint *)s->allocate(sizeof(IntersectionPoint) * SAMPLE_COUNT);
		int *intersectionPointer = (int *)s->allocate(sizeof(int) * SAMPLE_COUNT);
		int *sampleTally = (int *)s->allocate(sizeof(int) * intersectionCount);
		math::real *averageDistance = (math::real *)s->allocate(sizeof(math::real) * intersectionCount);

		// Initialize all intersection points
		for (int i = 0; i < SAMPLE_WIDTH * SAMPLE_HEIGHT; i++) {
			samples[i].m_intersection = false;
			intersectionPointer[i] = -1;
		}

		for (int i = 0; i < intersectionCount; i++) {
			sampleTally[i] = 0;
			averageDistance[i] = 0.0;
		}

		// Calculate basis vectors
		math::Vector u = math::constants::YAxis;
		math::Vector v;
		if (abs(math::getX(ray->getDirection())) < (math::real)0.1) {
			u = math::constants::XAxis;
		}
		u = math::normalize(math::cross(u, ray->getDirection()));
		v = math::cross(u, ray->getDirection());

		u = math::mul(u, math::loadScalar(1E-4));
		v = math::mul(v, math::loadScalar(1E-4));

		math::Vector mainPoint = math::add(math::mul(ray->getDirection(), math::loadScalar(referenceIntersection->depth)), ray->getSource());
		math::Vector referenceNormal;

		for (int i = -SAMPLE_RADIUS; i <= SAMPLE_RADIUS; i++) {
			for (int j = -SAMPLE_RADIUS; j <= SAMPLE_RADIUS; j++) {
				int si = (i + SAMPLE_RADIUS) * SAMPLE_WIDTH + j + SAMPLE_RADIUS;
				IntersectionPoint *sample = &samples[si];

				// Create a new ray
				math::Vector newTarget = math::add(mainPoint, math::mul(v, math::loadScalar(i)));
				newTarget = math::add(newTarget, math::mul(u, math::loadScalar(j)));

				LightRay newRay;
				newRay.setSource(ray->getSource());
				newRay.setDirection(math::normalize(math::sub(newTarget, ray->getSource())));

				for (int k = 0; k < intersectionCount; k++) {
					CoarseIntersection *c = list->getIntersection(k);
					if (abs(c->depth - referenceIntersection->depth) < epsilon) {
						IntersectionPoint p;
						c->sceneObject->getGeometry()->fineIntersection(&newRay, &p, c, (math::real)0.0);

						if (p.m_intersection) {
							if (!sample->m_intersection || p.m_depth < sample->m_depth) {
								*sample = p;
								intersectionPointer[si] = k;
								referenceNormal = sample->m_faceNormal;
							}
						}
					}
				}
			}
		}

		bool flipScenario = false;

		for (int i = 0; i < SAMPLE_COUNT; i++) {
			if (intersectionPointer[i] != -1) {
				if (math::getScalar(math::dot(samples[i].m_faceNormal, referenceNormal)) < 0.0) {
					flipScenario = true;
					break;
				}
			}
		}

		// Count the intersections
		for (int i = 0; i < SAMPLE_COUNT; i++) {
			if (intersectionPointer[i] != -1) {
				sampleTally[intersectionPointer[i]]++;
				averageDistance[intersectionPointer[i]] += samples[i].m_depth;
			}
		}

		for (int i = 0; i < intersectionCount; i++) {
			if (sampleTally[i] > 0) {
				averageDistance[i] /= sampleTally[i];
			}
			else {
				averageDistance[i] = (math::real)0.0;
			}
		}
		

		math::real lowestAverageDistance = math::constants::REAL_MAX;
		int highestSampleCount = 0;
		int closestIntersection = -1;
		int preferredIntersection = -1;
		for (int i = 0; i < intersectionCount; i++) {
			if (averageDistance[i] < lowestAverageDistance && sampleTally[i] > 0) {
				lowestAverageDistance = averageDistance[i];
				closestIntersection = i;
			}
			if (sampleTally[i] > highestSampleCount) {
				highestSampleCount = sampleTally[i];
				preferredIntersection = i;
			}
		}

		bool repairNormals = false;

		if (flipScenario) {
			if (preferredIntersection != closestIntersection) {
				math::real realDelta = abs(list->getIntersection(preferredIntersection)->depth - list->getIntersection(closestIntersection)->depth);
				math::real sampleDelta = abs(averageDistance[preferredIntersection] - averageDistance[closestIntersection]);
				if (sampleDelta > realDelta) {
					// Only switch if there is a high confidence in the decision
					preferredIntersection = closestIntersection;
					repairNormals = true;
				}
			}
		}

		math::Vector biasNormal = math::constants::Zero;
		math::Vector vertexNormal = math::constants::Zero;
		//if (!flipScenario) {
			// Calculate the bias normal
			for (int i = 0; i < SAMPLE_COUNT; i++) {
				if (intersectionPointer[i] != -1) {
					biasNormal = math::add(biasNormal, samples[i].m_faceNormal);
					vertexNormal = math::add(vertexNormal, samples[i].m_vertexNormal);
				}
			}
			biasNormal = math::normalize(biasNormal);
			vertexNormal = math::normalize(vertexNormal);
		//}

		list->getIntersection(preferredIntersection)->sceneObject->getGeometry()->fineIntersection(ray, point, list->getIntersection(preferredIntersection), 1000);

		if (point->m_intersection) {
			*closestObject = list->getIntersection(preferredIntersection)->sceneObject;

			//if (!flipScenario) {
				point->m_faceNormal = biasNormal;
				point->m_vertexNormal = vertexNormal;
			//}

			// Bias the position of the intersection point
			point->m_position = math::add(point->m_position, math::mul(math::loadScalar(SURFACE_BIAS), point->m_faceNormal));
			//point->m_position = math::add(point->m_position, math::mul(math::loadScalar(-SURFACE_BIAS), ray->getDirection()));

			//point->m_vertexNormal = math::loadVector(-1.0, -1.0, -1.0);
		} 
		else {
			*closestObject = nullptr;
		}

		// Cleanup the allocated memory
		s->free((void *)averageDistance);
		s->free((void *)sampleTally);
		s->free((void *)intersectionPointer);
		s->free((void *)samples);
	}

	if (fineIntersections != nullptr) {
		s->free((void *)fineIntersections);
	}
}

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s /**/ PATH_RECORDER_DECL) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point, s);

	math::real scatter = math::uniformRandom(1.0);
	constexpr bool causticScatter = false;
	if (causticScatter && scatter > 0.5) {
		// Do a scatter for caustic simulation
		void *buffer = s->allocate(sizeof(SimpleScatterEmitterGroup), 16);
		SimpleScatterEmitterGroup *group = new (buffer) SimpleScatterEmitterGroup;

		group->setStackAllocator(s);
		group->setDegree(degree + 1);
		group->createAllEmitters();

		constexpr math::real epsilon = 1E-3;
		math::real depth; 

		if (sceneObject != nullptr) {
			depth = math::uniformRandom(point.m_depth - epsilon) + epsilon;
		}
		else {
			// TODO: set more explicit depth
			depth = math::uniformRandom((math::real)10.0);
		}

		math::Vector pos = math::add(ray->getSource(), math::mul(ray->getDirection(), math::loadScalar(depth)));
		group->m_simpleRayEmitter->setPosition(pos);
		group->m_simpleRayEmitter->setDirection(ray->getDirection());
		group->m_simpleRayEmitter->setSamplesPerRay(1);

		START_BRANCH(pos);
		traceRayEmitterGroup(scene, group, s /**/ PATH_RECORDER_VAR);
		END_BRANCH();

		// Simple ray integration
		ray->setIntensity(math::mul(group->m_simpleRayEmitter->getIntensity(), math::loadScalar(1.0)));

		// Data must be freed in reverse order
		int emitterCount = group->getEmitterCount();
		for (int i = emitterCount - 1; i >= 0; i--) {
			group->getEmitters()[i]->destroyRays();
		}

		group->destroyEmitters();
		group->~SimpleScatterEmitterGroup();
		s->free((void *)group);
	}
	else {
		if (sceneObject != nullptr) {
			START_BRANCH(point.m_position); // For path recording
			Material *material = sceneObject->getMaterial();
			RayEmitterGroup *group = material->generateRayEmitterGroup(ray, &point, degree + 1, s);

			if (group != nullptr) {
				traceRayEmitterGroup(scene, group, s /**/ PATH_RECORDER_VAR);
			}

			material->integrateRay(ray, group);

			//if (group != nullptr) {
			//	int emitterCount = group->getEmitterCount();
			//
			//	// Data must be freed in reverse order
			//	for (int i = emitterCount - 1; i >= 0; i--) {
			//		group->getEmitters()[i]->destroyRays();
			//	}
			//
			//	material->destroyEmitterGroup(group, s);
			//}
			END_BRANCH();
		}
		else {
			// The ray hit nothing so it receives the background color
			ray->setIntensity(m_backgroundColor);

			// Point a ray in the air
			START_BRANCH(math::add(ray->getSource(), math::mul(ray->getDirection(), math::loadScalar((math::real)1000.0))));
			END_BRANCH();
		}
	}
}

void manta::RayTracer::traceRayEmitterGroup(const Scene *scene, const RayEmitterGroup *rayEmitterGroup, StackAllocator *s /**/ PATH_RECORDER_DECL) const {
	int emitterCount = rayEmitterGroup->getEmitterCount();
	RayEmitter **emitters = rayEmitterGroup->getEmitters();

	for (int i = 0; i < emitterCount; i++) {
		RayEmitter *emitter = emitters[i];
		emitter->setStackAllocator(s);
		traceRayEmitter(scene, emitter, s /**/ PATH_RECORDER_VAR);
	}
}
