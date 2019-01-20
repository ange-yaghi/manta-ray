#ifndef JITTER_CAMERA_RAY_EMITTER_GROUP_H
#define JITTER_CAMERA_RAY_EMITTER_GROUP_H

#include <camera_ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class JitterCameraRayEmitter;

	class JitterCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		JitterCameraRayEmitterGroup();
		virtual ~JitterCameraRayEmitterGroup();

		virtual void createAllEmitters();

		void setSamplesPerPixel(int s) { m_samplesPerPixel = s; }
		int getSamplesPerPixel() const { return m_samplesPerPixel; }

	protected:
		int m_samplesPerPixel;
	};

} /* namespace manta */

#endif /* JITTER_CAMERA_RAY_EMITTER_GROUP_H */
