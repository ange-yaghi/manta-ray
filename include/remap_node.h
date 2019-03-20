#ifndef REMAP_NODE_H
#define REMAP_NODE_H

#include <vector_material_node.h>

namespace manta {

	class RemapNode : public VectorMaterialNode {
	public:
		RemapNode();
		RemapNode(const math::Vector &start, const math::Vector &end, const VectorMaterialNode *inputNode);
		~RemapNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

		void setStart(const math::Vector &start) { m_start = start; }
		math::Vector getStart() { return m_start; }

		void setEnd(const math::Vector &end) { m_end = end; }
		math::Vector getEnd() { return m_end; }

		void setInputNode(const VectorMaterialNode *inputNode) { m_inputNode = inputNode; }
		const VectorMaterialNode *getInputNode() const { return m_inputNode; }

	protected:
		const VectorMaterialNode *m_inputNode;

		math::Vector m_start;
		math::Vector m_end;
	};

} /* namespace manta */

#endif /* REMAP_NODE_H */
