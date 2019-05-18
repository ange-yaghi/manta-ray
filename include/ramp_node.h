#ifndef RAMP_NODE_H
#define RAMP_NODE_H

#include <node.h>

#include <vector_node_output.h>
#include <ramp_node_output.h>
#include <manta_math.h>

namespace manta {

	class RampNode : public Node {
	public:
		RampNode();
		virtual ~RampNode();

		RampNodeOutput *getMainOutput() { return &m_output; }

	protected:
		RampNodeOutput m_output;

		virtual void _initialize();

		virtual void registerInputs();
		virtual void registerOutputs();
	};

} /* namespace manta */

#endif /* RAMP_NODE_H */
