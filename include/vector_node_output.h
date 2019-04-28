#ifndef VECTOR_NODE_OUTPUT_H
#define VECTOR_NODE_OUTPUT_H

#include <node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class VectorNodeOutput : public NodeOutput {
	protected:
		static const NodeType VectorType;

	public:
		VectorNodeOutput() : NodeOutput(&VectorType) {
			/* void */
		}

		virtual ~VectorNodeOutput() {
			/* void */
		}

		virtual void fullCompute(void *target) const;
	};

} /* namespace manta */

#endif /* VECTOR_NODE_OUTPUT_H */
