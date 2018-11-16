#ifndef SIMPLE_LENS_H
#define SIMPLE_LENS_H

#include <lens.h>

#include <circular_aperture.h>
#include <biconvex_lens.h>

namespace manta {

	class SimpleLens : public Lens {
	public:
		SimpleLens();
		virtual ~SimpleLens();

		virtual void setFocus(math::real dist);
		virtual void setFocalLength(math::real focalLength);
		virtual bool transformRay(const LightRay *inputRay, LightRay *outputRay) const;
		
		virtual void initialize();
		virtual void update();

	protected:
		CircularAperture m_aperture;
		BiconvexLens m_lens;
	};

} /* namespace manta */

#endif /* SIMPLE_LENS_H */
