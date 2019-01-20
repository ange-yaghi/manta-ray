#ifndef JITTER_CAMERA_RAY_EMITTER_H
#define JITTER_CAMERA_RAY_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class JitterCameraRayEmitter : public RayEmitter {
	public:
		JitterCameraRayEmitter();
		virtual ~JitterCameraRayEmitter();

		virtual void generateRays();

		void setAxisX(const math::Vector &axisX) { m_axisX = axisX; }
		math::Vector getAxisX() const { return m_axisX; }

		void setAxisY(const math::Vector &axisY) { m_axisY = axisY; }
		math::Vector getAxisY() const { return m_axisY; }

		void setStartPosition(const math::Vector &startPosition) { m_startPosition = startPosition; }
		math::Vector getStartPosition() const { return m_startPosition; }

		void setAASamples(int samples) { m_explicitSampleCount = samples; }
		int getAASamples() const { return m_explicitSampleCount; }

		void setWidth(math::real width) { m_width = width; }
		math::real getWidth() const { return m_width; }

		void setHeight(math::real height) { m_height = height; }
		math::real getHeight() const { return m_height; }

		virtual void calculateIntensity();

	protected:
		math::Vector m_axisX;
		math::Vector m_axisY;

		math::real m_width;
		math::real m_height;

		math::Vector m_startPosition;

		int m_explicitSampleCount;
	};

} /* namespace manta */

#endif /* JITTER_CAMERA_RAY_EMITTER_H */
