#ifndef LAMBERTIAN_BSDF_H
#define LAMBERTIAN_BSDF_H

#include <bsdf.h>

namespace manta {

	class VectorMaterialNode;

	struct LambertMemory {
		math::Vector reflectance;
	};

	class LambertianBSDF : public BSDF {
	public:
		LambertianBSDF();
		~LambertianBSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const;
	};

} /* namespace manta */

#endif /* LAMBERTIAN_BSDF_H */
