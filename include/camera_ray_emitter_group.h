#ifndef CAMERA_RAY_EMITTER_GROUP_H
#define CAMERA_RAY_EMITTER_GROUP_H

#include <manta_math.h>
#include <stack_allocator.h>

namespace manta {

	class SceneBuffer;
	class Sampler2D;
	class CameraRayEmitter;
	class RayContainer;

	class CameraRayEmitterGroup {
	public:
		CameraRayEmitterGroup();
		~CameraRayEmitterGroup();

		virtual void createAllEmitters() = 0;
		void destroyRays(); // Manually destroy all rays

		void setUp(const math::Vector &up) { m_up = up; }
		math::Vector getUp() const { return m_up; }

		void setResolutionX(int resolution) { m_resolutionX = resolution; }
		int getResolutionX() const { return m_resolutionX; }

		void setResolutionY(int resolution) { m_resolutionY = resolution; }
		int getResolutionY() const { return m_resolutionY; }

		void setPlaneHeight(float planeHeight) { m_planeHeight = planeHeight; }
		float getPlaneHeight() const { return m_planeHeight; }

		void setPlaneDistance(float planeDistance) { m_planeDistance = planeDistance; }
		float getPlaneDistance() const { return m_planeDistance; }

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

		void setDirection(const math::Vector &direction) { m_direction = direction; }
		math::Vector getDirection() const { return m_direction; }

		virtual void fillSceneBuffer(SceneBuffer *sceneBuffer) const;

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		void setSampler(Sampler2D *sampler) { m_sampler = sampler; }
		Sampler2D *getSampler() const { return m_sampler; }

		void initializeEmitters(int count);
		void destroyEmitters();

		int getEmitterCount() const { return m_rayEmitterCount; }
		CameraRayEmitter **getEmitters() const { return m_rayEmitters; }
		CameraRayEmitter *getEmitter(int index = 0) const { return m_rayEmitters[index]; }
		RayContainer *getBuckets() const { return m_rayBuckets; }

		void setStackAllocator(StackAllocator *allocator) { m_stackAllocator = allocator; }
		StackAllocator *getStackAllocator() const { return m_stackAllocator; }

		void setMeta(int meta) { m_meta = meta; }
		int getMeta() const { return m_meta; }

	protected:
		template<typename t_RayEmitterType>
		t_RayEmitterType *createEmitter() {
			t_RayEmitterType *newEmitter = nullptr;
			if (m_stackAllocator == nullptr) {
				void *buffer = _aligned_malloc(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
			}
			else {
				void *buffer = m_stackAllocator->allocate(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
			}
			m_rayEmitters[m_currentRayEmitterCount] = newEmitter;
			m_currentRayEmitterCount++;

			return newEmitter;
		}

	protected:
		math::Vector m_up;
		math::Vector m_position;
		math::Vector m_direction;

		int m_resolutionX;
		int m_resolutionY;

		float m_planeHeight;
		float m_planeDistance;

		int m_samples;
		Sampler2D *m_sampler;

	protected:
		CameraRayEmitter **m_rayEmitters;
		RayContainer *m_rayBuckets;
		int m_rayEmitterCount;
		int m_currentRayEmitterCount;

		int m_meta;

	private:
		StackAllocator *m_stackAllocator;
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_GROUP_H */
