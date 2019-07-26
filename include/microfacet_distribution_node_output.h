#ifndef MANTARAY_MICROFACET_DISTRIBUTION_NODE_OUTPUT_H
#define MANTARAY_MICROFACET_DISTRIBUTION_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class MicrofacetDistribution;

	class MicrofacetDistributionNodeOutput : public piranha::NodeOutput {
	protected:
        static const piranha::ChannelType MicrofacetDistributionNodeType;

	public:
		MicrofacetDistributionNodeOutput() : NodeOutput(&MicrofacetDistributionNodeType) {
			m_distribution = nullptr;
		}

		virtual ~MicrofacetDistributionNodeOutput() {
			/* void */
		}

		const MicrofacetDistribution *getDistribution() const { return m_distribution; }
		void setDistribution(const MicrofacetDistribution *dist) { m_distribution = dist; }

	protected:
		const MicrofacetDistribution *m_distribution;
	};

} /* namespace manta */

#endif /* MICROFACET_DISTRIBUTION_NODE_OUTPUT_H */
