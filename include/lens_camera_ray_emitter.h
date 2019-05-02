#ifndef LENS_CAMERA_RAY_EMITTER_H
#define LENS_CAMERA_RAY_EMITTER_H

#include <camera_ray_emitter.h>

namespace manta {

	// Forward declarations
	class Lens;
	class RayContainer;

	class LensCameraRayEmitter : public CameraRayEmitter {
	public:
		LensCameraRayEmitter();
		virtual ~LensCameraRayEmitter();

		virtual void generateRays(RayContainer *rayContainer) const;

		void setLens(const Lens *lens) { m_lens = lens; }
		const Lens *getLens() const { return m_lens; }

	protected:
		const Lens *m_lens;
	};

} /* namespace manta */

#endif /* LENS_CAMERA_RAY_EMITTER_H */
