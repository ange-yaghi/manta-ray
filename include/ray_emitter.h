#ifndef RAY_EMITTER_H
#define RAY_EMITTER_H

#include <manta_math.h>

namespace manta {

	class LightRay;

	class RayEmitter {
	public:
		RayEmitter();
		virtual ~RayEmitter();

		LightRay *getRays() const { return m_rays; }
		int getRayCount() const { return m_rayCount; }

		virtual void generateRays() = 0;

		math::Vector getPosition() const { return m_position; }
		void setPosition(const math::Vector &v) { m_position = v; }

		math::Vector getDirection() const { return m_direction; }
		void setDirection(const math::Vector &v) { m_direction = v; }

		int getDegree() const { return m_degree; }
		void setDegree(int degree) { m_degree = degree; }

		int getSamplesPerRay() const { return m_samplesPerRay; }
		void setSamplesPerRay(int samples) { m_samplesPerRay = samples; }

	protected:
		void initializeRays(int count);
		void destroyRays();

	private:
		int m_degree;
		int m_rayCount;
		int m_samplesPerRay;

		LightRay *m_rays;

	protected:
		math::Vector m_position;
		math::Vector m_direction;
	};

} /* namespace manta */

#endif /* RAY_EMITTER_H */
