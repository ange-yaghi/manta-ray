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

	class FraunhoferDiffraction {
	public:
		struct Settings {
			int maxSamples;
			int minWaveLength; // nm
			int maxWaveLength; // nm
			int wavelengthStep; // nm
			math::real_d frequencyMultiplier;
		};

	public:
		FraunhoferDiffraction();
		~FraunhoferDiffraction();

		void generate(const Aperture *aperture, int outputResolution, math::real physicalSensorWidth, const Settings *settings = nullptr);
		virtual void destroy();

		math::Vector samplePattern(math::real dx, math::real dy) const;
		math::Vector2 getPerturbance(math::real u, math::real v) const;
		const VectorMap2D *getDiffractionPattern() const { return &m_diffractionPattern; }

		math::Vector getTotalFlux() const;
		void normalize();

		static math::Vector wavelengthToRgb(math::real_d wavelength);
		static math::real_d blackBodyRadiation(math::real_d wavelength);

		static void defaultSettings(Settings *settings);

		math::real getPhysicalSensorWidth() const { return m_physicalSensorWidth; }

	protected:
		void addLayer(const CftEstimator2D *estimator, math::real_d wavelength, VectorMap2D *target);

	protected:
		VectorMap2D m_diffractionPattern;

		math::real m_physicalSensorWidth;
		math::real m_sensorElementWidth;
	};

} /* namespace manta */

#endif /* FRAUNHOFER_DIFFRACTION_H */
