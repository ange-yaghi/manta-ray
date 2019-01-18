#ifndef BSDF_EMITTER_H
#define BSDF_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class BSDF;

	class BSDFEmitter : public RayEmitter {
	public:
		BSDFEmitter();
		virtual ~BSDFEmitter();

		virtual void generateRays();

		void setNormal(const math::Vector &normal) { m_normal = normal; }
		math::Vector getNormal() const { return m_normal; }

		void setIncident(const math::Vector &incident) { m_incident = math::negate(incident); }
		math::Vector getIncident() const { return m_incident; }

		void setBSDF(const BSDF *bsdf) { m_bsdf = bsdf; }
		const BSDF *getBSDF() const { return m_bsdf; }

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		void setWeightLimit(math::real weightLimit) { m_weightLimit = weightLimit; }
		math::real getWeightLimit() const { return m_weightLimit; }

	protected:
		math::Vector m_normal;
		math::Vector m_incident;

		math::real m_weightLimit;

		const BSDF *m_bsdf;

		int m_samples;
	};

} /* namespace manta */

#endif /* BSDF_EMITTER_H */
