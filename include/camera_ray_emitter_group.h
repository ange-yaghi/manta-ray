#ifndef CAMERA_RAY_EMITTER_GROUP_H
#define CAMERA_RAY_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class SceneBuffer;

	class CameraRayEmitterGroup : public RayEmitterGroup {
	public:
		CameraRayEmitterGroup();
		virtual ~CameraRayEmitterGroup();

		virtual void createAllEmitters() = 0;
		void destroyRays(); // Manually destroy all rays

		void setUp(const math::Vector &up) { m_up = up; }
		math::Vector getUp() const { return m_up; }

		void setResolutionX(int resolution) { m_resolutionX = resolution; }
		int getResolutionX() const { return m_resolutionX; }

		void setResolutionY(int resolution) { m_resolutionY = resolution; }
		int getResolutionY() const { return m_resolutionY; }

		void setPlaneHeight(float planeHeight) { m_planeHeight = planeHeight; }
		float getPlaneHeight() const { return m_planeHeight; }

		void setPlaneDistance(float planeDistance) { m_planeDistance = planeDistance; }
		float getPlaneDistance() const { return m_planeDistance; }

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

		void setDirection(const math::Vector &direction) { m_direction = direction; }
		math::Vector getDirection() const { return m_direction; }

		virtual void fillSceneBuffer(SceneBuffer *sceneBuffer) const;

	protected:
		math::Vector m_up;
		math::Vector m_position;
		math::Vector m_direction;

		int m_resolutionX;
		int m_resolutionY;

		float m_planeHeight;
		float m_planeDistance;
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_GROUP_H */
