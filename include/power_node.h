#ifndef POWER_NODE_H
#define POWER_NODE_H

#include <vector_node.h>

namespace manta {

	class PowerNode : public VectorNode {
	public:
		PowerNode();
		PowerNode(math::real power, const VectorNode *input);
		~PowerNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

		void setInputNode(const VectorNode *inputNode) { m_inputNode = inputNode; }
		const VectorNode *getInputNode() const { return m_inputNode; }

	protected:
		const VectorNode *m_inputNode;

		math::real m_power;
	};

} /* namespace manta */

#endif /* POWER_NODE_H */
