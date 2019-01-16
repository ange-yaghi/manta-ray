#ifndef HEMI_MONTE_CARLO_EMITTER_H
#define HEMI_MONTE_CARLO_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class HemiMonteCarloEmitter : public RayEmitter {
	public:
		HemiMonteCarloEmitter();
		virtual ~HemiMonteCarloEmitter();

		virtual void generateRays();

		void setNormal(const math::Vector &normal) { m_normal = normal; }
		math::Vector getNormal() const { return m_normal; }

		void setSampleCount(int samples) { m_samples = samples; }
		int getSampleCount() const { return m_samples; }

		void setMeta(int meta) { m_meta = meta; }
		int getMeta() const { return m_meta; }

	protected:
		math::Vector m_normal;

		int m_meta;
		int m_samples;
	};

} /* namespace manta */

#endif /* HEMI_MONTE_CARLO_EMITTER_H */
