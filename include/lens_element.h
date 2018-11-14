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

		virtual bool transformLightRay(const LightRay *ray, LightRay *transformed) = 0;
		virtual math::real calculateFocalLength() const = 0;

		virtual void setIOR(math::real ior) { m_ior = ior; }
		math::real getIOR() const { return m_ior; }

	protected:
		math::real m_ior;
	};

} /*namespace manta */

#endif /* LENS_ELEMENT_H */
