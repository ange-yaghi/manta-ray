#ifndef STARBURST_APPROXIMATION_H
#define STARBURST_APPROXIMATION_H

#include <convolution.h>

#include <manta_math.h>

namespace manta {

	class StarburstApproximation : public Convolution {
	public:
		StarburstApproximation();
		virtual ~StarburstApproximation();

		virtual math::real getExtents(const math::Vector &reference) const;
		virtual math::Vector sample(math::real x, math::real y) const;

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		void setColor(const math::Vector &color) { m_color = color; }
		math::Vector getColor() const { return m_color; }

		void setBarWidth(math::real barWidth) { m_barWidth = barWidth; }
		math::real getBarWidth() const { return m_barWidth; }

	protected:
		math::real m_radius;
		math::real m_barWidth;
		math::Vector m_color;
	};

} /* namespace manta */

#endif /* STARBURST_APPROXIMATION_H */
