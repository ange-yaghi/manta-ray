#ifndef APERTURE_H
#define APERTURE_H

#include <manta_math.h>

namespace manta {

	class Aperture {
	public:
		Aperture();
		virtual ~Aperture();

		// Test whether a point lies inside or outside the aperture
		virtual bool filter(math::real x, math::real y) const = 0;

		void setRadius(math::real size) { m_radius = size; };
		math::real getRadius() const { return m_radius; };

	protected:
		math::real m_radius;
	};

} /* namespace manta */

#endif /* APERTURE_H */
