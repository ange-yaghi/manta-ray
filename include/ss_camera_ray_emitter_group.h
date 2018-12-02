#ifndef SS_CAMERA_RAY_EMITTER_GROUP_H
#define SS_CAMERA_RAY_EMITTER_GROUP_H

#include <camera_ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class SSCameraRayEmitter;

	class SSCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		SSCameraRayEmitterGroup();
		virtual ~SSCameraRayEmitterGroup();

		virtual void createAllEmitters();

		void setSamplingWidth(int width) { m_samplingWidth = width; }
		int getSamplingWidth() const { return m_samplingWidth; }

		void setSamplesPerPixel(int s) { m_samplesPerPixel = s; }
		int getSamplesPerPixel() const { return m_samplesPerPixel; }

	protected:
		int m_samplingWidth;
		int m_samplesPerPixel;
	};

} /* namespace manta */

#endif /* SS_CAMERA_RAY_EMITTER_GROUP_H */
