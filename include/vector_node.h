#ifndef VECTOR_NODE_H
#define VECTOR_NODE_H

#include <node.h>

#include <constructed_vector_node_output.h>

namespace manta {

	class VectorNode : public Node {
	public:
		VectorNode();
		virtual ~VectorNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

		const ConstructedVectorNodeOutput *getMainOutput() const { return &m_output; }

	protected:
		ConstructedVectorNodeOutput m_output;
	};

} /* namespace manta */

#endif /* VECTOR_NODE_H */
