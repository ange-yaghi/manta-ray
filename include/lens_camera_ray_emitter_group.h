#ifndef LENS_CAMERA_RAY_EMITTER_GROUP_H
#define LENS_CAMERA_RAY_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <camera_ray_emitter_group.h>

namespace manta {

	class Lens;
	class LensCameraRayEmitter;

	class LensCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		LensCameraRayEmitterGroup();
		virtual ~LensCameraRayEmitterGroup();

		virtual void createAllEmitters();

		LensCameraRayEmitter **m_cameraRayEmitters;

		void setSamplesPerPixel(int s) { m_samplesPerPixel = s; }
		int getSamplesPerPixel() const { return m_samplesPerPixel; }

		void setLens(Lens *lens) { m_lens = lens; }
		Lens *getLens() { return m_lens; }

		void setExplicitSampleCount(int s) { m_explicitSampleCount = s; }
		int getExplicitSampleCount() const { return m_explicitSampleCount; }

	protected:
		int m_samplesPerPixel;
		int m_explicitSampleCount;

		Lens *m_lens;
	};

} /* namespace manta */

#endif /* LENS_CAMERA_RAY_EMITTER_GROUP_H */