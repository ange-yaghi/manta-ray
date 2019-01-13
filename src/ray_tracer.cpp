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
#include <standard_allocator.h>
#include <stack_list.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

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
	std::cout <<		"================================================" << std::endl;
	std::cout <<		"Total processing time:               " << diff.count() << " s" << std::endl;
	std::cout <<		"------------------------------------------------" << std::endl;
	std::cout <<		"Standard allocator peak usage:       " << StandardAllocator::Global()->getMaxUsage() / (double)MB << " MB" << std::endl;
	std::cout <<		"Main allocator peak usage:           " << m_stack.getMaxUsage() / (double)MB << " MB" << std::endl;
	unsigned __int64 totalUsage = m_stack.getMaxUsage() + StandardAllocator::Global()->getMaxUsage();
	for (int i = 0; i < m_threadCount; i++) {
		std::stringstream ss;
		ss <<			"Worker " << i << " peak memory usage:";
		std::cout << ss.str();
		for (int j = 0; j < (37 - ss.str().length()); j++) {
			std::cout << " ";
		}
		std::cout << m_workers[i].getMaxMemoryUsage() / (double)MB << " MB" << std::endl;
		totalUsage += m_workers[i].getMaxMemoryUsage();
	}
	std::cout <<		"                                     -----------" << std::endl;
	std::cout <<		"Total memory usage:                  " << totalUsage / (double)MB << " MB" << std::endl;
	std::cout <<		"================================================" << std::endl;
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

std::string manta::RayTracer::getTreeName(int pixelIndex, int sample) const {
	std::stringstream ss;
	ss << "PATH_" << pixelIndex << "_S" << sample;

	return ss.str();
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
			if (emitter->getSamplesPerRay() > 1) NEW_TREE(getTreeName(i, samp), emitter->getPosition());
			traceRay(scene, ray, emitter->getDegree(), s /**/ PATH_RECORDER_VAR);
			average = math::add(average, math::div(ray->getIntensity(), samples));
			if (emitter->getSamplesPerRay() > 1) END_TREE();
		}
		ray->setIntensity(average);
	}

	emitter->calculateIntensity();
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

	IntersectionList list;
	list.setStack(s);

	CoarseIntersection closestIntersection;
	math::real closestDepth = math::constants::REAL_MAX;

	// Find the closest intersection
	bool found = false;
	for (int i = 0; i < objectCount; i++) {
		SceneObject *object = scene->getSceneObject(i);
		SceneGeometry *geometry = object->getGeometry();

		if (!geometry->fastIntersection(ray)) continue;

		if (geometry->findClosestIntersection(ray, &closestIntersection, (math::real)0.0, closestDepth, s)) {
			found = true;
			closestIntersection.sceneObject = object;
			closestDepth = closestIntersection.depth;
		}
	}

	if (found) {
		math::Vector position = math::add(ray->getSource(), math::mul(ray->getDirection(), math::loadScalar(closestIntersection.depth)));
		closestIntersection.sceneGeometry->fineIntersection(position, point, &closestIntersection);
		point->m_valid = true;
		*closestObject = closestIntersection.sceneObject;

		for (int i = 0; i < objectCount; i++) {
			SceneObject *object = scene->getSceneObject(i);
			SceneGeometry *geometry = object->getGeometry();

			//geometry->getVicinity(position, (math::real)1E-2, &list, object);
		}

		refineContact(ray, closestIntersection.depth, &list, point, closestObject, s);

		//if (conflicts < 2) {
		//	point->m_valid = false;
		//	*closestObject = nullptr;
		//}
	}
	else {
		point->m_valid = false;
		*closestObject = nullptr;
	}

	list.destroy();
}

