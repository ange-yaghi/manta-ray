#ifndef MANTARAY_STANDARD_CAMERA_RAY_EMITTER_H
#define MANTARAY_STANDARD_CAMERA_RAY_EMITTER_H

#include "camera_ray_emitter.h"

namespace manta {

    class RayContainer;

    class StandardCameraRayEmitter : public CameraRayEmitter {
    public:
        StandardCameraRayEmitter();
        virtual ~StandardCameraRayEmitter();

        virtual void generateRays(RayContainer *rayContainer) const;
    };

} /* namespace manta */

#endif /* MANTARAY_STANDARD_CAMERA_RAY_EMITTER_H */
