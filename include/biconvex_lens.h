#ifndef BICONVEX_LENS_H
#define BICONVEX_LENS_H

#include <lens_element.h>

#include <spherical_surface.h>

namespace manta {

	class BiconvexLens : public LensElement {
	public:
		BiconvexLens();
		virtual ~BiconvexLens();

		void setDepth(math::real depth);
		math::real getDepth() const { return m_depth; }

		void setPosition(const math::Vector &position);
		math::Vector getPosition() const { return m_position; }

		void setDirection(const math::Vector &direction);
		math::Vector getDirection() const { return m_direction; }

		void setRadius(math::real radius) { m_radius = radius; }
		math::real getRadius() const { return m_radius; }

		void setInputSurfaceRadius(math::real radius);
		void setOutputSurfaceRadius(math::real radius);

		void configure();
		virtual bool transformLightRay(const LightRay *ray, LightRay *transformed);

		virtual math::real calculateFocalLength() const;

	protected:
		SphericalSurface m_outputSurface;
		SphericalSurface m_inputSurface;

		math::Vector m_position;
		math::Vector m_direction;
		math::real m_depth;
		math::real m_radius;
	};

} /* namespace manta */

#endif /* BICONVEX_LENS_H */
