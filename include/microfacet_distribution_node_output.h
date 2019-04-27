#ifndef MICROFACET_DISTRIBUTION_NODE_OUTPUT_H
#define MICROFACET_DISTRIBUTION_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

	class MicrofacetDistribution;

	class MicrofacetDistributionNodeOutput : public NodeOutput {
	protected:
		static const NodeType MicrofacetDistributionNodeType;

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
