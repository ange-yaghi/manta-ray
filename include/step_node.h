#ifndef STEP_NODE_H
#define STEP_NODE_H

#include <node.h>

#include <vector_node_output.h>
#include <step_node_output.h>
#include <manta_math.h>

namespace manta {

	class StepNode : public Node {
	public:
		StepNode();
		virtual ~StepNode();

		StepNodeOutput *getMainOutput() { return &m_output; }

	protected:
		StepNodeOutput m_output;

		virtual void _initialize();

		virtual void registerInputs();
		virtual void registerOutputs();
	};

} /* namespace manta */

#endif /* STEP_NODE_H */
