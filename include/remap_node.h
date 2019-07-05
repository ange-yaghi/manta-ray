#ifndef REMAP_NODE_H
#define REMAP_NODE_H

#include <node.h>

#include <remap_node_output.h>
#include <manta_math.h>

namespace manta {

	class NodeOutput;

	class RemapNode : public Node {
	public:
		RemapNode();
		RemapNode(const math::Vector &start, const math::Vector &end, pNodeInput inputNode);
		virtual ~RemapNode();

		NodeOutput *getMainOutput() { return &m_output; }

	protected:
		virtual void _evaluate();

		virtual void registerInputs();
		virtual void registerOutputs();

		RemapNodeOutput m_output;
	};

} /* namespace manta */

#endif /* REMAP_NODE_H */
