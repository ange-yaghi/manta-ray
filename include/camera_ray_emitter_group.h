#ifndef CAMERA_RAY_EMITTER_GROUP_H
#define CAMERA_RAY_EMITTER_GROUP_H

#include <manta_math.h>
#include <stack_allocator.h>

namespace manta {

	class ImagePlane;
	class Sampler2D;
	class CameraRayEmitter;
	class RayContainer;

	class CameraRayEmitterGroup {
	public:
		CameraRayEmitterGroup();
		~CameraRayEmitterGroup();

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

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		void setSampler(Sampler2D *sampler) { m_sampler = sampler; }
		Sampler2D *getSampler() const { return m_sampler; }

		virtual void initialize() = 0;

		virtual CameraRayEmitter *createEmitter(int ix, int iy, StackAllocator *stackAllocator) const = 0;
		void freeEmitter(CameraRayEmitter *rayEmitter, StackAllocator *stackAllocator) const;

	protected:
		template<typename t_RayEmitterType>
		t_RayEmitterType *allocateEmitter(StackAllocator *stackAllocator) const {
			t_RayEmitterType *newEmitter = nullptr;
			if (stackAllocator == nullptr) {
				void *buffer = _aligned_malloc(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
			}
			else {
				void *buffer = stackAllocator->allocate(sizeof(t_RayEmitterType), 16);
				newEmitter = new(buffer) t_RayEmitterType;
			}

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
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_GROUP_H */
