#ifndef BICONVEX_LENS_H
#define BICONVEX_LENS_H

#include <lens_element.h>

#include <spherical_surface.h>

namespace manta {

	class BiconvexLens : public LensElement {
	public:
		BiconvexLens();
		virtual ~BiconvexLens();

		void setInputSurfaceRadius(math::real radius);
		void setOutputSurfaceRadius(math::real radius);

		void configure();
		virtual bool transformLightRay(const LightRay *ray, LightRay *transformed) const;

		virtual math::real calculateFocalLength() const;

	protected:
		SphericalSurface m_outputSurface;
		SphericalSurface m_inputSurface;		
	};

} /* namespace manta */

#endif /* BICONVEX_LENS_H */
