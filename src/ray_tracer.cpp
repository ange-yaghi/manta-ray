#include <ray_tracer.h>

#include <material.h>
#include <light_ray.h>
#include <camera_ray_emitter.h>
#include <camera_ray_emitter_group.h>
#include <scene.h>
#include <scene_object.h>
#include <intersection_point.h>
#include <scene_geometry.h>
#include <stack_allocator.h>
#include <memory_management.h>
#include <worker.h>
#include <camera_ray_emitter_group.h>
#include <path_recorder.h>
#include <standard_allocator.h>
#include <stack_list.h>
#include <ray_container.h>
#include <coarse_intersection.h>

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

#if ENABLE_DETAILED_STATISTICS
	RuntimeStatistics combinedStatistics;
	combinedStatistics.reset();

	// Add all statistics from workers
	for (int i = 0; i < m_threadCount; i++) {
		combinedStatistics.add(m_workers[i].getStatistics());
	}

	// Display statistics
	std::cout <<		"================================================" << std::endl;
	std::cout <<		"Run-time Statistics" << std::endl;
	std::cout <<		"------------------------------------------------" << std::endl;
	int counterCount = RuntimeStatistics::COUNTER_COUNT;
	for (int i = 0; i < counterCount; i++) {
		unsigned __int64 counter = combinedStatistics.counters[i];
		const char *counterName = combinedStatistics.getCounterName((RuntimeStatistics::COUNTER)i);
		std::stringstream ss;
		ss << counterName << ":";
		std::cout << ss.str();
		for (int j = 0; j < (37 - ss.str().length()); j++) {
			std::cout << " ";
		}

		std::cout << counter << std::endl;
	}
#endif /* ENABLE_DETAILED_STATISTICS */

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

void manta::RayTracer::traceRayEmitter(const CameraRayEmitter *emitter, RayContainer *container, const Scene *scene, StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const {
	// Get the target ray container

	emitter->generateRays(container);

	LightRay *rays = container->getRays();
	int rayCount = container->getRayCount();

	for (int i = 0; i < rayCount; i++) {
		LightRay *ray = &rays[i];
		ray->calculateTransformations();

		math::Vector intensity = math::constants::Zero;

		traceRay(scene, ray, 0, s /**/ PATH_RECORDER_VAR /**/ STATISTICS_PARAM_INPUT);
		intensity = ray->getWeightedIntensity();

		ray->setIntensity(intensity);
	}

	container->calculateIntensity();
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

void manta::RayTracer::depthCull(const Scene *scene, const LightRay *ray, SceneObject **closestObject, IntersectionPoint *point, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const {
	int objectCount = scene->getSceneObjectCount();

	CoarseIntersection closestIntersection;
	math::real closestDepth = math::constants::REAL_MAX;

	// Find the closest intersection
	bool found = false;
	for (int i = 0; i < objectCount; i++) {
		SceneObject *object = scene->getSceneObject(i);
		SceneGeometry *geometry = object->getGeometry();

		if (!geometry->fastIntersection(ray)) continue;

		if (geometry->findClosestIntersection(ray, &closestIntersection, (math::real)0.0, closestDepth, s /**/ STATISTICS_PARAM_INPUT)) {
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

		refineContact(ray, closestIntersection.depth, point, closestObject, s);
	}
	else {
		point->m_valid = false;
		*closestObject = nullptr;
	}
}

void manta::RayTracer::refineContact(const LightRay *ray, math::real depth, IntersectionPoint *point, SceneObject **closestObject, StackAllocator *s) const {
	// Simple bias
	math::real d = depth;
	d -= (math::real)5E-3;
	if (d < (math::real)0.0) {
		d = (math::real)0.0;
	}
	math::Vector dist = math::loadScalar(d);
	point->m_position = math::add(ray->getSource(), math::mul(ray->getDirection(), dist));

	if (math::getScalar(math::dot(point->m_faceNormal, ray->getDirection())) > 0.0) {
		point->m_faceNormal = math::negate(point->m_faceNormal);
		point->m_vertexNormal = math::negate(point->m_vertexNormal);
		point->m_direction = MediaInterface::DIRECTION_OUT;
	}
	else {
		point->m_direction = MediaInterface::DIRECTION_IN;
	}
}

void manta::RayTracer::traceRay(const Scene *scene, LightRay *ray, int degree, StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const {
	SceneObject *sceneObject = nullptr;
	IntersectionPoint point;

	depthCull(scene, ray, &sceneObject, &point, s /**/ STATISTICS_PARAM_INPUT);

	if (sceneObject != nullptr) {
		START_BRANCH(point.m_position); // For path recording
		Material *material;
		if (point.m_material == -1) {
			material = sceneObject->getDefaultMaterial();
		}
		else {
			material = m_materialManager.getMaterial(point.m_material);
		}
		
		// Create a new container
		RayContainer container;
		container.setStackAllocator(s);
		container.setDegree(degree + 1);
		if (point.m_valid) {
			material->generateRays(&container, *ray, point, degree + 1, s);
		}

		traceRays(scene, container, s /**/ PATH_RECORDER_VAR /**/ STATISTICS_PARAM_INPUT);
		material->integrateRay(ray, container, point);
		container.destroyRays();

		END_BRANCH();
	}
	else {
		// The ray hit nothing so it receives the background color
		ray->setIntensity(m_backgroundColor);

		// Point a ray at nothing
		START_BRANCH(math::add(ray->getSource(), math::mul(ray->getDirection(), math::loadScalar((math::real)1.0))));
		END_BRANCH();
	}
}

void manta::RayTracer::traceRays(const Scene *scene, const RayContainer &rayContainer, StackAllocator *s /**/ PATH_RECORDER_DECL /**/ STATISTICS_PROTOTYPE) const {
	int nRays = rayContainer.getRayCount();

	for (int i = 0; i < nRays; i++) {
		INCREMENT_COUNTER(RuntimeStatistics::RAYS_CAST);

		LightRay *ray = &rayContainer.getRays()[i];
		ray->calculateTransformations();

		traceRay(scene, ray, rayContainer.getDegree(), s /**/ PATH_RECORDER_VAR /**/ STATISTICS_PARAM_INPUT);
	}
}
