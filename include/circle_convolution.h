#ifndef CIRCLE_CONVOLUTION_H
#define CIRCLE_CONVOLUTION_H

#include <convolution.h>

#include <manta_math.h>

namespace manta {

	class CircleConvolution : public Convolution {
	public:
		CircleConvolution();
		~CircleConvolution();

		virtual math::real getExtents(const math::Vector &reference) const;
		virtual math::Vector sample(math::real x, math::real y) const;

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		void setColor(const math::Vector &color) { m_color = color; }
		math::Vector getColor() const { return m_color; }

	protected:
		math::real m_radius;
		math::Vector m_color;
	};

} /* namespace manta */

#endif /* CIRCLE_CONVOLUTION_H */
