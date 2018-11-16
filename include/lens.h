#ifndef LENS_H
#define LENS_H

#include <manta_math.h>

namespace manta {

	class Aperture;
	class LightRay;

	class Lens {
	public:
		Lens();
		virtual ~Lens();

		void setPosition(const math::Vector &position) { m_position = position; }
		math::Vector getPosition() const { return m_position; }

		void setDirection(const math::Vector &direction) { m_direction = direction; }
		math::Vector getDirection() const { return m_direction; }

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		void setUp(const math::Vector &up) { m_up = up; }

		virtual void setFocus(math::real dist) = 0;
		virtual void setFocalLength(math::real focalLength) = 0;
		virtual bool transformRay(const LightRay *inputRay, LightRay *outputRay) const = 0;

		math::Vector getSensorLocation() const { return m_sensorLocation; }
		math::Vector getSensorUp() const;
		math::Vector getSensorRight() const;

		virtual void initialize() = 0;
		virtual void update() = 0;

		Aperture *getAperture() { return r_aperture; }

	protected:
		math::Vector m_position;
		math::Vector m_direction;
		math::Vector m_up;
		math::Vector m_sensorLocation;
		math::real m_radius;

		void setAperture(Aperture *aperture) { r_aperture = aperture; }

	private:
		Aperture *r_aperture;
	};

} /* namespace manta */

#endif /* LENS_H */
