#ifndef SIMPLE_MONTE_CARLO_EMITTER_H
#define SIMPLE_MONTE_CARLO_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class SimpleMonteCarloEmitter : public RayEmitter {
	public:
		SimpleMonteCarloEmitter();
		virtual ~SimpleMonteCarloEmitter();

		virtual void generateRays();

		LightRay *getRay() const { return m_singleRay; }

		void setNormal(const math::Vector &normal) { m_normal = normal; }
		math::Vector getNormal() const { return m_normal; }

		void setIncident(const math::Vector &incident) { m_incidentDirection = incident; }
		math::Vector getIncident() const { return m_incidentDirection; }

	protected:
		LightRay *m_singleRay;

		math::Vector m_incidentDirection;
		math::Vector m_normal;
	};

}

#endif /* SIMPLE_MONTE_CARLO_EMITTER_H */
