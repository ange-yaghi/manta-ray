#ifndef BSDF_NODE_OUTPUT_H
#define BSDF_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

	class BSDF;

	class BSDFNodeOutput : public NodeOutput {
	protected:
		static const NodeType BSDFNodeType;

	public:
		BSDFNodeOutput() : NodeOutput(&BSDFNodeType) {
			m_bsdf = nullptr;
		}

		virtual ~BSDFNodeOutput() {
			/* void */
		}

		const BSDF *getMediaInterface() const { return m_bsdf; }
		void setMediaInterface(const BSDF *bsdf) { m_bsdf = bsdf; }

	protected:
		const BSDF *m_bsdf;
	};

} /* namespace manta */

#endif /* BSDF_NODE_OUTPUT_H */
