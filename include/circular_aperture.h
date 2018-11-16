#ifndef CIRCULAR_APERTURE_H
#define CIRCULAR_APERTURE_H

#include <aperture.h>

namespace manta {

	class CircularAperture : public Aperture {
	public:
		CircularAperture();
		~CircularAperture();

		virtual void setSize(math::real size) { m_radius = size; }

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		bool filter(math::real x, math::real y) const;

	protected:
		math::real m_radius;
	};

} /* namespace manta */

#endif /* CIRCULAR_APERTURE_H */
