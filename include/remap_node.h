#ifndef PIRANHA_REMAP_NODE_H
#define PIRANHA_REMAP_NODE_H

#include <piranha.h>

#include "remap_node_output.h"
#include "manta_math.h"

namespace manta {

	class NodeOutput;

	class RemapNode : public piranha::Node {
	public:
		RemapNode();
		RemapNode(const math::Vector &start, const math::Vector &end, piranha::pNodeInput inputNode);
		virtual ~RemapNode();

		piranha::NodeOutput *getMainOutput() { return &m_output; }

	protected:
		virtual void _evaluate();

		virtual void registerInputs();
		virtual void registerOutputs();

		RemapNodeOutput m_output;
	};

} /* namespace manta */

#endif /* PIRANHA_REMAP_NODE_H */
