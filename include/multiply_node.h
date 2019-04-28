#ifndef MULTIPLY_NODE_H
#define MULTIPLY_NODE_H

#include <node.h>

#include <vector_node_output.h>
#include <multiply_node_output.h>
#include <manta_math.h>

namespace manta {

	class MultiplyNode : public Node {
	public:
		MultiplyNode();
		~MultiplyNode();

		MultiplyNodeOutput *getMainOutput() { return &m_output; }

	protected:
		MultiplyNodeOutput m_output;

		virtual void _initialize();

		virtual void registerInputs();
		virtual void registerOutputs();
	};

} /* namespace manta */

#endif /* MULTIPLY_NODE_H */
