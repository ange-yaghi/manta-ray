#ifndef STANDARD_CAMERA_RAY_EMITTER_GROUP_H
#define STANDARD_CAMERA_RAY_EMITTER_GROUP_H

#include <camera_ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class ImagePlane;

	class StandardCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		StandardCameraRayEmitterGroup();
		virtual ~StandardCameraRayEmitterGroup();

		virtual void initialize();
		virtual CameraRayEmitter *createEmitter(int ix, int iy, StackAllocator *stackAllocator) const;

	protected:
		math::real m_planeWidth;
		math::real m_xIncrement;
		math::real m_yIncrement;
		math::Vector m_right;
		math::Vector m_planeCenter;
	};

} /* namespace manta */

#endif /* STANDARD_CAMERA_RAY_EMITTER_GROUP_H */
