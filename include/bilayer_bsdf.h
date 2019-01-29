#ifndef BILAYER_BSDF_H
#define BILAYER_BSDF_H

#include <bsdf.h>

namespace manta {

	class BilayerBSDF : public BSDF {
	public:
		BilayerBSDF();
		~BilayerBSDF();

		virtual math::Vector generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const;
		virtual math::real generateWeight(const math::Vector &n, const math::Vector &i, const math::Vector &m, const math::Vector &o) const;

		void setUpperLayer(const BSDF *upperLayer) { m_upperLayer = upperLayer; }
		const BSDF *getUpperLayer() const { return m_upperLayer; }

		void setLowerLayer(const BSDF *lowerLayer) { m_lowerLayer = lowerLayer; }
		const BSDF *getLowerLayer() const { return m_lowerLayer; }

	protected:
		const BSDF *m_upperLayer;
		const BSDF *m_lowerLayer;
	};

} /* namespace manta */

#endif /* BILAYER_BSDF_H */
