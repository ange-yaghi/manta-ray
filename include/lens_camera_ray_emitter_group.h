#ifndef MANTARAY_LENS_CAMERA_RAY_EMITTER_GROUP_H
#define MANTARAY_LENS_CAMERA_RAY_EMITTER_GROUP_H

#include "../include/camera_ray_emitter_group.h"

namespace manta {

	// Forward declarations
	class Lens;
	class LensCameraRayEmitter;

	class LensCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		LensCameraRayEmitterGroup();
		virtual ~LensCameraRayEmitterGroup();

		virtual void configure();
		virtual CameraRayEmitter *createEmitter(int ix, int iy, 
			StackAllocator *stackAllocator) const;

		void setLens(const Lens *lens) { m_lens = lens; }
		const Lens *getLens() { return m_lens; }

	protected:
		const Lens *m_lens;
	};

} /* namespace manta */

#endif /* MANTARAY_LENS_CAMERA_RAY_EMITTER_GROUP_H */
