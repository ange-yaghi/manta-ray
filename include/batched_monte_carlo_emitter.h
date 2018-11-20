#ifndef BATCHED_MONTE_CARLO_EMITTER_H
#define BATCHED_MONTE_CARLO_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class BatchedMonteCarloEmitter : public RayEmitter {
	public:
		BatchedMonteCarloEmitter();
		virtual ~BatchedMonteCarloEmitter();

		virtual void generateRays();

		void setNormal(const math::Vector &normal) { m_normal = normal; }
		math::Vector getNormal() const { return m_normal; }

		void setIncident(const math::Vector &incident) { m_incidentDirection = incident; }
		math::Vector getIncident() const { return m_incidentDirection; }

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		math::Vector getAverageIntensity() const;

	protected:
		math::Vector m_incidentDirection;
		math::Vector m_normal;

		int m_samples;
	};

} /* namespace manta */

#endif /* BATCHED_MONTE_CARLO_EMITTER_H */