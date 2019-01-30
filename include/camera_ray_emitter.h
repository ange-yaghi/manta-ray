#ifndef CAMERA_RAY_EMITTER_H
#define CAMERA_RAY_EMITTER_H

#include <manta_math.h>

namespace manta {

	class Sampler2D;
	class RayContainer;
	class StackAllocator;

	class CameraRayEmitter {
	public:
		CameraRayEmitter();
		virtual ~CameraRayEmitter();

		virtual void generateRays(RayContainer *rayContainer) const = 0;
		void setSampler(Sampler2D *sampler) { m_sampler = sampler; }
		Sampler2D *getSampler() const { return m_sampler; }

		void setSampleCount(int samples) { m_sampleCount = samples; }
		int getSampleCount() const { return m_sampleCount; }

		math::Vector getPosition() const { return m_position; }
		void setPosition(const math::Vector &v) { m_position = v; }

		math::Vector getDirection() const { return m_direction; }
		void setDirection(const math::Vector &v) { m_direction = v; }

		void setStackAllocator(StackAllocator *s) { m_stackAllocator = s; }
		StackAllocator *getStackAllocator() { return m_stackAllocator; }

	protected:
		Sampler2D *m_sampler;

		math::Vector m_position;
		math::Vector m_direction;

		int m_sampleCount;

		StackAllocator *m_stackAllocator;
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_H */
