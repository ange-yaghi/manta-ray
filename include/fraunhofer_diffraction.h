#ifndef FRAUNHOFER_DIFFRACTION_H
#define FRAUNHOFER_DIFFRACTION_H

#include <convolution.h>

#include <vector_map_2d.h>
#include <scalar_map_2d.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class Aperture;
	class CftEstimator2D;

	class FraunhoferDiffraction : public Convolution {
	public:
		FraunhoferDiffraction();
		~FraunhoferDiffraction();

		virtual math::real getExtents(const math::Vector &reference) const;
		virtual math::Vector sample(math::real x, math::real y) const;

		void generate(const Aperture *aperture, int outputResolution, math::real physicalSensorWidth);
		virtual void destroy();

		const VectorMap2D *getDiffractionPattern() const { return &m_diffractionPattern; }

		math::Vector getTotalFlux() const;
		void normalize();

		static math::Vector wavelengthToRgb(math::real_d wavelength);
		static math::real_d blackBodyRadiation(math::real_d wavelength);

	protected:
		void addLayer(const CftEstimator2D *estimator, math::real_d wavelength, VectorMap2D *target);

	protected:
		VectorMap2D m_diffractionPattern;

		math::real m_physicalSensorWidth;
		math::real m_sensorElementWidth;
	};

} /* namespace manta */

#endif /* FRAUNHOFER_DIFFRACTION_H */
