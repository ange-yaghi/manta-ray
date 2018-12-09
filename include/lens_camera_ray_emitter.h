#ifndef LENS_CAMERA_RAY_EMITTER_H
#define LENS_CAMERA_RAY_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class Lens;

	class LensCameraRayEmitter : public RayEmitter {
	public:
		LensCameraRayEmitter();
		virtual ~LensCameraRayEmitter();

		virtual void generateRays();

		virtual void calculateIntensity();

		void setLens(Lens *lens) { m_lens = lens; }
		Lens *getLens() const { return m_lens; }

		void setExplicitSampleCount(int s) { m_explicitSampleCount = s; }
		int getExplicitSampleCount() const { return m_explicitSampleCount; }

	protected:
		int m_explicitSampleCount;
		Lens *m_lens;
	};

} /* namespace manta */

#endif /* LENS_CAMERA_RAY_EMITTER_H */
