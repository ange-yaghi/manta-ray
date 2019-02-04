#ifndef PERFECT_SPECULAR_REFLECTION_BSDF_H
#define PERFECT_SPECULAR_REFLECTION_BSDF_H

#include <bsdf.h>

namespace manta {

	class PerfectSpecularReflectionBSDF : public BSDF {
	public:
		PerfectSpecularReflectionBSDF();
		~PerfectSpecularReflectionBSDF();

		virtual math::Vector sampleF(const math::Vector &i, math::Vector *o, math::real *pdf) const;
	};

} /* namespace manta */

#endif /* PERFECT_SPECULAR_REFLECTION_BSDF_H */
