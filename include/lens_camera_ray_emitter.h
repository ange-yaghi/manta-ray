#ifndef LENS_CAMERA_RAY_EMITTER_H
#define LENS_CAMERA_RAY_EMITTER_H

#include <camera_ray_emitter.h>

namespace manta {

	class Lens;
	class RayContainer;

	class LensCameraRayEmitter : public CameraRayEmitter {
	public:
		LensCameraRayEmitter();
		virtual ~LensCameraRayEmitter();

		virtual void generateRays(RayContainer *rayContainer) const;

		void setLens(Lens *lens) { m_lens = lens; }
		Lens *getLens() const { return m_lens; }

	protected:
		Lens *m_lens;
	};

} /* namespace manta */

#endif /* LENS_CAMERA_RAY_EMITTER_H */
