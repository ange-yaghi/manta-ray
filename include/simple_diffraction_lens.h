#ifndef SIMPLE_DIFFRACTION_LENS_H
#define SIMPLE_DIFFRACTION_LENS_H

/* While this lens works it is mainly for demonstration
   and is too slow/inefficient for practical use. */

#include <lens.h>

#include <fraunhofer_diffraction.h>
#include <simple_lens.h>

namespace manta {

	class SimpleDiffractionLens : public SimpleLens {
	public:
		SimpleDiffractionLens();
		virtual ~SimpleDiffractionLens();

		virtual bool transformRay(const LightRay *inputRay, LightRay *outputRay) const;
		virtual bool generateOutgoingRay(const math::Vector &sensorElement, const LensScanHint *hint, LightRay *targetRay) const;

		virtual void update();
	
		FraunhoferDiffraction *getDiffractionModel() { return &m_diffractionModel; }

	protected:
		// Diffraction model
		FraunhoferDiffraction m_diffractionModel;

		math::real m_diffractionPlaneZ;
		math::Vector m_diffractionPlaneLocation;
	};

} /* namespace manta */

#endif /* SIMPLE_DIFFRACTION_LENS_H */
