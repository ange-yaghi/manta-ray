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

		virtual math::Vector getSensorElement(int x, int y) const;
		virtual void lensScan(const math::Vector &sensorElement, LensScanHint *target, int div) const;
		virtual bool generateOutgoingRay(const math::Vector &sensorElement, const LensScanHint *hint, LightRay *targetRay) const;

	protected:
		void lensScan(const math::Vector &sensorElement, math::real x, math::real y, math::real r, LensScanHint *target, int div) const;

		CircularAperture m_aperture;
		BiconvexLens m_lens;

		// Internal use only
		math::Vector m_sensorDeltaX;
		math::Vector m_sensorDeltaY;
		math::Vector m_sensorBottomRight;
		math::Vector m_right;
	};

} /* namespace manta */

#endif /* SIMPLE_LENS_H */
