#ifndef CAMERA_RAY_EMITTER_H
#define CAMERA_RAY_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class Sampler2D;

	class CameraRayEmitter : public RayEmitter {
	public:
		CameraRayEmitter();
		virtual ~CameraRayEmitter();

		virtual void generateRays() = 0;
		void setSampler(Sampler2D *sampler) { m_sampler = sampler; }
		Sampler2D *getSampler() const { return m_sampler; }

		virtual void calculateIntensity();

		void setSampleCount(int samples) { m_sampleCount = samples; }
		int getSampleCount() const { return m_sampleCount; }

	protected:
		Sampler2D *m_sampler;

		int m_sampleCount;
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_H */
