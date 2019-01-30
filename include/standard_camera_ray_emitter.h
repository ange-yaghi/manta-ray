#ifndef STANDARD_CAMERA_RAY_EMITTER_H
#define STANDARD_CAMERA_RAY_EMITTER_H

#include <camera_ray_emitter.h>

namespace manta {

	class RayContainer;

	class StandardCameraRayEmitter : public CameraRayEmitter {
	public:
		StandardCameraRayEmitter();
		virtual ~StandardCameraRayEmitter();

		virtual void generateRays(RayContainer *rayContainer) const;

		void setTargetOrigin(const math::Vector &origin) { m_targetOrigin = origin; }
		math::Vector getTargetOrigin() const { return m_targetOrigin; }

	protected:
		math::Vector m_targetOrigin;
	};

} /* namespace manta */

#endif /* STANDARD_CAMERA_RAY_EMITTER_H */
