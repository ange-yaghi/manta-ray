#ifndef GGX_DISTRIBUTION_H
#define GGX_DISTRIBUTION_H

#include <microfacet_distribution.h>

namespace manta {

	class VectorNode;

	struct GgxMemory {
		math::real width;
	};

	class GgxDistribution : public MicrofacetDistribution {
	public:
		GgxDistribution();
		~GgxDistribution();

		virtual void initializeSessionMemory(const IntersectionPoint *surfaceInteraction, NodeSessionMemory *memory, StackAllocator *stackAllocator) const;

		virtual math::Vector generateMicrosurfaceNormal(NodeSessionMemory *mem) const;
		virtual math::real calculateDistribution(const math::Vector &m, NodeSessionMemory *mem) const;
		virtual math::real calculateG1(const math::Vector &v, const math::Vector &m, NodeSessionMemory *mem) const;

		void setWidth(math::real width) { m_width = width; }
		math::real getWidth() const { return m_width; }

		void setWidthNode(VectorNode *node) { m_widthNode = node; }
		VectorNode *getWidthNode() const { return m_widthNode; }

		void setMinMapWidth(math::real power) { m_minMapWidth = power; }
		math::real getMinMapWidth() const { return m_minMapWidth; }

	protected:
		VectorNode *m_widthNode;
		math::real m_width;
		math::real m_minMapWidth;
	};

} /* namespace manta */

#endif /* GGX_DISTRIBUTION_H */
