#ifndef PHONG_DISTRIBUTION_H
#define PHONG_DISTRIBUTION_H

#include <microfacet_distribution.h>

namespace manta {

	struct MaterialNodeMemory;
	class VectorMaterialNode;

	struct PhongMemory {
		math::real power;
	};

	class PhongDistribution : public MicrofacetDistribution {
	public:
		PhongDistribution();
		~PhongDistribution();

		virtual void initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const;

		virtual math::Vector generateMicrosurfaceNormal(MaterialNodeMemory *mem) const;
		virtual math::real calculateDistribution(const math::Vector &m, MaterialNodeMemory *mem) const;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, MaterialNodeMemory *mem) const;

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

		void setPowerNode(VectorMaterialNode *node) { m_powerNode = node; }
		VectorMaterialNode *getPowerNode() const { return m_powerNode; }

	protected:
		VectorMaterialNode *m_powerNode;
		math::real m_power;
	};

} /* namespace manta */

#endif /* PHONG_DISTRIBUTION_H */
