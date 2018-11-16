#ifndef LENS_ELEMENT_H
#define LENS_ELEMENT_H

#include <manta_math.h>

namespace manta {

	class LightRay;

	class LensElement {
	public:
		static const math::real IOR_CROWN_GLASS;
	public:
		LensElement() { m_ior = IOR_CROWN_GLASS; }
		virtual ~LensElement() {}

		virtual bool transformLightRay(const LightRay *ray, LightRay *transformed) const = 0;
		virtual math::real calculateFocalLength() const = 0;

		void setDepth(math::real depth) { m_depth = depth; }
		math::real getDepth() const { return m_depth; }

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

		void setDirection(const math::Vector &direction) { m_direction = direction; }
		math::Vector getDirection() const { return m_direction; }

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		virtual void setIOR(math::real ior) { m_ior = ior; }
		math::real getIOR() const { return m_ior; }

	protected:
		math::real m_ior;

		math::Vector m_position;
		math::Vector m_direction;
		math::real m_depth;
		math::real m_radius;
	};

} /*namespace manta */

#endif /* LENS_ELEMENT_H */
