#include <camera_ray_emitter_group.h>

#include <scene_buffer.h>
#include <stack_allocator.h>
#include <camera_ray_emitter.h>
#include <ray_container.h>

#include <assert.h>

manta::CameraRayEmitterGroup::CameraRayEmitterGroup() {
	m_currentRayEmitterCount = 0;
	m_rayEmitterCount = 0;
	m_rayEmitters = nullptr;
	m_stackAllocator = nullptr;
	m_rayBuckets = nullptr;
}

manta::CameraRayEmitterGroup::~CameraRayEmitterGroup() {
	assert(m_rayEmitters == nullptr);
}

void manta::CameraRayEmitterGroup::destroyRays() {
	// Destroy the rays in reverse order
	for (int i = getEmitterCount() - 1; i >= 0; i--) {
		m_rayBuckets[i].destroyRays();
	}
}

void manta::CameraRayEmitterGroup::fillSceneBuffer(SceneBuffer *sceneBuffer) const {
	sceneBuffer->initialize(m_resolutionX, m_resolutionY);

	for (int x = 0; x < m_resolutionX; x++) {
		for (int y = 0; y < m_resolutionY; y++) {
			sceneBuffer->set(m_rayBuckets[y * m_resolutionX + x].getIntensity(), x, y);
		}
	}
}

void manta::CameraRayEmitterGroup::destroyEmitters() {
	if (m_rayEmitterCount == 0) return;

	// Make sure to destroy the emitters in reverse
	for (int i = m_rayEmitterCount - 1; i >= 0; i--) {
		if (m_rayEmitters[i] != nullptr) {
			m_rayEmitters[i]->~CameraRayEmitter();

			if (m_stackAllocator == nullptr) {
				_aligned_free((void *)m_rayEmitters[i]);
			}
			else {
				m_stackAllocator->free((void *)m_rayEmitters[i]);
			}
		}
	}

	if (m_stackAllocator == nullptr) {
		delete[] m_rayEmitters;
	}
	else {
		m_stackAllocator->free((void *)m_rayBuckets);
		m_stackAllocator->free((void *)m_rayEmitters);
	}

	m_rayEmitters = nullptr;
	m_rayEmitterCount = 0;
}

void manta::CameraRayEmitterGroup::initializeEmitters(int count) {
	m_rayEmitterCount = count;

	if (count > 0) {
		if (m_stackAllocator == nullptr) {
			m_rayEmitters = new CameraRayEmitter *[count];
			m_rayBuckets = new RayContainer[count];
		}
		else {
			m_rayEmitters = (CameraRayEmitter **)m_stackAllocator->allocate(sizeof(CameraRayEmitter *) * count);
			m_rayBuckets = (RayContainer *)m_stackAllocator->allocate(sizeof(RayContainer) * count);

			for (int i = 0; i < count; i++) {
				// Create the ray container
				new ((void *)&m_rayBuckets[i])RayContainer;
			}
		}
	}
	else {
		m_rayEmitters = nullptr;
		m_rayBuckets = nullptr;
	}
}
