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
		VectorNodeOutput(bool scalar = false);
		virtual ~VectorNodeOutput();

		virtual void fullCompute(void *target) const;

	protected:
		virtual Node *generateInterface();

	protected:
		bool m_scalar;
	};

} /* namespace manta */

#endif /* VECTOR_NODE_OUTPUT_H */
