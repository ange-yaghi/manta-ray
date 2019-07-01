#ifndef SRGB_NODE_H
#define SRGB_NODE_H

#include <node.h>

#include <srgb_node_output.h>

namespace manta {

	class SrgbNode : public Node {
	public:
		SrgbNode();
		virtual ~SrgbNode();

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

		const SrgbNodeOutput *getMainOutput() const { return &m_output; }

	protected:
		SrgbNodeOutput m_output;
	};

} /* namespace manta */

#endif /* SRGB_NODE_H */
