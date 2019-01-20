#ifndef STANDARD_CAMERA_RAY_EMITTER_GROUP_H
#define STANDARD_CAMERA_RAY_EMITTER_GROUP_H

#include <camera_ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class SceneBuffer;

	class StandardCameraRayEmitterGroup : public CameraRayEmitterGroup {
	public:
		StandardCameraRayEmitterGroup();
		virtual ~StandardCameraRayEmitterGroup();

		virtual void createAllEmitters();
	};

} /* namespace manta */

#endif /* STANDARD_CAMERA_RAY_EMITTER_GROUP_H */
