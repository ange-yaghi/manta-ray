#ifndef MICROFACET_DISTRIBUTION_H
#define MICROFACET_DISTRIBUTION_H

#include <material_node.h>

#include <manta_math.h>
#include <intersection_point.h>

namespace manta {

	class StackAllocator;
	struct MaterialNodeMemory;

	class MicrofacetDistribution : public MaterialNode {
	public:
		MicrofacetDistribution();
		~MicrofacetDistribution();

		virtual math::Vector generateMicrosurfaceNormal(MaterialNodeMemory *mem) const = 0;
		virtual math::real calculatePDF(const math::Vector &o, const math::Vector &m, MaterialNodeMemory *mem) const;
		virtual math::real calculateDistribution(const math::Vector &m, MaterialNodeMemory *mem) const = 0;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, MaterialNodeMemory *mem) const = 0;
		virtual math::real bidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m, MaterialNodeMemory *mem) const;

		math::real smithBidirectionalShadowMasking(const math::Vector &i, const math::Vector &o, const math::Vector &m, MaterialNodeMemory *mem) const;
	};

} /* namespace manta */

#endif /* MICROFACET_DISTRIBUTION_H */
