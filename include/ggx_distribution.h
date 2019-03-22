#ifndef GGX_DISTRIBUTION_H
#define GGX_DISTRIBUTION_H

#include <microfacet_distribution.h>

namespace manta {

	struct MaterialNodeMemory;
	class VectorMaterialNode;

	struct GgxMemory {
		math::real width;
	};

	class GgxDistribution : public MicrofacetDistribution {
	public:
		GgxDistribution();
		~GgxDistribution();

		virtual void initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const;

		virtual math::Vector generateMicrosurfaceNormal(MaterialNodeMemory *mem) const;
		virtual math::real calculateDistribution(const math::Vector &m, MaterialNodeMemory *mem) const;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, MaterialNodeMemory *mem) const;

		void setWidth(math::real width) { m_width = width; }
		math::real getWidth() const { return m_width; }

		void setWidthNode(VectorMaterialNode *node) { m_widthNode = node; }
		VectorMaterialNode *getWidthNode() const { return m_widthNode; }

		void setMinMapWidth(math::real power) { m_minMapWidth = power; }
		math::real getMinMapWidth() const { return m_minMapWidth; }

	protected:
		VectorMaterialNode *m_widthNode;
		math::real m_width;
		math::real m_minMapWidth;
	};

} /* namespace manta */

#endif /* GGX_DISTRIBUTION_H */
