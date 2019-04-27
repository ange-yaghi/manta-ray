#ifndef REMAP_NODE_H
#define REMAP_NODE_H

#include <node.h>

#include <remap_node_output.h>
#include <manta_math.h>

namespace manta {

	class VectorNodeOutput;

	class RemapNode : public Node {
	public:
		RemapNode();
		RemapNode(const math::Vector &start, const math::Vector &end, const VectorNodeOutput *inputNode);
		~RemapNode();

		void setStart(const math::Vector &start) { m_start = start; }
		math::Vector getStart() { return m_start; }

		void setEnd(const math::Vector &end) { m_end = end; }
		math::Vector getEnd() { return m_end; }

		const VectorNodeOutput *getMainOutput() const { return &m_output; }

		void setInputNode(const VectorNodeOutput *inputNode) { m_inputNode = inputNode; }
		const VectorNodeOutput *getInputNode() const { return m_inputNode; }

	protected:
		virtual void _evaluate();

		virtual void registerInputs();
		virtual void registerOutputs();

		const VectorNodeOutput *m_inputNode;
		RemapNodeOutput m_output;

		math::Vector m_start;
		math::Vector m_end;
	};

} /* namespace manta */

#endif /* REMAP_NODE_H */
