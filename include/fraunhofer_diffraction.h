#ifndef FRAUNHOFER_DIFFRACTION_H
#define FRAUNHOFER_DIFFRACTION_H

#include <convolution.h>

#include <vector_map_2d.h>
#include <scalar_map_2d.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class Aperture;

	class FraunhoferDiffraction : public Convolution {
	public:
		FraunhoferDiffraction();
		~FraunhoferDiffraction();

		virtual math::real getExtents(const math::Vector &reference) const;
		virtual math::Vector sample(math::real x, math::real y) const;

		void generate(const Aperture *aperture, int precision);
		virtual void destroy();

		const VectorMap2D *getDiffractionPattern() const { return &m_diffractionPattern; }
		const ScalarMap2D *getApertureFunction() const { return &m_apertureFunction; }

		math::Vector getTotalFlux() const;
		void normalize();

	protected:
		VectorMap2D m_diffractionPattern;
		ScalarMap2D m_apertureFunction;

		math::real m_physicalRadius;
	};

} /* namespace manta */

#endif /* FRAUNHOFER_DIFFRACTION_H */
