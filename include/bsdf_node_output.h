#ifndef MANTARAY_BSDF_NODE_OUTPUT_H
#define MANTARAY_BSDF_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class BSDF;

	class BSDFNodeOutput : public piranha::NodeOutput {
	protected:
		static const piranha::ChannelType BSDFNodeType;

	public:
		BSDFNodeOutput() : NodeOutput(&BSDFNodeType) {
			m_bsdf = nullptr;
		}

		virtual ~BSDFNodeOutput() {
			/* void */
		}

		BSDF *getBsdf() const { return m_bsdf; }
		void setBsdf(BSDF *bsdf) { m_bsdf = bsdf; }

	protected:
		BSDF *m_bsdf;
	};

} /* namespace manta */

#endif /* MANTARAY_BSDF_NODE_OUTPUT_H */
