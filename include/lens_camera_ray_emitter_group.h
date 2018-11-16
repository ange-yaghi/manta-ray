#ifndef CAMERA_RAY_EMITTER_GROUP_H
#define CAMERA_RAY_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <lens_camera_ray_emitter.h>

namespace manta {

	class LensCameraRayEmitterGroup : public RayEmitterGroup {
	public:
		LensCameraRayEmitterGroup();
		virtual ~LensCameraRayEmitterGroup();

		virtual void createAllEmitters();

		LensCameraRayEmitter **m_cameraRayEmitters;

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

		void setSamplingWidth(int width) { m_samplingWidth = width; }
		int getSamplingWidth() const { return m_samplingWidth; }

		void setSamplesPerPixel(int s) { m_samplesPerPixel = s; }
		int getSamplesPerPixel() const { return m_samplesPerPixel; }

	protected:
		math::Vector m_up;
		math::Vector m_position;
		math::Vector m_direction;

		int m_resolutionX;
		int m_resolutionY;

		int m_samplingWidth;
		int m_samplesPerPixel;

		float m_planeHeight;
		float m_planeDistance;
	};

} /* namespace manta */

#endif /* CAMERA_RAY_EMITTER_GROUP_H */