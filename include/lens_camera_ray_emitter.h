#ifndef MANTARAY_LENS_CAMERA_RAY_EMITTER_H
#define MANTARAY_LENS_CAMERA_RAY_EMITTER_H

#include "camera_ray_emitter.h"

#include "lens.h"

namespace manta {

    // Forward declarations
    class Lens;
    class LightRay;

    class LensCameraRayEmitter : public CameraRayEmitter {
    public:
        LensCameraRayEmitter();
        virtual ~LensCameraRayEmitter();

        virtual void initialize();
        virtual void generateRay(LightRay *ray) const;

        void setLens(const Lens *lens) { m_lens = lens; }
        const Lens *getLens() const { return m_lens; }

    protected:
        const Lens *m_lens;
        Lens::LensScanHint m_lensHint;
    };

} /* namespace manta */

#endif /* MANTARAY_LENS_CAMERA_RAY_EMITTER_H */
