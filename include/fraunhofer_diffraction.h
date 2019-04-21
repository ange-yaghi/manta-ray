#ifndef FRAUNHOFER_DIFFRACTION_H
#define FRAUNHOFER_DIFFRACTION_H

#include <convolution.h>

#include <vector_map_2d.h>
#include <scalar_map_2d.h>
#include <complex_map_2d.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class Aperture;
	class CftEstimator2D;
	class CmfTable;
	class Spectrum;
	class TextureNode;

	class FraunhoferDiffraction {
	public:
		struct Settings {
			int maxSamples;
			int textureSamples;
			int minWaveLength; // nm
			int maxWaveLength; // nm
			int wavelengthStep; // nm
			math::real_d frequencyMultiplier;

			// This value controls what percentage of the flux comes from the ideal distribution (ie the center point)
			// with the remaining flux being contributed by the diffraction halo
			math::real_d deltaWeight;

			// Debugging flags
			bool saveApertureFunction;
		};

	public:
		FraunhoferDiffraction();
		~FraunhoferDiffraction();

		void generate(const Aperture *aperture, const TextureNode *dirtMap, int outputResolution, math::real physicalSensorWidth, CmfTable *colorTable, Spectrum *sourceSpectrum, const Settings *settings = nullptr);
		virtual void destroy();

		math::Vector samplePattern(math::real dx, math::real dy) const;
		const VectorMap2D *getDiffractionPattern() const { return &m_diffractionPattern; }
		const ComplexMap2D *getApertureFunction() const { return &m_apertureFunction; }

		void normalize(math::real_d deltaWeight);

		static math::Vector wavelengthToRgb(math::real_d wavelength);
		static math::real_d blackBodyRadiation(math::real_d wavelength);

		static void defaultSettings(Settings *settings);

		math::real getPhysicalSensorWidth() const { return m_physicalSensorWidth; }

	protected:
		void generateMap(const CftEstimator2D *estimator, int startwavelength, int endwavelength, int wavelengthStep, int textureSamples, VectorMap2D *target) const;

	protected:
		VectorMap2D m_diffractionPattern;
		ComplexMap2D m_apertureFunction;
		CmfTable *m_colorTable;
		Spectrum *m_sourceSpectrum;

		math::real m_physicalSensorWidth;
		math::real m_sensorElementWidth;
	};

} /* namespace manta */

#endif /* FRAUNHOFER_DIFFRACTION_H */
