#ifndef REMAP_NODE_H
#define REMAP_NODE_H

#include <vector_node.h>

namespace manta {

	class RemapNode : public VectorNode {
	public:
		RemapNode();
		RemapNode(const math::Vector &start, const math::Vector &end, const VectorNode *inputNode);
		~RemapNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

		void setStart(const math::Vector &start) { m_start = start; }
		math::Vector getStart() { return m_start; }

		void setEnd(const math::Vector &end) { m_end = end; }
		math::Vector getEnd() { return m_end; }

		void setInputNode(const VectorNode *inputNode) { m_inputNode = inputNode; }
		const VectorNode *getInputNode() const { return m_inputNode; }

	protected:
		const VectorNode *m_inputNode;

		math::Vector m_start;
		math::Vector m_end;
	};

} /* namespace manta */

#endif /* REMAP_NODE_H */