void manta::RayTracer::refineContact(const LightRay *ray, math::real depth, IntersectionList *list, IntersectionPoint *point, SceneObject **closestObject, StackAllocator *s) const {
	/*int conflicts = list->getIntersectionCount();
	int contactCount = list->getIntersectionCount();
	for (int i = 0; i < contactCount; i++) {
		CoarseIntersection *ci = list->getIntersection(i);
		if (ci->valid) {
			for (int j = i + 1; j < contactCount; j++) {
				CoarseIntersection *cj = list->getIntersection(j);
				if (cj->valid) {
					if (ci->globalHint == cj->globalHint && ci->sceneObject == cj->sceneObject) {
						cj->valid = false;
						conflicts--;
					}
				}
			}
		}
	}*/

	// Determine which contact is the correct one
	constexpr int samples = 10;
	constexpr math::real photonRadius = (math::real)1E-3;
	math::real distance = (math::real)1E-2;
	math::real step = distance / 10;

	CoarseIntersection *f = nullptr;

	/*
	for (int i = 0; i < samples; i++) {
		math::Vector pos = math::sub(point->m_position, math::mul(math::loadScalar(step * i), ray->getDirection()));

		math::real closest = math::constants::REAL_MAX;
		CoarseIntersection *newInt = nullptr;

		for (int c = 0; c < contactCount; c++) {
			CoarseIntersection *ci = list->getIntersection(c);
			if (ci->valid) {
				math::Vector closestPoint = ci->sceneGeometry->getClosestPoint(ci, pos);
				math::Vector d = math::sub(closestPoint, pos);
				math::real dist2 = math::getScalar(math::magnitudeSquared3(d));
				if (dist2 < distance * distance) {
					math::real dot = math::getScalar(math::dot(math::normalize(d), ray->getDirection()));
					if (dot >= 1E-4) {
						if (dist2 < closest) {
							newInt = ci;
						}
					}
					if (dist2 < closest) {
						closest = dist2;
					}
				}
			}
		}

		if (newInt != nullptr) {
			f = newInt;
		}
	}*/

	if (f != nullptr) f->sceneGeometry->fineIntersection(point->m_position, point, f);

	// Simple bias
	math::real d = depth;
	d -= 5E-3;
	if (d < (math::real)0.0) {
		d = (math::real)0.0;
	}
	math::Vector dist = math::loadScalar(d);
	point->m_position = math::add(ray->getSource(), math::mul(ray->getDirection(), dist));

	if (math::getScalar(math::dot(point->m_faceNormal, ray->getDirection())) > 0.0) {
		point->m_faceNormal = math::negate(point->m_faceNormal);
		point->m_vertexNormal = math::negate(point->m_vertexNormal);
	}
}

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s /**/ PATH_RECORDER_DECL) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point, s);

	math::real scatterDepth = (point.m_intersection) ? point.m_depth : 11.0;
	if (scatterDepth > 11.0) scatterDepth = 11.0;
	math::real scatterPDF = 0.03 * scatterDepth; // 10% chance per 10 units travelled
	math::real scatter = math::uniformRandom(1.0);
	constexpr bool causticScatter = false;
	if (causticScatter && scatter < scatterPDF && degree < 5) {
		// Do a scatter for caustic simulation
		void *buffer = s->allocate(sizeof(SimpleScatterEmitterGroup), 16);
		SimpleScatterEmitterGroup *group = new (buffer) SimpleScatterEmitterGroup;

		group->setStackAllocator(s);
		group->setDegree(degree + 1);
		group->createAllEmitters();

		constexpr math::real epsilon = 1E-3;
		math::real depth; 
		depth = math::uniformRandom(scatterDepth - epsilon) + epsilon;

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
			Material *material;
			if (point.m_material == -1) {
				material = sceneObject->getDefaultMaterial();
			}
			else {
				material = m_materialManager.getMaterial(point.m_material);
			}
			RayEmitterGroup *group = nullptr;
			if (point.m_valid) group = material->generateRayEmitterGroup(ray, &point, degree + 1, s);

			if (group != nullptr) {
				traceRayEmitterGroup(scene, group, s /**/ PATH_RECORDER_VAR);
			}

			material->integrateRay(ray, group, &point);

			if (group != nullptr) {
				// Destroy rays
				int emitterCount = group->getEmitterCount();
				for (int i = 0; i < emitterCount; i++) {
					group->getEmitter(i)->destroyRays();
				}

				material->destroyEmitterGroup(group, s);
			}

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
